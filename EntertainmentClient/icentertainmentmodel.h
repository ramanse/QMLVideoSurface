#ifndef ICENTERTAINMENTMODEL_H
#define ICENTERTAINMENTMODEL_H


#include <QObject>
#include <QAbstractListModel>

class ICMediaObject:public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString artist READ artist WRITE setArtist NOTIFY artistChanged)
    Q_PROPERTY(QString album READ album WRITE setAlbum NOTIFY albumChanged)
    Q_PROPERTY(qint32 playTime READ playTime WRITE setPlayTime NOTIFY playTimeChanged)
    Q_PROPERTY(QString coverHash READ coverHash WRITE setCoverHash NOTIFY coverHashChanged)

public:
    ICMediaObject(const QString &title, const QString &album,
                  const QString &artist, const qint32 &playTime,
                  const QString &coverHash);

    QString title() const;
    QString artist() const;
    QString album() const;
    qint32 playTime() const;
    QString coverHash() const;

    void setTitle(const QString &value);
    void setArtist(const QString &value);
    void setAlbum(const QString &value);
    void setPlayTime(const qint32 &value);
    void setCoverHash(const QString &value);
 signals:
    void titleChanged();
    void artistChanged();
    void albumChanged();
    void playTimeChanged();
    void coverHashChanged();
private:
    QString m_title;
    QString m_album;
    QString m_artist;
    qint32 m_playTime;
    QString m_coverHash;
};

class ICEntertainmentModel : public QAbstractListModel
{
    Q_OBJECT
    //Q_PROPERTY(qint32 currentICListIndex READ currentICListIndex WRITE setCurrentICListIndex NOTIFY currentICListIndexChanged)


public:
    enum ICEntertainmentRoles{
        TitleRole = Qt::UserRole +1,
        AlbumRole,
        ArtistRole,
        PlayTimeRole,
        CoverHashRole
    };
    ICEntertainmentModel(QObject *parent = 0);

    void addMedia(ICMediaObject *mObject);
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    void updateCurrentMediaModel(QJsonArray array);

protected:
    QHash<int, QByteArray> roleNames() const;


private:
    QList<ICMediaObject *> m_mediaObjs;

};

#endif // ICENTERTAINMENTMODEL_H
