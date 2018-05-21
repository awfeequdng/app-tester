#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QQueue>
#include <QTimer>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QtXml/QDomDocument>

#include "appdefine.h"

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = 0);
signals:
    void sendAppMsg(QTmpMap msg);
public slots:
    void initSocket();
protected:
    void incomingConnection(int handle);
private slots:
    void recvSocket();
    void sendSocket();
    void recvTcpXml();
    void sendUdpXml(QTmpMap dat);
    void recvAppMsg(QTmpMap dat);
    void reckOK();
    void recvError(QAbstractSocket::SocketError);
private:
    QTcpServer *server;
    QTcpSocket *socket;
    qint64 frameSize;
    QQueue<QByteArray> recver;
    QQueue<QByteArray> sender;
    QTmpMap tmpSet;
    QTmpMap tmpMsg;
    bool isOK;
};

#endif // TCPSERVER_H
