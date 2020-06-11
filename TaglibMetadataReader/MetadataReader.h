#pragma once


#include <QProcessEnvironment>
#include <QDir>
#include <QFileInfo>


class MetadataReader : public QObject
{
    Q_OBJECT
    MetadataReader( const QString mockDataPath, const QString serviceName );

public:
    MetadataReader(MetadataReader const&) = delete;
    void operator=(MetadataReader const&) = delete;
    static MetadataReader &Instance(const QString mockDataPath, const QString serviceName);

    void prepareMediaData( );
    void prepareTunerData( );
    void setServerName(const QString serverName);
    void updateMediaDBJson(const QString serverName);

private:
    QJsonArray *getMediaObjectsFromFolder(const QDir & directory);
    QJsonObject *mediaObjectFromTagInfo(const QFileInfo & fileInfo, qint32 fileIdIndex);
    static const QString m_mediaDomainName;
    static const QString m_tunerDomainName;
    QJsonDocument jsonDoc;
    const QString m_mockDataPath;
    QString m_serviceName;
    QString m_mediaFilesPath;
    QString m_mediaCoversPath;
    QString m_currentMediaFilesFolder;

};


