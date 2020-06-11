#include "prototypingvideosurface.h"
#include <qabstractvideosurface.h>
#include <qvideosurfaceformat.h>
#include <QPainter>
#include <QTime>
#include <QDebug>
#include <QtMultimedia/private/qvideoframe_p.h>
#include <QBuffer>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>

PrototypingVideoSurface::PrototypingVideoSurface(QObject *parent)
    : QAbstractVideoSurface(parent)
    , imageFormat(QImage::Format_Invalid)

{
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
}

void PrototypingVideoSurface::startFrameServer(qint16 clientID){
    qint16 f_Port = 8787;
    if(!server->listen(QHostAddress::Any, f_Port))
    {
        qDebug() << "Server could not start!";
    }

}
void PrototypingVideoSurface::removeConnection(SimuSocket *socket)
{
    qCritical()<<"Video: Connection removed ";
    m_socketsHandler.remove(socket);
    qCritical()<<"Video: Connection(s) active " <<m_socketsHandler.size();

}

void PrototypingVideoSurface::setFrameCoverArtPath(QString filePath)
{
    m_frameCoverArt = filePath;
}
void PrototypingVideoSurface::newConnection()
{
    qCritical() << "Video: New connection requested";
    if(m_socketsHandler.size() <= MAX_CONNECTIONS){
        QTcpSocket *f_socket = server->nextPendingConnection();
        SimuSocket *listScoket = new SimuSocket(f_socket);
        m_socketsHandler.push_back(listScoket);
        connect(m_socketsHandler.back(), SIGNAL(lostConnection(SimuSocket*)), this, SLOT(removeConnection(SimuSocket*)));
    }
    else
        qCritical() << "Max connection reached, No more allowed";
}

QList<QVideoFrame::PixelFormat> PrototypingVideoSurface::supportedPixelFormats(
        QAbstractVideoBuffer::HandleType handleType) const
{
    if (handleType == QAbstractVideoBuffer::NoHandle) {
        return QList<QVideoFrame::PixelFormat>()
                << QVideoFrame::Format_RGB32
                << QVideoFrame::Format_ARGB32
                << QVideoFrame::Format_ARGB32_Premultiplied
                << QVideoFrame::Format_RGB565
                << QVideoFrame::Format_RGB555;
    } else {
        return QList<QVideoFrame::PixelFormat>();
    }
}

bool PrototypingVideoSurface::isFormatSupported(const QVideoSurfaceFormat &format) const
{
    const QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat());
    const QSize size = format.frameSize();

    return imageFormat != QImage::Format_Invalid
            && !size.isEmpty()
            && format.handleType() == QAbstractVideoBuffer::NoHandle;
}
bool PrototypingVideoSurface::start(const QVideoSurfaceFormat &format)
{
    const QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat());
    const QSize size = format.frameSize();

    if (imageFormat != QImage::Format_Invalid && !size.isEmpty()) {
        this->imageFormat = imageFormat;
        QAbstractVideoSurface::start(format);

        return true;
    } else {
        return false;
    }
}
void PrototypingVideoSurface::stop()
{
    currentFrame = QVideoFrame();
    QAbstractVideoSurface::stop();
}

bool PrototypingVideoSurface::present(const QVideoFrame &frame)
{
    if (surfaceFormat().pixelFormat() != frame.pixelFormat()
            || surfaceFormat().frameSize() != frame.size()) {
        setError(IncorrectFormatError);
        stop();

        return false;
    } else {
        currentFrame = frame;
        QImage imageItem= qt_imageFromVideoFrame(currentFrame);
        QByteArray ba;
        QBuffer buffer(&ba);
        buffer.open(QIODevice::WriteOnly);
        imageItem.save(&buffer, "JPG");
        QFile imgFile(m_frameCoverArt);

        //We wont use this anymore as we use prefected covers
//        if(!imgFile.exists()){
//            imgFile.open(QIODevice::WriteOnly);
//            imageItem.save(&imgFile, "JPG");
//        }
        for(SimuSocket *client:m_socketsHandler){
            client->write(ba);
        }
        imgFile.close();

        return true;
    }
}


