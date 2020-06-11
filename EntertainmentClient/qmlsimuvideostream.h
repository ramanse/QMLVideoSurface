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
#pragma once

#include<QQuickItem>
#include <QQuickPaintedItem>
#include <QImage>
#include <QPointer>
#include <QPixmap>
#include "videostreamclient.h"

class  QmlSimuVideoStream : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(bool streamVideo READ streamVideo WRITE setStreamVideo NOTIFY streamVideoChanged)        
    Q_PROPERTY(int hostPort READ hostPort WRITE setHostPort NOTIFY hostPortChanged)

public:
    QmlSimuVideoStream( QQuickItem *parent = nullptr );
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
