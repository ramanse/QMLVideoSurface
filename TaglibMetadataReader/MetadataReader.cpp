#include "MetadataReader.h"

#include <fileref.h>
#include <tag.h>
#include <id3v2tag.h>
#include <mpegfile.h>
#include <mp4file.h>
#include <attachedpictureframe.h>
#include <tpropertymap.h>
#include <QDir>
#include <QStringList>
#include <QElapsedTimer>
#include <QImage>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>
#include <iostream>


using namespace std;

const QString MetadataReader::m_mediaDomainName = "entertainment.player";
const QString MetadataReader::m_tunerDomainName = "entertainment.tuner";

MetadataReader::MetadataReader( const QString mockDataPath, const QString serviceName ):
    m_mockDataPath { mockDataPath },
    m_serviceName { serviceName },
    m_mediaFilesPath { QString::fromUtf8(qgetenv("SimuMediaFiles")) }
{    
    m_mediaCoversPath = QDir::homePath() + QDir::separator() + ".cache" + QDir::separator() + "SimuMediaCovers";
    if(!QDir(m_mediaCoversPath).exists()){
        QDir cacheDir(QDir::homePath()+QDir::separator()+".cache" );
        cacheDir.mkdir("SimuMediaCovers");
    }
}

MetadataReader &MetadataReader::Instance(const QString mockDataPath, const QString serviceName)
{
    static MetadataReader instance(mockDataPath,serviceName);
    return instance;
}

void MetadataReader::prepareMediaData(){
    QDir directory(m_mediaFilesPath + QDir::separator() + "media");
    if(qgetenv("SimuMediaFiles") == ""){
        qCritical()<< "Set enviroment SimuMediaFiles";
        return;
    }
    else{
        qCritical()<< "Media file " + m_mediaFilesPath;
    }
    m_currentMediaFilesFolder = directory.path();
    QJsonObject rootObject;
    directory.setSorting(QDir::Name);
    QJsonArray* allMediaObjects = getMediaObjectsFromFolder( directory );
    rootObject.insert("allMediaObjects",*allMediaObjects);
    rootObject.insert("currentPlaylist", *allMediaObjects);
    rootObject.insert("repeatMode", "OFF");
    rootObject.insert("isCoverValid", true);
    jsonDoc = QJsonDocument(rootObject);
    QFile jsonFile( m_mockDataPath + QDir::separator() + m_serviceName.toLower() +".json" );
    qCritical()<<"Writing to file"<<jsonFile.fileName();
    jsonFile.open(QFile::WriteOnly);
    jsonFile.write(jsonDoc.toJson());
    jsonFile.close();
    return;
}

void MetadataReader::prepareTunerData(){    
    QDir directory(m_mediaFilesPath + QDir::separator() + "radio" + QDir::separator() + "am");
    QJsonObject rootObject;
    QJsonArray* allMediaObjects = getMediaObjectsFromFolder( directory );
    rootObject.insert("am",*allMediaObjects);
    directory = m_mediaFilesPath + QDir::separator() + "radio" + QDir::separator() + "fm_dab";
    m_currentMediaFilesFolder = directory.path();
    allMediaObjects = getMediaObjectsFromFolder( directory );
    rootObject.insert("fm_dab",*allMediaObjects);
    QJsonDocument doc(rootObject);
    QFile jsonFile( m_mockDataPath + QDir::separator() + m_serviceName.toLower() +".json" );
    jsonFile.open(QFile::WriteOnly);
    jsonFile.write(doc.toJson());
    jsonFile.close();
    return;
}

void MetadataReader::setServerName(const QString serverName)
{
    m_serviceName = serverName;
}

void MetadataReader::updateMediaDBJson(const QString serverName)
{
    QFile jsonFile( m_mockDataPath + QDir::separator() + serverName.toLower() +".json" );
    qCritical()<<"Writing to file"<<jsonFile.fileName();
    jsonFile.open(QFile::WriteOnly);
    jsonFile.write(jsonDoc.toJson());
    jsonFile.close();
}
QJsonArray * MetadataReader::getMediaObjectsFromFolder( const QDir & directory ){

    QJsonArray* fcurrentMediaObjects = new QJsonArray;
    for(QFileInfo &file:directory.entryInfoList()) {
        if(file.isDir() && !file.isHidden()){
            fcurrentMediaObjects = getMediaObjectsFromFolder((QDir(file.filePath())));
        }
        else if(file.fileName().endsWith(".mp3") || file.fileName().endsWith(".mp4")){
            QJsonObject * mediaObject = mediaObjectFromTagInfo(file,fcurrentMediaObjects->size());
            if( mediaObject != nullptr)
                fcurrentMediaObjects->push_back(*mediaObject);
        }
    }
    return fcurrentMediaObjects;

}

