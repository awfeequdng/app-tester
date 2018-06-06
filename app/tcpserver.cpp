/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       TCP服务端
*******************************************************************************/
#include "tcpserver.h"

TcpServer::TcpServer(QObject *parent) : QTcpServer(parent)
{
    frameSize = 0;
    server = NULL;
    socket = NULL;
    isOK = false;

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(recvTcpXml()));
    connect(timer, SIGNAL(timeout()), this, SLOT(sendSocket()));
    timer->start(10);
}

void TcpServer::incomingConnection(int handle)
{
    isOK = true;
    socket = new QTcpSocket;
    socket->setSocketDescriptor(handle);
    connect(socket, SIGNAL(readyRead()), this, SLOT(recvSocket()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this,
            SLOT(recvError(QAbstractSocket::SocketError)));
    tmpMsg.insert(AddrEnum, Qt::Key_WLAN);
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
}

void TcpServer::initSocket()
{
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(recvSocket()));
    connect(socket, SIGNAL(connected()), this, SLOT(reckOK()));
    socket->connectToHost("192.168.1.50", 5999);
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this,
            SLOT(recvError(QAbstractSocket::SocketError)));
}

void TcpServer::recvSocket()
{
    QByteArray msg;
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_4_8);
    while (1) {
        if (frameSize == 0) {
            if (socket->bytesAvailable() < qint64(sizeof(qint64)))
                return;
            in >> frameSize;
        }
        if (socket->bytesAvailable() < frameSize)
            return;
        in >> msg;
        recver.append(msg);
        frameSize = 0;
    }
}

void TcpServer::sendSocket()
{
    if (sender.isEmpty())
        return;
    QByteArray dat = sender.dequeue();
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out << (qint64)0  << dat;
    out.device()->seek(0);
    out << (qint64)(msg.size()-sizeof(qint64));
    socket->write(msg);
}

void TcpServer::recvTcpXml()
{
    if (recver.isEmpty())
        return;
    QByteArray dat = recver.dequeue();
    QDomDocument docs;
    QString error;
    if (docs.setContent(dat, &error)) {
        QDomNode root = docs.firstChild();
        QDomNode node = root.firstChild();
        while (!node.isNull()) {
            tmpMsg[node.nodeName().remove(0, 1).toInt(0, 16)] = node.toElement().text();
            node = node.nextSibling();
        }
    } else {
        qDebug() << "tcp recv:" << error;
    }
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
}

void TcpServer::sendUdpXml(QTmpMap dat)
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

void TcpServer::recvAppMsg(QTmpMap dat)
{
    if (isOK) {
        QList<int> keys = dat.keys();
        QDomDocument doc;
        QDomElement root;
        root = doc.createElement("xml");
        doc.appendChild(root);
        for (int t=0; t < keys.size(); t++) {
            if (!dat.value(keys.at(t)).toString().isEmpty()) {
                QString name = QString("_%1").arg(keys.at(t), 4, 16, QChar('0'));
                QDomElement element = doc.createElement(name);
                QDomText text = doc.createTextNode(dat.value(keys.at(t)).toString());
                element.appendChild(text);
                root.appendChild(element);
            }
        }
        QByteArray msg = doc.toByteArray();
        sender.append(msg);
    }
}

void TcpServer::reckOK()
{
    isOK = true;
    tmpMsg.insert(AddrEnum, Qt::Key_WLAN);
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
}

void TcpServer::recvError(QAbstractSocket::SocketError)
{
    qDebug() << socket->errorString();
}

