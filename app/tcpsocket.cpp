/*******************************************************************************
 * Copyright [2017]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:
*******************************************************************************/
#include "tcpsocket.h"

TcpSocket::TcpSocket(QObject *parent) : QTcpSocket(parent)
{
    heart = 0;
    LoadSize = 4*1024;;
    BlockSize = 0;
    recvByte = 0;
    recvFile = 0;
    sendFile = 0;
    sendByte = 0;
    file = NULL;

    connect(this, SIGNAL(readyRead()), this, SLOT(readSocket()));
    connect(this, SIGNAL(bytesWritten(qint64)), this, SLOT(sendFileData(qint64)));
    connect(this, SIGNAL(error(QAbstractSocket::SocketError)), this,
            SLOT(recvSocketErr(QAbstractSocket::SocketError)));
    connect(this, SIGNAL(connected()), this, SLOT(recvSocketConnected()));

    QDir dir;
    dir.mkdir(QString("temp"));
    dir.mkdir(QString("network"));

    proc = new QProcess(this);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(recvTcpXml()));
    connect(timer, SIGNAL(timeout()), this, SLOT(sendSocket()));
    timer->start(10);
}

void TcpSocket::connectToServer(QTmpMap msg)
{
    tmpSet = msg;
#ifdef __arm__
    this->connectToHost("s.aipuo.com", 6000);
#else
    this->connectToHost("192.168.1.55", 5999);
#endif
}

void TcpSocket::readSocket()
{
    quint16 addr;
    quint16 cmd;
    QByteArray msg;
    QDataStream in(this);
    in.setVersion(QDataStream::Qt_4_8);
    while (1) {
        if (BlockSize == 0) {
            if (this->bytesAvailable() < qint64(sizeof(qint64)))
                return;
            in >> BlockSize;
        }
        if (BlockSize > 8*1024) {
            qDebug() << "tcp recv block size overflow";
            this->abort();
            return;
        }
        if (this->bytesAvailable() < BlockSize)
            return;
        in >> addr >> cmd >> msg;
        recvSocketCmd(addr, cmd, msg);
        BlockSize = 0;
    }
}

void TcpSocket::recvFileHead(QByteArray msg)
{
    GetFileMD5 = msg.mid(0, 16);
    QString temp = msg.remove(0, 17);
    QStringList detail = temp.split(" ");
    recvFile = detail.at(0).toInt();
    fileName = detail.at(1).toUtf8();
    recvByte = 0;
    file = new QFile(QString("./temp/").append(fileName));
    if (!file->open(QFile::ReadWrite)) {
        sendSocket(ADDR, FILE_FAIL, file->errorString().toUtf8());
    }
}

void TcpSocket::recvFileData(QByteArray msg)
{
    if (file == NULL)
        return;
    if (!file->isOpen())
        return;

    int ret = file->write(msg);

    recvByte += ret;
    display(QString("服务器已接收%1%").arg(recvByte*100/recvFile).toUtf8());
    if (recvByte >= recvFile) {
        file->seek(0);
        QByteArray md5 = QCryptographicHash::hash(file->readAll(), QCryptographicHash::Md5);
        recvByte = 0;
        recvFile = 0;
        if (GetFileMD5 == md5) {
            QProcess::execute("sync");
            QString cmd = QString("mv %1 %2").arg(file->fileName()).arg("./network/");
            QProcess::execute(cmd);
            display("服务器接收成功");
            sendSocket(ADDR, FILE_OVER, "");
        } else {
            file->close();
            file->remove();
            display("服务器接收失败");
            sendSocket(ADDR, FILE_FAIL, "MD5 Error");
        }
    }
}

void TcpSocket::sendSocket(quint16 addr, quint16 cmd, QByteArray data)
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_8);
    out << (qint64)0 << quint16(addr) << quint16(cmd) << data;
    out.device()->seek(0);
    out << (qint64)(msg.size()-sizeof(qint64));
    this->write(msg);
}

void TcpSocket::sendFileHead(QByteArray data)
{
    QString name = data;
    file = new QFile(name);
    if (!file->open(QFile::ReadOnly)) {
        display(file->errorString().toUtf8());
        return;
    }
    QByteArray md5;
    md5 = QCryptographicHash::hash(file->readAll(), QCryptographicHash::Md5);
    file->seek(0);
    sendFile = file->size();
    sendByte = 0;
    PutFileName = name.right(name.size()- name.lastIndexOf('/')-1).toUtf8();
    QByteArray msg;
    msg.append(md5);
    msg.append(QString(" %1 ").arg(sendFile));
    msg.append(PutFileName);
    sendSocket(ADDR, FILE_HEAD, msg);
}

