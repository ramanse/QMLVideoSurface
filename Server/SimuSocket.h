
#pragma once

#include<QObject>
#include <QTcpSocket>
#include <QQueue>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

class SimuSocket: public QObject{
    Q_OBJECT

public:
    SimuSocket (QTcpSocket * socket)
    {
        this->sock = socket;
        connect(this->sock , SIGNAL(readyRead()), this, SLOT (slotReadyRead()));
        connect(this->sock , SIGNAL(connected()), this, SLOT(slotConnected()));
        connect(this->sock , SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
    }
    void write(QByteArray buffData){
        //qCritical() << this->sock->state();
        if(this->sock->state() != QAbstractSocket::ConnectedState)
            return;

        int chuncksize = buffData.size();
        this->sock->write((const char *) &chuncksize, sizeof(int));

        qint64 cnt = this->sock->write(buffData);
        if(cnt != buffData.size())
            qCritical() << "ERROR while sending " << cnt << " <> " << buffData.size();

    }

 signals:
    void dataReady(QTcpSocket *sckt);
    void newConnection(QTcpSocket *sckt);
    void lostConnection(SimuSocket *sckt);
public slots:
    void slotReadyRead()
    {
        emit dataReady(this->sock);
    }

    void slotConnected()
    {
        emit newConnection(this->sock);
    }

    void slotDisconnected()
    {
        emit lostConnection(this);
    }

public:
    QTcpSocket *sock;
};
