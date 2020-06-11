#pragma once

#include<QQuickItem>
#include <QQuickPaintedItem>
#include <QImage>
#include <QPointer>
#include <QPixmap>

#include "videostreamclient.h"

class QmlSimuVideoStream : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(bool streamVideo READ streamVideo WRITE setStreamVideo NOTIFY streamVideoChanged)        
    Q_PROPERTY(int hostPort READ hostPort WRITE setHostPort NOTIFY hostPortChanged)

public:
    QmlSimuVideoStream( QQuickItem *parent = nullptr );
    ~QmlSimuVideoStream();
    virtual void paint( QPainter* painter ) override;

signals:
     void streamVideoChanged();
     void hostPortChanged();

protected:
    bool streamVideo() const;
    void setStreamVideo( bool value );

    qint16 hostPort() const;
    void setHostPort( qint16 hostPort);

private slots:
    void processFrame(QByteArray frameByte);
    void resetHostPort();
private:
    void updateVariantStreamPort();
    bool m_streamVideo;
    qint16 m_port;
    VideoStreamClient *m_client;
    QPixmap m_framePixMap;

};
