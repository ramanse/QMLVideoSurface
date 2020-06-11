#include "radiostreamsocket.h"
#include <QNetworkProxy>
#include <QIODevice>

RadioStreamSocket::RadioStreamSocket(QObject *parent)
    : QObject(parent)
    , m_msgLen(-1)
{
    // Create socket
    m_isReset = false;
}

RadioStreamSocket::~RadioStreamSocket()
{
    disconnected();
    socket->disconnectFromHost();
    delete socket;
}

void RadioStreamSocket::setHostPort(int port){
    m_port = port;
}
void RadioStreamSocket::start()
{
    qCritical() << "Thread started";
    socket = new QTcpSocket();
    if(socket){
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    m_ipService = "127.0.0.1";// qgetenv("HeadUnitIP");
    socket->setProxy(QNetworkProxy::NoProxy);
    socket->connectToHost(m_ipService,m_port);
    }

}

void RadioStreamSocket::readyRead()
{
    while (socket->bytesAvailable()) {
        if (socket->bytesAvailable()<sizeof(m_msgLen))
            return;
        if(m_msgLen < 0) {
            socket->read((char*)&m_msgLen,sizeof(m_msgLen));
        }
        if (socket->bytesAvailable()<m_msgLen)
            return;
        if(m_msgLen > 0) {
            data = socket->read(m_msgLen);
            processAudioMessage(data);
        }
        m_msgLen = -1;
    }
}

void RadioStreamSocket::disconnected()
{
    socket->readAll();
    socket->reset();
    socket->close();
}
void RadioStreamSocket::resetSocketData(){

    if(socket){
        m_isReset = true;
        socket->readAll();
        socket->reset();

    }

}

void RadioStreamSocket::processAudioMessage(QByteArray message)
{    
    emit audioFrameAvailable(message);
}
