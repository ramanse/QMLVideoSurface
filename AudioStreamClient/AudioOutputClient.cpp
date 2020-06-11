#include "AudioOutputClient.h"
#include <QAudioDeviceInfo>

AudioOutputClient::AudioOutputClient() :
    m_input(&m_data),
    m_output(&m_data)
{
    setOpenMode(QIODevice::ReadOnly);
}

bool AudioOutputClient::init(const QAudioFormat& format)
{
    m_format = format;
    m_decoder.setAudioFormat(m_format);
    if (!m_output.open(QIODevice::ReadOnly) || !m_input.open(QIODevice::WriteOnly))
    {
        return false;
    }

    return true;
}

void AudioOutputClient::resetBufferData()
{
    m_input.write(m_data, 0);
}

qint64 AudioOutputClient::readData(char* data, qint64 maxlen)
{
    memset(data, 0, maxlen);
    m_output.read(data, maxlen);
    return maxlen;
}

qint64 AudioOutputClient::writeData(const char* data, qint64 len)
{
    Q_UNUSED(data);
    Q_UNUSED(len);

    return 0;
}

void AudioOutputClient::setBufferData(QByteArray audioArray)
{
    m_input.write(audioArray.data(), audioArray.size());

}


