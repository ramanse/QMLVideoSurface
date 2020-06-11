#pragma once

#include<QQuickItem>
#include <QQuickPaintedItem>
#include <QImage>
#include <QPointer>
#include <QPixmap>
#include "../../core/utilityexports.h"
#include "radiostreamsocket.h"
#include "AudioOutputClient.h"
#include <QAudioOutput>

class QmlSimuAudioStream : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(bool streamRadio READ streamRadio WRITE setStreamRadio)
    Q_PROPERTY(qint32 tunerVolume READ tunerVolume WRITE setTunerVolume)
    Q_PROPERTY(bool isMuted READ isMuted WRITE setIsMuted)

    public:
    Q_INVOKABLE void resetStream();

public:
    QmlSimuAudioStream( QQuickItem *parent = nullptr );


protected:
    bool streamRadio() const;
    void setStreamRadio( bool value );

    bool isMuted() const;
    void setIsMuted( bool value );

    qint32 tunerVolume() const;
    void setTunerVolume( qint32 value );


private:

    bool m_streamRadio;
    bool m_isMuted;
    qreal m_currentVolume;

    qint16 m_port;
    RadioStreamSocket *m_socket;
    AudioOutputClient *m_aClient;
    QAudioOutput *m_audioOutput;


};
