#ifndef RADIOSTREAMSIMU_H
#define RADIOSTREAMSIMU_H


#pragma once

#include <QIODevice>
#include <QBuffer>
#include <QAudioDecoder>
#include <QAudioFormat>
#include <QAudioBuffer>
#include <QFile>
#include <QUrl>
#include <QTcpServer>
#include <QTcpSocket>
#include <QQueue>
#include "SimuSocket.h"


class RadioStreamSimu : public QIODevice
{
    Q_OBJECT

public:
    RadioStreamSimu();
    bool init(const QAudioFormat& format);

    enum State { Playing, Stopped, Paused };

    void play(const QString &filePath);
    void play();
    void stop();
    void pause();
    void resume();
    void setMedia(const QUrl &filePath);
    void setMedia(const QString &filePath);
    bool atEnd() const override;
    int getQueueSize(){
        return m_audioQueue.length();
    }

    void setBufferData(QByteArray audioArray);
protected:
    qint64 readData(char* data, qint64 maxlen) override;
    qint64 writeData(const char* data, qint64 len) override;

 public slots:
    void bufferReadyFromProbe(QAudioBuffer audioBuffer);

private:
    constexpr static const int MAX_CONNECTIONS = 4;
    QFile m_file;
    QBuffer m_input;
    QBuffer m_output;
    QByteArray m_data;
    QAudioDecoder m_decoder;
    QAudioFormat m_format;
    State m_state;
    QTcpServer *server;

    std::list<SimuSocket*> m_socketsHandler;
    QQueue <QByteArray> m_audioQueue;


    bool isInited;
    bool isDecodingFinished;
    bool isBufferReady;
    void clear();

private slots:
    void bufferReady();
    void finished();
    void newConnection();
    void removeConnection(SimuSocket *socket);

signals:
    void stateChanged(RadioStreamSimu::State state);
    void newData(const QByteArray& data);
};
#endif // RADIOSTREAMSIMU_H