void TcpSocket::sendFileData(qint64)
{
    if (sendFile == 0)
        return;
    sendSocket(ADDR, FILE_DATA, file->read(LoadSize));
    sendByte += qMin(sendFile, LoadSize);
    sendFile -= qMin(sendFile, LoadSize);
    display(QString("服务器已发送%1%").arg(sendByte*100/(sendFile+sendByte)).toUtf8());
    heart = 0;
    if (sendFile == 0) {
        file->close();
    }
}

void TcpSocket::recvSocketCmd(quint16 addr, quint16 cmd, QByteArray msg)
{
    if (cmd != BEAT)
        qDebug() << "tcp recv:" << addr << cmd;
    heart = 0;
    switch (cmd) {
    case FILE_OVER:           // 发送成功
        display(tr("服务器发送成功"));
        break;
    case FILE_FAIL:           // 发送失败
        display(msg);
        break;
    case READ_HEAD:    // 客户获取文件
        sendFileHead(msg);
        break;
    case FILE_HEAD:         // 收取文件头
        recvFileHead(msg);
        break;
    case FILE_DATA:         // 获取文件内容
        recvFileData(msg);
        break;
    case SHELL_CMD:
        proc->start(msg);
        proc->waitForFinished();
        sendSocket(quint16(addr), SHELL_DAT, proc->readAll());
        break;
    case BEAT:        // 心跳
        break;
    case SEND_DATA:
        recver.append(msg);
        break;
    default:
        break;
    }
}

void TcpSocket::recvSocketErr(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::ConnectionRefusedError:
        break;
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        break;
    case QAbstractSocket::SocketAccessError:
        break;
    case QAbstractSocket::SocketResourceError:
        break;
    case QAbstractSocket::SocketTimeoutError:
        break;
    default:
        this->close();
        break;
    }
    display(this->errorString().toUtf8());
}

void TcpSocket::recvSocketConnected()
{
    display(tr("TCP连接成功"));
#ifdef __arm__
    int r = tmpSet.value(AddrDevA).toInt();
    QStringList dev;
    dev.append(tmpSet.value(r + 1).toString());
    dev.append(getHardwareAddress());
    dev.append(tmpSet.value(DataSoft).toString());
    QByteArray msg = dev.join(" ").toUtf8();
    this->sendSocket(ADDR, SIGN_IN, msg);
#endif
    heart = 0;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(sendSocketBeat()));
    timer->start(5000);
}

void TcpSocket::sendSocketBeat()
{
    if (this->state() != QAbstractSocket::ConnectedState) {
        connectToServer(tmpSet);
    } else {
        sendSocket(ADDR, BEAT, "");
        heart++;
        if (heart > 6) {
            this->abort();
        }
    }
}

QString TcpSocket::getHardwareAddress()
{
    int i;
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
    for (i=0; i < list.size(); i++) {
        if (list[i].hardwareAddress().size() == 17 &&
                list[i].hardwareAddress() != "00:00:00:00:00:00")
            return list[i].hardwareAddress();
    }
    return list[0].hardwareAddress();
}

void TcpSocket::display(QString msg)
{
    qDebug() << "tcp show:" << msg;
}

void TcpSocket::sendSocket()
{
    if (sender.isEmpty())
        return;
    QByteArray dat = sender.dequeue();
    sendSocket(txPort, SEND_DATA, dat);
}

void TcpSocket::recvTcpXml()
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
            tmpMsg[node.nodeName().remove(0, 1).toInt()] = node.toElement().text();
            node = node.nextSibling();
        }
    } else {
        qDebug() << "tcp recv:" << error;
    }
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
}

void TcpSocket::recvAppCmd(QTmpMap dat)
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

void TcpSocket::recvAppMsg(QTmpMap msg)
{
    int c = msg.value(AddrEnum).toInt();
    switch (c) {
    case Qt::Key_Zoom:
        isOK = true;
        txPort = msg.value(AddrPort).toInt();
        if (!msg.value(AddrData).isNull()) {
            this->sendSocket(txPort, SHELL_CMD, msg.value(AddrData).toByteArray());
        }
        if (!msg.value(AddrSend).isNull()) {
            this->sendSocket(txPort, SEND_HEAD, msg.value(AddrSend).toByteArray());
        }
        if (!msg.value(AddrRecv).isNull()) {
            this->sendSocket(txPort, READ_HEAD, msg.value(AddrRecv).toByteArray());
        }
        break;
    default:
        recvAppCmd(msg);
        break;
    }
}
