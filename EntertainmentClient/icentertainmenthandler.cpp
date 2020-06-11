#include "icentertainmenthandler.h"

qint32 ICEntertainmentHandler::currentPlayTime() const
{
    return m_currentPlayTime;
}

void ICEntertainmentHandler::setCurrentPlayTime(const qint32 &value)
{
    m_currentPlayTime = value;
    emit currentPlayTimeChanged();
}

qint32 ICEntertainmentHandler::curentListIndex() const
{
    return m_currentListIndex;
}

void ICEntertainmentHandler::setCurentListIndex(const qint32 &value)
{
    if(m_currentListIndex != value){
        m_currentListIndex = value;
        emit currentListIndexChanged();
    }
}

qint16 ICEntertainmentHandler::currentAudioSource() const
{
    return m_currentAudioSource;
}

void ICEntertainmentHandler::setCurrentAudioSource(const qint16 &value)
{
   if(m_currentAudioSource != value){
        m_currentAudioSource = value;
    }
    emit currentAudioSourceChanged();
}
ICEntertainmentModel *ICEntertainmentHandler::currentStationsList() const
{
    return m_stationsList;
}

ICEntertainmentModel* ICEntertainmentHandler::currentPlayList() const
{
    return m_playList;
}

void ICEntertainmentHandler::updateProperties(QByteArray array)
{
    QJsonDocument jDoc = QJsonDocument::fromBinaryData(array);
    QJsonObject jObj = jDoc.object();
    if(!jObj.value("playList").isUndefined() && jObj.value("playList").isArray()){
        qCritical()<<"Received playlist from media";
        m_playList->updateCurrentMediaModel(jObj.value("playList").toArray());
    }
    else if(!jObj.value("stationsList").isUndefined() && jObj.value("stationsList").isArray()){
        m_stationsList->updateCurrentMediaModel(jObj.value("stationsList").toArray());
    }
    else if(!jObj.value("currentIndex").isUndefined()){
        setCurentListIndex(jObj.value("currentIndex").toInt());
    }
    else if(!jObj.value("currentAudioSource").isUndefined() ){
          qCritical()<<"Received audioSource  from media";
        setCurrentAudioSource(static_cast<qint16>(jObj.value("currentAudioSource").toInt()));
    }
    else if(!jObj.value("inputSize").isUndefined()){
        m_client->updateChunkSize(static_cast<qint64>(jObj.value("inputSize").toInt()));

    }
    else if(!jObj.value("currentPlayTime").isUndefined()){
        setCurrentPlayTime(static_cast<qint64>(jObj.value("currentPlayTime").toInt()));
    }

}

void ICEntertainmentHandler::updatePlayListwithICIndex(qint32 iclistIndex)
{
    QJsonObject listIndex;
    listIndex.insert("icListIndex", iclistIndex);
    QJsonDocument f_indextJson(listIndex);
    QByteArray byteArray = (f_indextJson.toBinaryData());
    m_client->writeData(byteArray);
}
void ICEntertainmentHandler::updateAudioSource(qint16 audioSource)
{
    QJsonObject listIndex;
    listIndex.insert("icAudioSource", audioSource);
    QJsonDocument f_indextJson(listIndex);
    QByteArray byteArray = (f_indextJson.toBinaryData());
    m_client->writeData(byteArray);
}

ICEntertainmentHandler::ICEntertainmentHandler()
    :m_currentPlayTime(0),m_currentListIndex(0),m_currentAudioSource(0)
{
    m_client = ICEntertainmentClient::instance();
    connect(m_client,&ICEntertainmentClient::dataAvailable,this,&ICEntertainmentHandler::updateProperties);
    m_playList = new ICEntertainmentModel();
    m_stationsList = new ICEntertainmentModel();
}

ICEntertainmentHandler::~ICEntertainmentHandler()
{

}