QJsonObject * MetadataReader::mediaObjectFromTagInfo( const QFileInfo & file, qint32 fileIdIndex){

    TagLib::FileRef f( file.filePath().toStdString().c_str() );
    TagLib::String year;
    int discNumber( 0 ); //TODO: YTD
    int trackNumber( 0 ); //TODO: YTD
    qint32 playTime ( 0 );
    qint32 bitrate ( 0 );
    QString genre = "UNKNOWN";
    QString artistName = "UNKNOWN";
    QString album = file.fileName().mid(0, file.fileName().indexOf(".mp"));
    QString title = file.fileName().mid(0, file.fileName().indexOf(".mp"));
    QString coverFileName(file.fileName().mid(0, file.fileName().indexOf(".mp"))+".jpg");
    bool isVideoFile = file.fileName().endsWith(".mp4");

    if(f.tag()) {
        TagLib::Tag *tag = f.tag();
        TagLib::PropertyMap tags = f.file()->properties();
        unsigned int longest = 0;
        for(TagLib::PropertyMap::ConstIterator i = tags.begin(); i != tags.end(); ++i) {
            if (i->first.size() > longest) {
                longest = i->first.size();
            }
        }
        for(TagLib::PropertyMap::ConstIterator i = tags.begin(); i != tags.end(); ++i) {
            for(TagLib::StringList::ConstIterator j = i->second.begin(); j != i->second.end(); ++j) {
                if(QString::fromStdString(i->first.toCString()).startsWith("DATE")){
                    year = *j;
                }
                if(QString::fromStdString(i->first.toCString()).startsWith("DISCNUMBER"))
                    discNumber = QString(*j->toCString()).mid(0, 1).toInt();

                if(QString::fromStdString(i->first.toCString()).startsWith("TRACKNUMBER"))
                    trackNumber = QString(*j->toCString()).mid(0, 1).toInt();
            }
        }
        album = QString::fromStdString(tag->album().toCString());
        artistName = QString::fromStdString(tag->artist().toCString());
        genre = QString::fromStdString(tag->genre().toCString());
        title = QString::fromStdString(tag->title().toCString());
        if(!f.isNull() && f.audioProperties()) {

            TagLib::AudioProperties *properties = f.audioProperties();

            int seconds = properties->length() % 60;
            int minutes = (properties->length() - seconds) / 60;
            bitrate = properties->bitrate();
            playTime = (minutes * 60) + seconds;
        }

        QFile preFetchedImage(m_currentMediaFilesFolder + QDir::separator() + coverFileName);
        if(preFetchedImage.exists()){
            QFile::copy(m_currentMediaFilesFolder + QDir::separator() + coverFileName, m_mediaCoversPath + QDir::separator() + coverFileName);
        }
        else{

            TagLib::MPEG::File fileCover(file.filePath().toStdString().c_str());
            TagLib::ID3v2::Tag *m_tag = fileCover.ID3v2Tag(true);
            TagLib::ID3v2::FrameList frameList = m_tag->frameList("APIC");
            TagLib::ID3v2::AttachedPictureFrame *coverImg = static_cast<TagLib::ID3v2::AttachedPictureFrame *>(frameList.front());
            if( coverImg ){
                QFile imgFile(m_mediaCoversPath + QDir::separator() + coverFileName);
                if(!imgFile.exists()){
                    imgFile.open(QIODevice::WriteOnly);
                    QImage coverQImg;
                    coverQImg.loadFromData((const uchar *) coverImg->picture().data(), coverImg->picture().size());
                    coverQImg.save(&imgFile,"JPG");
                    imgFile.close();
                }
            }
        }
    }

    QJsonObject *mediaObject = new QJsonObject;
    mediaObject->insert("Album", album == ""?file.fileName().mid(0, file.fileName().indexOf(".mp")):album);
    mediaObject->insert("AlbumArtist", artistName == ""?file.fileName().mid(0, file.fileName().indexOf(".mp")):artistName);
    mediaObject->insert("Artist", artistName == ""?file.fileName().mid(0, file.fileName().indexOf(".mp")): artistName);
    mediaObject->insert("Composer", "");
    mediaObject->insert("Genre", genre);
    mediaObject->insert("Year", year.toCString());
    mediaObject->insert("Title", title == ""?file.fileName().mid(0, file.fileName().indexOf(".mp")):title);
    mediaObject->insert("PlayTime", playTime);
    mediaObject->insert("TrackNo", trackNumber);
    mediaObject->insert("DiscNumber", discNumber);
    mediaObject->insert("CoverHash", coverFileName);
    mediaObject->insert("FileName", file.fileName());
    mediaObject->insert("FolderName", "Folder Name#1#1");
    mediaObject->insert("FileID", fileIdIndex);
    mediaObject->insert("FileHash", "#music0.mp3");
    mediaObject->insert("Stream", "null");
    mediaObject->insert("IsCompilation", true);
    mediaObject->insert("MediumID", "USB1");
    mediaObject->insert("IsStream", false);
    mediaObject->insert("Bitrate", bitrate);
    mediaObject->insert("IsPartOfCurrentPlayingTrack", false);
    mediaObject->insert("PositionInCurrentPlaylist", 0);
    mediaObject->insert("MoodName", "Mood Name#0");
    mediaObject->insert("IsProvidedByGracenote", true);
    mediaObject->insert("TotalCountOfTracksOnAlbum", 3);
    mediaObject->insert("MediaObjectType", isVideoFile?3:2);
    mediaObject->insert("SearchReferenceID", "SearchReferenceID#0");
    mediaObject->insert("Decade", "null");
    mediaObject->insert("UserRating", 0);
    mediaObject->insert("TotalCountOfAlbumsRelatedToArtist", 2);
    mediaObject->insert("TotalCountOfTracksRelatedToArtist", 6);

    return mediaObject;

}

