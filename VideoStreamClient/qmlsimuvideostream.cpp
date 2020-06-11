#include "qmlsimuvideostream.h"
#include <QImage>
#include <QBuffer>
#include <QDebug>
#include <QPainter>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>

QmlSimuVideoStream::QmlSimuVideoStream( QQuickItem *parent )
    : QQuickPaintedItem( parent )
{    
    m_client = new VideoStreamClient;
    connect(m_client, SIGNAL(frameAvailable(QByteArray)), this, SLOT(processFrame(QByteArray)));
    updateVariantStreamPort();
}

QmlSimuVideoStream::~QmlSimuVideoStream()
{
    qCritical()<<"Deleting the stream";
    delete m_client;
}
void QmlSimuVideoStream::setStreamVideo( bool value )
{
    m_streamVideo = value;
    emit streamVideoChanged();
}

qint16 QmlSimuVideoStream::hostPort() const
{
    return m_port;
}

void QmlSimuVideoStream::updateVariantStreamPort(){

    m_port = 8787;
    resetHostPort();
    emit hostPortChanged();

}

void QmlSimuVideoStream::setHostPort(qint16 hostPort)
{
    Q_UNUSED(hostPort);
    //updated internally by getVariantStreamPort
}

void QmlSimuVideoStream::resetHostPort(){

    m_client->setHostPort(m_port);
    m_client->start();

}
void QmlSimuVideoStream::processFrame(QByteArray frameByte)
{
    QImage img = QImage::fromData(frameByte);
    m_framePixMap = QPixmap::fromImage(img);
    update();
}

bool QmlSimuVideoStream::streamVideo() const
{
    return m_streamVideo;
}

void QmlSimuVideoStream::paint( QPainter *painter )
{
    if (painter != nullptr)
    {   QRectF source(0, 0, m_framePixMap.width(), m_framePixMap.height());
        QRectF target(boundingRect().x(),boundingRect().y(),boundingRect().width(), boundingRect().height());
        painter->drawPixmap(target, m_framePixMap, source);
    }
}

