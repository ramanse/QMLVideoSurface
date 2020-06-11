/*
 * Daimler AG ("COMPANY") CONFIDENTIAL
 * Unpublished Copyright (c) 2014-2016 Daimler AG, All Rights Reserved.
 *
 * NOTICE:  All information contained herein is, and remains the property of COMPANY. The intellectual and
 * technical concepts contained herein are proprietary to COMPANY and may be covered by U.S. and Foreign
 * Patents, patents in process, and are protected by trade secret or copyright law. Dissemination of this
 * information or reproduction of this material is strictly forbidden unless prior written permission is obtained
 * from COMPANY.  Access to the source code contained herein is hereby forbidden to anyone except current
 * COMPANY employees, managers or contractors who have executed Confidentiality and Non-disclosure agreements
 * explicitly covering such access.
 *
 * The copyright notice above does not evidence any actual or intended publication or
 * disclosure  of  this source code, which includes information that is confidential and/or proprietary,
 * and is a trade secret, of  COMPANY. ANY REPRODUCTION, MODIFICATION, DISTRIBUTION, PUBLIC  PERFORMANCE,
 * OR PUBLIC DISPLAY OF OR THROUGH USE  OF THIS  SOURCE CODE  WITHOUT  THE EXPRESS WRITTEN CONSENT OF
 * COMPANY IS STRICTLY PROHIBITED, AND IN VIOLATION OF APPLICABLE LAWS AND INTERNATIONAL TREATIES.  THE
 * RECEIPT OR POSSESSION OF  THIS SOURCE CODE AND/OR RELATED INFORMATION DOES NOT CONVEY OR IMPLY ANY RIGHTS
 * TO REPRODUCE, DISCLOSE OR DISTRIBUTE ITS CONTENTS, OR TO MANUFACTURE, USE, OR SELL ANYTHING THAT IT  MAY
 * DESCRIBE, IN WHOLE OR IN PART.
*/
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

    QFile file;
    QString mediaFileDir = qgetenv("SimuMediaFiles");
    file.setFileName( mediaFileDir + QDir::separator()+"videostreamports.json" );
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString val = file.readAll();
    file.close();
    QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
    m_port = 8787;//d.object().value(QString("variantID" + qgetenv("CLIENT_ID"))).toInt();
    qCritical() <<" client id"<<d.object().keys();
    qCritical() << "port value is " << m_port;
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
    {
        QRectF source(0, 0, m_framePixMap.width(), m_framePixMap.height());
        QRectF target(boundingRect().x(),boundingRect().y(),boundingRect().width(), boundingRect().height());
        painter->drawPixmap(target, m_framePixMap, source);
    }
}

