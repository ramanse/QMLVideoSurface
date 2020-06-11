#ifndef RadioStreamSocket_H
#define RadioStreamSocket_H

#include <QThread>
#include <QTcpSocket>

class RadioStreamSocket: public QObject
{
    Q_OBJECT
public:
    explicit RadioStreamSocket(QObject *parent = nullptr);
    ~RadioStreamSocket();
    void start();
    void setHostPort(int port);
    void resetSocketData();
signals:
    void error(QTcpSocket::SocketError socketError);
    void audioFrameAvailable(QByteArray array);
public slots:
    void readyRead();
    void disconnected();
private:
    void processAudioMessage(QByteArray message);
    QTcpSocket *socket;    
    QByteArray data;
    qint16 m_port;
    QString m_ipService;
    bool m_isReset;
    int m_msgLen;
};

#endif // RadioStreamSocket_H
