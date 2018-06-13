/*******************************************************************************
 * Copyright [2017~2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       UDP处理模块
*******************************************************************************/
#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include <QQueue>
#include <QTimer>
#include <QObject>
#include <QUdpSocket>
#include <QMessageBox>
#include <QtXml/QDomDocument>

#include "main.h"

class UdpSocket : public QUdpSocket
{
    Q_OBJECT
public:
    explicit UdpSocket(QObject *parent = 0);
signals:
    void sendAppMsg(QTmpMap msg);
private slots:
    void initSocket();
    void readReady();
    void recvAll();
    void sendAll();
    void recvUdpXml(QByteArray dat);
    void sendUdpXml(QTmpMap msg);
    void recvAppMsg(QTmpMap msg);
private:
    quint16 hostPort;
    QHostAddress hostAddr;
    quint16 recvPort;
    QHostAddress recvAddr;
    QByteArray tmpByte;
    QByteArray tmpData;
    QQueue<QByteArray> sender;
    QQueue<QByteArray> recver;

    QTmpMap tmpSet;
    QTmpMap tmpMsg;
    bool isOK;
};

#endif // UDPSOCKET_H
