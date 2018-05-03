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

#include "qjson/QJsonArray.h"
#include "qjson/QJsonObject.h"
#include "qjson/QJsonDocument.h"

#define TIME_OUT 1000

class UdpSocket : public QUdpSocket
{
    Q_OBJECT
public:
    explicit UdpSocket(QObject *parent = 0);
signals:
    void sendAppMap(QVariantMap msg);
private slots:
    void initSocket();
    void readReady();
    void saveAll();
    void writeBytes(QVariantMap msg);
    void recvUdpMap(QVariantMap msg);
    void recvAppMap(QVariantMap msg);
private:
    quint16 hostPort;
    QHostAddress hostAddr;
    quint16 recvPort;
    QHostAddress recvAddr;
    QByteArray tmpByte;
    QVariantMap tmpMap;
    QVariantMap config;
    QQueue<QByteArray> sender;
    QQueue<QByteArray> recver;
};

#endif // UDPSOCKET_H
