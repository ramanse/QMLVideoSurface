#ifndef ICENTERTAINMENTCLIENT_H
#define ICENTERTAINMENTCLIENT_H

#include <QThread>
#include <QTcpSocket>

class ICEntertainmentClient: public QObject
{
    Q_OBJECT
public:
    explicit ICEntertainmentClient(QObject *parent = nullptr);
    ~ICEntertainmentClient();

    void setHostPort(int port);
    static ICEntertainmentClient *instance();
    void updateChunkSize(const qint64 &size);
    void writeData(const QByteArray &data );
signals:
    void error(QTcpSocket::SocketError socketError);
    void dataAvailable(QByteArray array);
public slots:
    void readyRead();
    void disconnected();
private:
    void test();
    void processMediaMessage(QByteArray message);
    QTcpSocket *socket;    
    QByteArray data;
    qint16 m_port;    
    static ICEntertainmentClient * m_Instance;
     qint64 m_chunkSize;
};

#endif // ICENTERTAINMENTCLIENT_H
