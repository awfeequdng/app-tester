/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       CAN总线设置
*******************************************************************************/
#include "devsetcan.h"

DevSetCan::DevSetCan(QObject *parent) : QObject(parent)
{
    initDevPort();
    getStatus();
    getVerNum();
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(thread()));
    timer->start(10);
}

void DevSetCan::initDevPort()
{
    int ret = 0;

    fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);  //打开套接字
    if (fd < 0) {
        QString tmp = tr("CAN口打开失败");
        qDebug() << "can show:" << tmp;
        return;
    }

    ret = system("ifconfig can0 down");
    if (ret < 0) {
        perror("can0 down error");
    }
    ret = system("ip link set can0 type can bitrate 500000 restart-ms 100");
    if (ret < 0) {
        perror("can0 set error");
    }
    ret = system("ifconfig can0 up");
    if (ret < 0) {
        perror("can0 up error");
    }

    struct ifreq ifr;
    strcpy(ifr.ifr_name, "can0");
    ret = ioctl(fd, SIOCGIFINDEX, &ifr);
    if (ret < 0) {
        QString tmp = tr("CAN口设置失败");
        qDebug() << "can show:" << tmp;
        return;
    }

    struct sockaddr_can addr;
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    ret = bind(fd, (struct sockaddr *)&addr, sizeof(addr));
    if (ret < 0) {
        QString tmp = tr("CAN口绑定失败");
        qDebug() << "can show:" << tmp;
        return;
    }
    struct can_filter rfilter[3];      //  rfilter 记录总数
    rfilter[0].can_id = 0x41;           // 电阻板ID
    rfilter[0].can_mask = CAN_SFF_MASK;
    rfilter[1].can_id = 0x61;           // 绝缘板ID
    rfilter[1].can_mask = CAN_SFF_MASK;
    rfilter[2].can_id = 0x81;           // 匝间板ID
    rfilter[2].can_mask = CAN_SFF_MASK;
    qDebug() << setsockopt(fd, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));
}

bool DevSetCan::readDevPort()
{
    if (fd <= 0)
        return false;
    int ret = 0;
    fd_set rdfds;
    struct timeval tv;

    FD_ZERO(&rdfds);
    FD_SET(fd, &rdfds);
    tv.tv_sec  = 0;                                         // 0秒
    tv.tv_usec = 0;                                         // 0微秒
    ret = select(fd+1, &rdfds, NULL, NULL, &tv);            // 监听是否可读
    if (ret < 0)
        return false;
    if (FD_ISSET(fd, &rdfds)) {
        int nbytes = read(fd, &frame, sizeof(struct can_frame));  // 读取缓冲区
        if (nbytes > 0) {
            return true;
        }
    }
    return false;
}

bool DevSetCan::sendDevPort(can_frame can)
{
    if (fd <= 0)
        return false;
    int nbytes;
    int ret;
    fd_set rdfds;
    struct timeval tv;

    FD_ZERO(&rdfds);
    FD_SET(fd, &rdfds);
    tv.tv_sec  = 0;                                         // 0秒
    tv.tv_usec = 500;                                       // 0微秒
    ret = select(fd+1, NULL, &rdfds, NULL, &tv);            // 监听是否可写
    if (ret < 0)
        return false;
    if (FD_ISSET(fd, &rdfds)) {
        nbytes = write(fd, &can, sizeof(struct can_frame));
        if (nbytes != sizeof(struct can_frame)) {
            perror("can raw socket write error");
            return false;
        }
        return true;
    }
    return false;
}

void DevSetCan::getStatus()
{
    QList<int> ids;
    ids << 0x0022 << 0x0023 << 0x0024;
    for (int i=0; i < ids.size(); i++) {
        tmpMap.insert("index", ids.at(i));
        tmpMap.insert("data", QByteArray::fromHex("00"));
        sender.append(tmpMap);
        tmpMap.clear();
    }
}

void DevSetCan::getVerNum()
{
    QList<int> ids;
    ids << 0x0022 << 0x0023 << 0x0024;
    for (int i=0; i < ids.size(); i++) {
        tmpMap.insert("index", ids.at(i));
        tmpMap.insert("data", QByteArray::fromHex("08"));
        sender.append(tmpMap);
        tmpMap.clear();
    }
}

void DevSetCan::setStatus(int addr)
{
    currAddr = addr;
    if (addr == AddrSC) {
        sendConfig();
        return;
    }
    sendThread();
}

void DevSetCan::readAll()
{
    while (readDevPort()) {
        QByteArray msg;
        for (int i=0; i < frame.can_dlc; i++) {
            msg.append(frame.data[i]);
        }
        tmpMap["index"] = frame.can_id;
        tmpMap["data"] = msg;
        recver.append(tmpMap);
        qDebug() << "can recv:"
                 << QString("%1").arg(frame.can_id, 4, 16, QChar('0'))
                 << msg.toHex().toUpper();
        tmpMap.clear();
    }
}

