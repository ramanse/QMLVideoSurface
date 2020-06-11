#include "videostreamclient.h"
#include <QNetworkProxy>

VideoStreamClient::VideoStreamClient(QObject *parent)
    : QObject(parent)
{
    // Create socket
}

VideoStreamClient::~VideoStreamClient()
{
    socket->disconnectFromHost();
    delete socket;
}

void VideoStreamClient::setHostPort(int port){
    m_port = port;
}
void VideoStreamClient::start()
{
    qCritical() << "Thread started";
    socket = new QTcpSocket();
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    socket->setProxy(QNetworkProxy::NoProxy);
    QString headunitIP = qgetenv("HeadUnitIP");
    socket->connectToHost(headunitIP,m_port);
}

void VideoStreamClient::readyRead()
{
    data += socket->readAll();
    if(isValidJPEG(data)){
        processMessage(data);
        data.clear();
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
    if ( data.length() < 2){
        return false;
    }

    int totalBytes = data.length();
    const char *bytes = (const char*)data.constData();
    return (bytes[0] == (char)0xff &&
            bytes[1] == (char)0xd8 &&
            bytes[totalBytes-2] == (char)0xff &&
            bytes[totalBytes-1] == (char)0xd9);

}
