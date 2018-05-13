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
    hostAddr = "192.168.1.50";
    hostPort = 6000;
    initSocket();
}

void UdpSocket::initSocket()
{
    if (this->state() == QUdpSocket::BoundState)
        return;
#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    this->bind(QHostAddress::Any, 6000);
    isOK = false;
#else
    this->bind(QHostAddress::AnyIPv4, 6000);
    isOK = true;
#endif
    connect(this, SIGNAL(readyRead()), this, SLOT(readReady()));
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(recvAll()));
    connect(timer, SIGNAL(timeout()), this, SLOT(sendAll()));
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
        this->writeDatagram(msg, hostAddr, hostPort);
        this->waitForBytesWritten();
//        qDebug() << "udp send:" << msg;
    }
}

void UdpSocket::recvUdpXml(QByteArray dat)
{
    QDomDocument docs;
    if (docs.setContent(dat)) {
        QDomNode root = docs.firstChild();
        QDomNode node = root.firstChild();
        while (!node.isNull()) {
            tmpMsg[node.nodeName().remove(0, 1).toInt()] = node.toElement().text();
            node = node.nextSibling();
        }
    }
    if (tmpMsg.value(AddrEnum).toInt() == Qt::Key_Game) {
        hostAddr = recvAddr;
        hostPort = recvPort;
        isOK = true;
    }
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
}

void UdpSocket::sendUdpXml(QTmpMap dat)
{
    QList<int> keys = dat.keys();
    QDomDocument doc;
    QDomElement root;
    root = doc.createElement("xml");
    doc.appendChild(root);
    for (int t=0; t < keys.size(); t++) {
        if (!dat.value(keys.at(t)).toString().isEmpty()) {
            QDomElement element = doc.createElement(QString("_%1").arg(keys.at(t)));
            QDomText text = doc.createTextNode(dat.value(keys.at(t)).toString());
            element.appendChild(text);
            root.appendChild(element);
        }
    }
    QByteArray msg = doc.toByteArray();
    sender.append(msg);
}

void UdpSocket::recvAppMsg(QTmpMap dat)
{
    if (isOK) {
        QList<int> keys = dat.keys();
        QDomDocument doc;
        QDomElement root;
        root = doc.createElement("xml");
        doc.appendChild(root);
        for (int t=0; t < keys.size(); t++) {
            if (!dat.value(keys.at(t)).toString().isEmpty()) {
                QDomElement element = doc.createElement("_" + QString::number(keys.at(t)));
                QDomText text = doc.createTextNode(dat.value(keys.at(t)).toString());
                element.appendChild(text);
                root.appendChild(element);
            }
        }
        QByteArray msg = doc.toByteArray();
        sender.append(msg);
    }
}