void DevSetCan::sendAll()
{
    while (!sender.isEmpty()) {
        tmpMap = sender.dequeue();
        QByteArray msg = tmpMap["data"].toByteArray();
        frame.can_id = tmpMap["index"].toInt();
        frame.can_dlc = msg.size();
        for (int i=0; i < msg.size(); i++) {
            frame.data[i] = msg.at(i);
        }
        sendDevPort(frame);
        qDebug() << "can send:"
                 << QString("%1").arg(frame.can_id, 4, 16, QChar('0'))
                 << msg.toHex().toUpper();
        tmpMap.clear();
    }
}

void DevSetCan::saveAll()
{
    if (recver.isEmpty())
        return;
    for (int i=0; i < 10; i++) {
        tmpMap = recver.dequeue();
        int id = tmpMap.value("index").toInt();
        QByteArray msg = tmpMap.value("data").toByteArray();
        tmpMap.clear();
        if (msg.size() < 2)
            continue;
        recvOne(id, msg);
        if (recver.isEmpty())
            break;
    }
}

void DevSetCan::thread()
{
    sendAll();
    readAll();
    saveAll();
}

void DevSetCan::recvOne(int id, QByteArray msg)
{
    quint8 cmd = 0;
    quint8 num = 0;
    if (id == 0x0061) {
        cmd = quint8(msg.at(0));
        num = quint8(msg.at(1));
        if (cmd == 0x00) {
            config[QString::number(currAddr + AddrHS)] = num;
        }
        if (cmd == 0x01) {
            config[QString::number(currAddr + AddrHU)] = quint8(msg.at(1))*256 + (quint8)msg.at(2);
            config[QString::number(currAddr + AddrHR)] = quint8(msg.at(3))*256 + (quint8)msg.at(4);
            config[QString::number(currAddr + AddrHD)] = quint8(msg.at(5));
            double r = quint8(msg.at(3))*256 + (quint8)msg.at(4);
            r *= qPow(10, -quint8(msg.at(5)));
            double h = config[QString::number(currAddr + AddrHH)].toDouble();
            double l = config[QString::number(currAddr + AddrHL)].toDouble();
            h = (currAddr == AddrAG) ? h : h/100;
            l = (currAddr == AddrAG) ? l : l/100;
            if (h == 0) {
                config[QString::number(currAddr + AddrHJ)] = (r > l) ? 1 : 0;
            } else {
                config[QString::number(currAddr + AddrHJ)] = (r > l && r < h) ? 1 : 0;
            }
        }
        sendUpdate();
    }
}

void DevSetCan::sendConfig()
{
    int v = 0;
    int t = 0;
    QByteArray msg;
    for (int i=0; i < 4; i++) {
        if (i == 0) {
            msg = QByteArray::fromHex("0300048000FFFF00");
        } else {
            msg = QByteArray::fromHex("0301058000FFFF32");
            msg[1] = i;
        }
        tmpMap.insert("index", 0x0023);
        tmpMap.insert("data", msg);
        sender.append(tmpMap);

        if (i == 0) {
            msg = QByteArray::fromHex("040001F4000A01F4");
        } else {
            msg = QByteArray::fromHex("040001F4000A0000");
        }
        v = config[QString::number(AddrAG + i*0x10 + AddrHV)].toInt();
        t = config[QString::number(AddrAG + i*0x10 + AddrHT)].toInt();
        msg[1] = i;
        msg[2] = v/256;
        msg[3] = v%256;
        msg[4] = t/256;
        msg[5] = t%256;
        tmpMap.insert("index", 0x0023);
        tmpMap.insert("data", msg);
        sender.append(tmpMap);

        if (i == 0) {
            msg = QByteArray::fromHex("0500000000030A");
        } else {
            msg = QByteArray::fromHex("050009C400030A");
        }
        msg[1] = i;
        tmpMap.insert("data", msg);
        sender.append(tmpMap);
        tmpMap.clear();
    }
}

void DevSetCan::sendThread()
{
    int index;
    QByteArray msg;
    switch (currAddr) {
    case AddrAG:
        index = 0x0023;
        msg = QByteArray::fromHex("0104001300");  // 启动绝缘
        break;
    case AddrAC:
        index = 0x0023;
        msg = QByteArray::fromHex("0105001301");  // 启动交耐
        break;
    default:
        break;
    }
    if (msg.isEmpty())
        return;

    tmpMap.insert("index", index);
    tmpMap.insert("data", msg);
    sender.append(tmpMap);
    tmpMap.clear();
    config[QString::number(currAddr + AddrHS)] = 1;
}

void DevSetCan::sendUpdate()
{
    for (int i=currAddr; i < currAddr+0x10; i++) {
        tmpMap[QString::number(i)] = config[QString::number(i)];
    }
    tmpMap.insert("enum", Qt::Key_Refresh);
    tmpMap.insert("text", currAddr);
    emit sendAppMap(tmpMap);
    tmpMap.clear();
}

void DevSetCan::recvAppMap(QVariantMap msg)
{
    switch (msg.value("enum").toInt()) {
    case Qt::Key_Option:
        config = msg;
        break;
    case Qt::Key_Play:
        setStatus(msg.value("text").toInt());
        break;
    default:
        break;
    }
}
