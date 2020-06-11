#include "icentertainmentmodel.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
ICMediaObject::ICMediaObject(const QString &title, const QString &album, const QString &artist, const qint32 &playTime, const QString &coverHash)
{
    m_title = title;
    m_album = album;
    m_artist = artist;
    m_playTime = playTime;
    m_coverHash = coverHash;
}

QString ICMediaObject::title() const{
    return m_title;
}

QString ICMediaObject::artist() const{
    return m_artist;
}

QString ICMediaObject::album() const{
    return m_album;
}

qint32 ICMediaObject::playTime() const{
    return m_playTime;
}

QString ICMediaObject::coverHash() const{
    return m_coverHash;
}

void ICMediaObject::setTitle(const QString &value)
{
    m_title = value;
    emit titleChanged();
}

void ICMediaObject::setArtist(const QString &value)
{
    m_artist = value;
    emit artistChanged();
}

void ICMediaObject::setAlbum(const QString &value)
{
    m_album = value;
    emit albumChanged();
}

void ICMediaObject::setPlayTime(const qint32 &value)
{
    m_playTime = value;
    emit playTimeChanged();
}

void ICMediaObject::setCoverHash(const QString &value)
{
    m_coverHash = value;
    emit coverHashChanged();
}



ICEntertainmentModel::ICEntertainmentModel(QObject *parent):
    QAbstractListModel (parent){
}

void ICEntertainmentModel::addMedia(ICMediaObject *mObject){
    QObject::connect(mObject, &ICMediaObject::titleChanged, this, [this, mObject]() {
        auto index = this->index(m_mediaObjs.indexOf(mObject));
        this->dataChanged(index, index);
    });
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_mediaObjs.append(mObject);
    endInsertRows();
}

int ICEntertainmentModel::rowCount(const QModelIndex &parent) const{
    Q_UNUSED(parent)
    return m_mediaObjs.count();
}

QVariant ICEntertainmentModel::data(const QModelIndex &index, int role) const{
    if(index.row() < 0 || index.row() >= m_mediaObjs.count())
        return  QVariant();

    ICMediaObject *mediaObj = m_mediaObjs[index.row()];
    switch (role) {
    case TitleRole: return mediaObj->title();
    case AlbumRole: return mediaObj->album();
    case ArtistRole: return mediaObj->artist();
    case PlayTimeRole: return mediaObj->playTime();
    case CoverHashRole: return mediaObj->coverHash();
    default: return  QVariant();
    }

}

QHash<int, QByteArray> ICEntertainmentModel::roleNames() const{
    QHash<int, QByteArray> roles;
    roles[TitleRole] = "title";
    roles[AlbumRole] = "album";
    roles[ArtistRole] = "artist";
    roles[PlayTimeRole] = "playTime";
    roles[CoverHashRole] = "coverHash";
    return roles;

}

void ICEntertainmentModel::updateCurrentMediaModel(QJsonArray array)
{
    if (rowCount() > 0) {
        beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
        qDeleteAll(m_mediaObjs);
        m_mediaObjs.clear();
        endRemoveRows();
    }
    qCritical()<<"Array size is "<<array.count();

    foreach (const QJsonValue & value, array) {
        QJsonObject obj = value.toObject();
        addMedia(new ICMediaObject(obj["title"].toString(),obj["album"].toString(),obj["artist"].toString(),
                obj["duration"].toInt(), obj["cover"].toString()));


    }

}
