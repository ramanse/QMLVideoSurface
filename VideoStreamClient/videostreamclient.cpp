#include "videostreamclient.h"
#include <QNetworkProxy>

VideoStreamClient::VideoStreamClient(QObject *parent)
    : QObject(parent)
    , m_chunkSize(-1)
{
    // Create socket
}

VideoStreamClient::~VideoStreamClient()
{
    qCritical()<<"Video Stream disconnected";
    if(socket){
    socket->disconnectFromHost();
    delete socket;
    }
}

void VideoStreamClient::setHostPort(int port){
    m_port = port;
}
void VideoStreamClient::start()
{
    qCritical() << "Thread started";
    socket = new QTcpSocket();
    if(socket){
        connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
        connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
        socket->setProxy(QNetworkProxy::NoProxy);
        socket->connectToHost("127.0.0.1",m_port);
    }
}

void VideoStreamClient::readyRead()
{    
    while (socket->bytesAvailable()) {
        if (socket->bytesAvailable()<sizeof(m_chunkSize))
            return;
        if(m_chunkSize < 0) {
            socket->read((char*)&m_chunkSize,sizeof(m_chunkSize));
        }
        if (socket->bytesAvailable()<m_chunkSize)
            return;
        if(m_chunkSize > 0) {
            data = socket->read(m_chunkSize);
            if(isValidJPEG(data)){
                processMessage(data);
                data.clear();
            }
        }
        m_chunkSize = -1;
    }
}

void VideoStreamClient::disconnected()
{

}

void VideoStreamClient::processMessage(QByteArray message)
{    
    emit frameAvailable(message);
}

bool VideoStreamClient::isValidJPEG(QByteArray data)
{
    if ( data.length() < 2)
        return false;

    int totalBytes = data.length();
    const char *bytes = (const char*)data.constData();

    return (bytes[0] == (char)0xff &&
            bytes[1] == (char)0xd8 &&
            bytes[totalBytes-2] == (char)0xff &&
            bytes[totalBytes-1] == (char)0xd9);

}
