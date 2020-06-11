#ifndef VIDEOSTREAMCLIENT_H
#define VIDEOSTREAMCLIENT_H

#include <QThread>
#include <QTcpSocket>

class VideoStreamClient: public QObject
{
    Q_OBJECT
public:
    explicit VideoStreamClient(QObject *parent = nullptr);
    ~VideoStreamClient();
    void start();
    void setHostPort(int port);
signals:
    void error(QTcpSocket::SocketError socketError);
    void frameAvailable(QByteArray array);
public slots:
    void readyRead();
    void disconnected();
private:
    void processMessage(QByteArray message);
    bool isValidJPEG(QByteArray array);
    QTcpSocket *socket;    
    QByteArray data;
    qint16 m_port;
    int m_chunkSize;
};

#endif // VIDEOSTREAMCLIENT_H
