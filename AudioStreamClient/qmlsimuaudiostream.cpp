#include "qmlsimuaudiostream.h"
#include <QImage>
#include <QBuffer>
#include <QDebug>
#include <QPainter>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QAudioDeviceInfo>
#include <QAudioFormat>
#include <QAudioOutput>

QmlSimuAudioStream::QmlSimuAudioStream( QQuickItem *parent )
    : QQuickItem( parent )
{    
    QAudioDeviceInfo device = QAudioDeviceInfo::defaultOutputDevice();
    QAudioFormat desire_audio_romat = device.preferredFormat();
    m_socket = new RadioStreamSocket;
    m_socket->setHostPort(5757);
    m_socket->start();
    m_aClient = new AudioOutputClient;
    if (!m_aClient->init(desire_audio_romat))
    {

    }
    m_audioOutput = new QAudioOutput(desire_audio_romat);
    m_audioOutput->start(m_aClient);
    qCritical() <<"Starting here 1";

    connect(m_socket, SIGNAL(audioFrameAvailable(QByteArray)), m_aClient, SLOT(setBufferData(QByteArray)));
}

bool QmlSimuAudioStream::streamRadio() const
{
    return m_streamRadio;
}

void QmlSimuAudioStream::setStreamRadio(bool value)
{
    m_streamRadio = value;
    qCritical()<<"Switching stream"<< m_streamRadio;
    if(m_streamRadio)
        m_audioOutput->setVolume(0);

    else if(!m_isMuted)
        m_audioOutput->setVolume(m_currentVolume);

}

void QmlSimuAudioStream::resetStream()
{
    if(m_audioOutput){
        m_audioOutput->suspend();
        m_socket->resetSocketData();
        m_aClient->resetBufferData();
        m_audioOutput->resume();
    }
}



bool QmlSimuAudioStream::isMuted() const
{
    return m_isMuted;
}

void QmlSimuAudioStream::setIsMuted(bool value)
{
    m_isMuted = value;
    if(!m_streamRadio){
       m_audioOutput->setVolume((1 - m_isMuted) * m_currentVolume);
    }
}

qint32 QmlSimuAudioStream::tunerVolume() const
{
    return m_currentVolume;
}

void QmlSimuAudioStream::setTunerVolume(qint32 value)
{
    m_currentVolume = value/32.0;
    if(!m_streamRadio)
        m_audioOutput->setVolume(m_currentVolume);
}
