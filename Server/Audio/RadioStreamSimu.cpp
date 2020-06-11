#include "RadioStreamSimu.h"
#include <QAudioDeviceInfo>

RadioStreamSimu::RadioStreamSimu() :
    m_input(&m_data),
    m_output(&m_data),
    m_state(State::Stopped)
{
    setOpenMode(QIODevice::ReadOnly);

    isInited = false;
    isDecodingFinished = false;
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
    qint16 f_Port = 5757;//d.object().value(QString("variantID" + qgetenv("CLIENT_ID"))).toInt();
    if(!server->listen(QHostAddress::Any, 5757))
    {
        qDebug() << "Server could not start!";
    }

}
void RadioStreamSimu::newConnection()
{
    qCritical() << "Connection requested";
    //Allowed 5 connections as of now

    if(m_socketsHandler.size() <= MAX_CONNECTIONS){
        QTcpSocket *f_socket = server->nextPendingConnection();
        SimuSocket *listScoket = new SimuSocket(f_socket);
        m_socketsHandler.push_back(listScoket);
        connect(m_socketsHandler.back(), SIGNAL(lostConnection(SimuSocket*)), this, SLOT(removeConnection(SimuSocket*)));
        //connect(m_socketsHandler.,SIGNAL(disconnected()),this,SLOT(removeConnection()));
    }
    else
        qCritical() << "Max connection reached, No more allowed";
}

void RadioStreamSimu::removeConnection(SimuSocket *socket)
{
    qCritical()<<"Connection removed ";
    m_socketsHandler.remove(socket);
     qCritical()<<"connection" <<m_socketsHandler.size();

}
bool RadioStreamSimu::init(const QAudioFormat& format)
{
    m_format = format;
    m_decoder.setAudioFormat(m_format);

    connect(&m_decoder, SIGNAL(bufferReady()), this, SLOT(bufferReady()));
    connect(&m_decoder, SIGNAL(finished()), this, SLOT(finished()));

    if (!m_output.open(QIODevice::ReadOnly) || !m_input.open(QIODevice::WriteOnly))
    {
        return false;
    }

    isInited = true;

    return true;
}

qint64 RadioStreamSimu::readData(char* data, qint64 maxlen)
{
     memset(data, 0, maxlen);

    if (m_state == State::Playing)
    {
        m_output.read(data, maxlen);


        if (maxlen > 0)
        {
            QByteArray buff(data, maxlen);
            emit newData(buff);
        }
        if (atEnd())
        {
            stop();
        }
    }

    return maxlen;
}

qint64 RadioStreamSimu::writeData(const char* data, qint64 len)
{
    Q_UNUSED(data);
    Q_UNUSED(len);

    return 0;
}

void RadioStreamSimu::bufferReadyFromProbe(QAudioBuffer audioBuffer)
{
    Q_UNUSED(audioBuffer)//this audioBuffer media player cant be played very distorted
    if(!m_audioQueue.empty()){
        QByteArray bufToAudio = m_audioQueue.dequeue();
        for(SimuSocket *client:m_socketsHandler){
            client->write(bufToAudio);
        }
    }
}

void RadioStreamSimu::play(const QString &filePath)
{
    clear();

    m_file.setFileName(filePath);

    if (!m_file.open(QIODevice::ReadOnly))
    {
        return;
    }

    m_decoder.setSourceDevice(&m_file);
    m_decoder.start();

    m_state = State::Playing;
    emit stateChanged(m_state);
}

void RadioStreamSimu::play(){
    clear();

    if (!m_file.open(QIODevice::ReadOnly))
    {
        qCritical()<< "Cannot open Audio File";
        return;
    }
    m_decoder.setSourceDevice(&m_file);
    m_decoder.start();

    m_state = State::Playing;
    emit stateChanged(m_state);
}
void RadioStreamSimu::stop()
{
    clear();


    m_state = State::Stopped;
    emit stateChanged(m_state);
}

void RadioStreamSimu::resume()
{
    m_state = State::Playing;
    emit stateChanged(m_state);
}

void RadioStreamSimu::pause()
{
    m_state = State::Paused;
    if(!m_audioQueue.empty())
        m_audioQueue.clear();
    emit stateChanged(m_state);
}

void RadioStreamSimu::setMedia(const QUrl &filePath)
{
    m_file.close();
    m_file.setFileName(filePath.toEncoded());
}

void RadioStreamSimu::setMedia(const QString &filePath)
{
    qCritical()<< "Setting media "<< filePath;
    m_file.close();
    m_file.setFileName(filePath);
}


void RadioStreamSimu::clear()
{
    m_decoder.stop();
    m_data.clear();
    m_file.close();
    if(!m_audioQueue.empty())
        m_audioQueue.clear();
    isDecodingFinished = false;

    
}

// Is finish of file
bool RadioStreamSimu::atEnd() const
{
    return m_output.size()
            && m_output.atEnd()
            && isDecodingFinished;
}


void RadioStreamSimu::bufferReady() // SLOT
{
    isBufferReady = true;

    const QAudioBuffer &buffer = m_decoder.read();


    const int length = buffer.byteCount();
    const char *data = buffer.constData<char>();

    QByteArray buff(data, length);
    if(m_state == State::Playing)
        m_audioQueue.enqueue(buff);
    //Asking clients to play their own data
    //m_input.write(buff.data(), buff.size());
    isBufferReady = false;
}
void RadioStreamSimu::finished()
{
    isDecodingFinished = true;
}


