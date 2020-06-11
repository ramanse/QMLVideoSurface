#pragma once
#include <QAbstractVideoSurface>
#include <QImage>
#include <QRect>
#include <QVideoFrame>
#include <QFile>
#include <QDir>
#include <QTcpServer>
#include <QTcpSocket>
#include "SimuSocket.h"


class PrototypingVideoSurface : public QAbstractVideoSurface
{



    Q_OBJECT

public:
    PrototypingVideoSurface(QObject *parent = 0);

    QList<QVideoFrame::PixelFormat> supportedPixelFormats(
            QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const override;
    bool isFormatSupported(const QVideoSurfaceFormat &format) const override;

    bool start(const QVideoSurfaceFormat &format) override;
    void stop() override;

    bool present(const QVideoFrame &frame) override;


    void startFrameServer(qint16 clientID);
    void setFrameCoverArtPath(QString filePath);


private slots:
    void newConnection();
    void removeConnection(SimuSocket *socket);

private:        
    constexpr static const int MAX_CONNECTIONS = 4;
    QImage::Format imageFormat;   
    QVideoFrame currentFrame;
    QTcpServer *server;
    QTcpSocket *m_socket;
    QString m_frameCoverArt;
    std::list<SimuSocket*> m_socketsHandler;

};

