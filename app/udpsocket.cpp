/*******************************************************************************
 * Copyright [2017~2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       UDP处理模块
*******************************************************************************/
#include "udpsocket.h"

UdpSocket::UdpSocket(QObject *parent) : QUdpSocket(parent)
{
    hostAddr = "192.168.1.53";
    hostPort = 6000;
    initSocket();
}

void UdpSocket::initSocket()
{
    if (this->state() == QUdpSocket::BoundState)
        return;
#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    this->bind(QHostAddress::Any, 6000);
#else
    this->bind(QHostAddress::AnyIPv4, 6000);
#endif
    connect(this, SIGNAL(readyRead()), this, SLOT(readReady()));
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(saveAll()));
    timer->start(10);
}

void UdpSocket::readReady()
{
    while (this->hasPendingDatagrams()) {
        tmpByte.resize(this->pendingDatagramSize());
        this->readDatagram(tmpByte.data(), tmpByte.size(), &recvAddr, &recvPort);
        //        qDebug() << "udp recv:" << tmpByte;
        recver.append(tmpByte);
        tmpByte.clear();
    }
}

void UdpSocket::saveAll()
{
    if (recver.isEmpty())
        return;
    for (int i=0; i < 10; i++) {
        tmpMap = QJsonDocument::fromJson(recver.dequeue()).object().toVariantMap();
        QString show = QString("%1").arg(tmpMap.value("enum").toInt(), 8, 16, QChar('0')).toUpper();
        qDebug() << "udp recv:" << show;
        recvUdpMap(tmpMap);
        tmpMap.clear();
        if (recver.isEmpty())
            break;
    }
}

void UdpSocket::writeBytes(QVariantMap msg)
{
    QByteArray dat = QJsonDocument::fromVariant(msg).toJson();
    this->writeDatagram(dat, hostAddr, hostPort);
    QString show = QString("%1").arg(msg.value("enum").toInt(), 8, 16, QChar('0')).toUpper();
    qDebug() << "udp send:" << show;
    this->waitForBytesWritten();
}

void UdpSocket::recvUdpMap(QVariantMap msg)
{
    switch (msg.value("enum").toInt()) {
    case Qt::Key_Option:
        config = msg;
        break;
    case Qt::Key_Enter:
        hostAddr = recvAddr;
        hostPort = recvPort;
        break;
    case Qt::Key_Play:
        break;
    default:
        break;
    }
    emit sendAppMap(msg);
}

void UdpSocket::recvAppMap(QVariantMap msg)
{
#ifndef __arm__
    emit writeBytes(msg);
#else
    switch (msg.value("enum").toInt()) {
    case Qt::Key_Option:
        config = msg;
        break;
    case Qt::Key_Refresh:
        emit writeBytes(msg);
        break;
    default:
        break;
    }
#endif
}
