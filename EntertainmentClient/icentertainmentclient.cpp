#include "icentertainmentclient.h"
#include <QNetworkProxy>
ICEntertainmentClient * ICEntertainmentClient::m_Instance=NULL;

ICEntertainmentClient::ICEntertainmentClient(QObject *parent)
    : QObject(parent),
      m_chunkSize(0)
{
    // Create socket
    //setHostPort(5757);
    test();
}

ICEntertainmentClient::~ICEntertainmentClient()
{
    socket->disconnectFromHost();
    delete socket;
}

void ICEntertainmentClient::setHostPort(int port){
    m_port = port;
}

ICEntertainmentClient *ICEntertainmentClient::instance()
{
    if (!m_Instance)
    {
        m_Instance=new ICEntertainmentClient;
        //m_Instance->start();

    }
    return m_Instance;
}

void ICEntertainmentClient::updateChunkSize(const qint64 &size)
{
    m_chunkSize = size;
    qCritical()<<"updating chunck size"<<m_chunkSize;
}

void ICEntertainmentClient::writeData(const QByteArray &data)
{
    if(socket){
         socket->write(data);
    }
}
void ICEntertainmentClient::test()
{
    socket = new QTcpSocket();
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    socket->setProxy(QNetworkProxy::NoProxy);
    QString headunitIP = qgetenv("HeadUnitIP");
    socket->connectToHost(headunitIP,1937);

}


void ICEntertainmentClient::readyRead()
{    
   data += socket->readAll();
    if(m_chunkSize == 0 || data.size() == m_chunkSize){
        emit dataAvailable(data);
        if(data.size() == m_chunkSize)
            m_chunkSize = 0;
        data.clear();
    }
}

void ICEntertainmentClient::disconnected()
{

}
