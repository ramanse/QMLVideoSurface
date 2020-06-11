#ifndef ICENTERTAINMENTHANDLER_H
#define ICENTERTAINMENTHANDLER_H

#include <QObject>
#include "icentertainmentclient.h"
#include "icentertainmentmodel.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>


class ICEntertainmentHandler:public QObject
{
    Q_OBJECT

    Q_PROPERTY(qint32 currentPlayTime READ currentPlayTime WRITE setCurrentPlayTime NOTIFY currentPlayTimeChanged)
    Q_PROPERTY(qint32 curentListIndex READ curentListIndex WRITE setCurentListIndex NOTIFY currentListIndexChanged)
    Q_PROPERTY(qint16 currentAudioSource READ currentAudioSource WRITE setCurrentAudioSource NOTIFY currentAudioSourceChanged)
    Q_PROPERTY(ICEntertainmentModel* currentPlayList READ currentPlayList CONSTANT)
    Q_PROPERTY(ICEntertainmentModel* currentStationsList READ currentStationsList CONSTANT)

public:
    Q_INVOKABLE void updatePlayListwithICIndex(qint32 listIndex);
    Q_INVOKABLE void updateAudioSource(qint16 audioSource);

public:
    ICEntertainmentHandler();
    ~ICEntertainmentHandler();

    qint32 currentPlayTime() const;
    void setCurrentPlayTime(const qint32 &value);

    qint32 curentListIndex() const;
    void setCurentListIndex(const qint32 &value);

    qint16 currentAudioSource() const;
    void setCurrentAudioSource(const qint16 &value);

    ICEntertainmentModel* currentPlayList() const;
    ICEntertainmentModel* currentStationsList() const;

 signals:
    void currentPlayTimeChanged();
    void currentListIndexChanged();
    void currentAudioSourceChanged();

private slots:
    void updateProperties(QByteArray array);

private:
    qint32 m_currentPlayTime;
    qint32 m_currentListIndex;
    qint16 m_currentAudioSource;
    ICEntertainmentClient *m_client;
    ICEntertainmentModel *m_playList;
    ICEntertainmentModel *m_stationsList;

};


#endif // ICENTERTAINMENTHANDLER_H
