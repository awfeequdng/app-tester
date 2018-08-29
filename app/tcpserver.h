/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       TCP服务端
*******************************************************************************/
#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QQueue>
#include <QTimer>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QtXml/QDomDocument>

#include "main.h"

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = 0);
signals:
    void sendAppMsg(QTmpMap msg);
public slots:
    void initSocket(QString host);
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
