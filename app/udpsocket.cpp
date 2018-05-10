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
    hostAddr = "192.168.1.179";
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
    connect(timer, SIGNAL(timeout()), this, SLOT(recvAll()));
    timer->start(10);
}

void UdpSocket::readReady()
{
    while (this->hasPendingDatagrams()) {
        tmpByte.resize(this->pendingDatagramSize());
        this->readDatagram(tmpByte.data(), tmpByte.size(), &recvAddr, &recvPort);
        recver.append(tmpByte);
        tmpByte.clear();
    }
}

void UdpSocket::recvAll()
{
    while (!recver.isEmpty()) {
        recvUdpXml(recver.dequeue());
    }
}

void UdpSocket::sendAll()
{
    while (!sender.isEmpty()) {
        QByteArray msg = sender.dequeue();
//        qDebug() << "udp send:" << msg;
        this->writeDatagram(msg, hostAddr, hostPort);
        this->waitForBytesWritten();
    }
}

void UdpSocket::recvUdpXml(QByteArray dat)
{
    QDomDocument docs;
    if (docs.setContent(dat)) {
        QDomNode root = docs.firstChild();
        QDomNode node = root.firstChild();
        while (!node.isNull()) {
            tmpMap[node.nodeName().remove(0, 1)] = node.toElement().text();
            node = node.nextSibling();
        }
    }
//    qDebug() << "udp recv:" << dat;
    if (tmpMap.value("enum").toInt() == Qt::Key_Game) {
        hostAddr = recvAddr;
        hostPort = recvPort;
    }
    emit sendAppMap(tmpMap);
    tmpMap.clear();
}

void UdpSocket::sendUdpXml(QVariantMap dat)
{
    QStringList keys = dat.keys();
    QDomDocument doc;
    QDomElement root;
    root = doc.createElement("xml");
    doc.appendChild(root);
    for (int t=0; t < keys.size(); t++) {
        if (!dat.value(keys.at(t)).toString().isEmpty()) {
            QDomElement element = doc.createElement("_" + keys.at(t));
            QDomText text = doc.createTextNode(dat.value(keys.at(t)).toString());
            element.appendChild(text);
            root.appendChild(element);
        }
    }
    QByteArray msg = doc.toByteArray();
    sender.append(msg);
}

void UdpSocket::recvAppMap(QVariantMap msg)
{
    switch (msg.value("enum").toInt()) {
    case Qt::Key_Copy:
        config = msg;
        break;
    case Qt::Key_Plus:
        recvAll();
        sendAll();
        break;
    case Qt::Key_News:
        sendUdpXml(msg);
        break;
    default:
#ifndef __arm__
        sendUdpXml(msg);
#endif
        break;
    }
}
