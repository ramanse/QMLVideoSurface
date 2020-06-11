#ifndef AudioOutputClient_H
#define AudioOutputClient_H


#pragma once

#include <QIODevice>
#include <QBuffer>
#include <QAudioDecoder>
#include <QAudioFormat>


class AudioOutputClient : public QIODevice
{
    Q_OBJECT

public:
    AudioOutputClient();
    bool init(const QAudioFormat& format);
    void resetBufferData();

public slots:
    void setBufferData(QByteArray audioArray);

protected:
    qint64 readData(char* data, qint64 maxlen) override;
    qint64 writeData(const char* data, qint64 len) override;

private:
    QBuffer m_input;
    QBuffer m_output;
    QByteArray m_data;
    QAudioDecoder m_decoder;
    QAudioFormat m_format;


};
#endif // AudioOutputClient_H
