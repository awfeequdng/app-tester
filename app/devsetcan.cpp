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
    struct can_filter rfilter[4];      //  rfilter 记录总数
    rfilter[0].can_id = 0x41;           // 电阻板ID
    rfilter[0].can_mask = CAN_SFF_MASK;
    rfilter[1].can_id = 0x61;           // 绝缘板ID
    rfilter[1].can_mask = CAN_SFF_MASK;
    rfilter[2].can_id = 0x81;           // 匝间板ID
    rfilter[2].can_mask = CAN_SFF_MASK;
    rfilter[3].can_id = 0x481;           // 匝间板ID
    rfilter[3].can_mask = CAN_SFF_MASK;
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
    currAddr = 0;
    QList<int> ids;
    ids << 0x0022 << 0x0023 << 0x0024;
    for (int i=0; i < ids.size(); i++) {
        tmpMap.insert("index", ids.at(i));
        tmpMap.insert("data", QByteArray::fromHex("00"));
        sender.append(tmpMap);
        tmpMap.clear();
    }
    for (int i=0; i < ids.size(); i++) {
        tmpMap.insert("index", ids.at(i));
        tmpMap.insert("data", QByteArray::fromHex("08"));
        sender.append(tmpMap);
        tmpMap.clear();
    }
}

void DevSetCan::getAllDat()
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

void DevSetCan::putAllDat()
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

void DevSetCan::updateAll()
{
    timeOut++;
    for (int i=0; i < 100; i++) {
        if (recver.isEmpty())
            break;
        tmpMap = recver.dequeue();
        int id = tmpMap.value("index").toInt();
        QByteArray msg = tmpMap.value("data").toByteArray();
        tmpMap.clear();
        if (msg.size() < 2)
            continue;
        switch (id) {
        case 0x0061:
            updateAcw(msg);
            break;
        case 0x0081:
        case 0x0481:
            updateImp(id, msg);
            break;
        default:
            break;
        }
    }
}

void DevSetCan::sampleImp()
{
    QByteArray msg = QByteArray::fromHex("0101130001");  // 匝间采样
    tmpMap.insert("index", 0x0024);
    tmpMap.insert("data", msg);
    sender.append(tmpMap);
    tmpMap.clear();
}

void DevSetCan::configImp()
{
    QByteArray msg = QByteArray::fromHex("0300010201F41107");
    int v = config[QString::number(AddrSetIMP + AddrSkewIV)].toInt();  // 电压
    int c = config[QString::number(AddrSetIMP + AddrSkewIT)].toInt();  // 次数
    msg[4] = v/256;
    msg[5] = v%256;
    msg[6] = ((v/1000+1) << 4) + c;  // 档位+次数
    tmpMap.insert("index", 0x0024);
    tmpMap.insert("data", msg);
    sender.append(tmpMap);
}

void DevSetCan::selectImp()
{
    QByteArray msg = QByteArray::fromHex("0100130001");  // 匝间采样
    tmpMap.insert("index", 0x0024);
    tmpMap.insert("data", msg);
    sender.append(tmpMap);
    tmpMap.clear();
    config[QString::number(currAddr + AddrSkewHS)] = 1;
}

void DevSetCan::updateImp(int id, QByteArray msg)
{
    if (id == 0x0081) {
        quint8 cmd = quint8(msg.at(0));
        quint8 num = quint8(msg.at(1));
        if (cmd == 0x00) {
            config[QString::number(AddrSetIMP + AddrSkewIS)] = num;
            updateDat();
        }
        if (cmd == 0x02) {
            int v = quint8(msg.at(4))*256 + (quint8)msg.at(5);
            config[QString::number(AddrSetIMP + AddrSkewIU)] = v;
            config[QString::number(AddrSetIMP + AddrSkewIG)] = quint8(msg.at(2));
            config[QString::number(AddrSetIMP + AddrSkewIF)] = quint8(msg.at(3));
        }
        if (cmd == 0x03) {
            if (num == 0xFF) {
                for (int i=0; i < wave.size(); i++) {
                    config[QString::number(AddrWaveTP + i)] = wave.at(i);
                }
                wave.clear();
            }
        }
    }
    if (id == 0x0481) {
        for (int i=0; i < 4; i++) {
            wave.append(quint8(msg.at(i*2+0))*256 + quint8(msg.at(i*2+1)));
        }
    }
}

void DevSetCan::configAcw()
{
    int v = 0;
    int t = 0;
    QByteArray msg;
    for (int i=0; i < 4; i++) {
        if (i == 0) {
            msg = QByteArray::fromHex("0300040001000400");
        }
        if (i == 1) {
            msg = QByteArray::fromHex("0301050001000200");
        }
        if (i == 2) {
            msg = QByteArray::fromHex("0302050001000400");
        }
        if (i == 3) {
            msg = QByteArray::fromHex("0303050002000400");
        }
        tmpMap.insert("index", 0x0023);
        tmpMap.insert("data", msg);
        sender.append(tmpMap);

        if (i == 0) {
            msg = QByteArray::fromHex("040001F4000A0000");
        } else {
            msg = QByteArray::fromHex("040001F4000A0000");
        }
        v = config[QString::number(AddrHighAG + i*0x10 + AddrSkewHV)].toInt();
        t = config[QString::number(AddrHighAG + i*0x10 + AddrSkewHT)].toInt();
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

void DevSetCan::selectAcw()
{
    int index = 0;
    QByteArray msg;
    switch (currAddr) {
    case AddrHighAG:
        index = 0x0023;
        msg = QByteArray::fromHex("0104001100");  // 启动绝缘
        break;
    case AddrHighAC:
        index = 0x0023;
        msg = QByteArray::fromHex("0105001101");  // 启动交耐
        break;
    case AddrHighAL:
        index = 0x0023;
        msg = QByteArray::fromHex("0105001102");  // 启动交耐
        break;
    case AddrHighLC:
        index = 0x0023;
        msg = QByteArray::fromHex("0105001103");  // 启动交耐
        break;
    default:
        break;
    }
    timeOut = 0;
    tmpMap.insert("index", index);
    tmpMap.insert("data", msg);
    sender.append(tmpMap);
    tmpMap.clear();
    config[QString::number(currAddr + AddrSkewHS)] = 1;
}

void DevSetCan::updateAcw(QByteArray msg)
{
    quint8 cmd = quint8(msg.at(0));
    quint8 num = quint8(msg.at(1));
    if (cmd == 0x00) {
        config[QString::number(currAddr + AddrSkewHS)] = num;
        updateDat();
    }
    if (cmd == 0x01) {
        config[QString::number(currAddr + AddrSkewHU)] = quint8(msg.at(1))*256 + (quint8)msg.at(2);
        config[QString::number(currAddr + AddrSkewHR)] = quint8(msg.at(3))*256 + (quint8)msg.at(4);
        config[QString::number(currAddr + AddrSkewHD)] = quint8(msg.at(5));
        double r = quint8(msg.at(3))*256 + (quint8)msg.at(4);
        r *= qPow(10, -quint8(msg.at(5)));
        double h = config[QString::number(currAddr + AddrSkewHH)].toDouble();
        double l = config[QString::number(currAddr + AddrSkewHL)].toDouble();
        h = (currAddr == AddrHighAG) ? h : h/100;
        l = (currAddr == AddrHighAG) ? l : l/100;
        if (h == 0) {
            config[QString::number(currAddr + AddrSkewHJ)] = (r > l) ? 1 : 0;
        } else {
            config[QString::number(currAddr + AddrSkewHJ)] = (r > l && r < h) ? 1 : 0;
        }
        updateDat();
    }
}

void DevSetCan::updateDat()
{
    t.restart();
    for (int i=AddrConfig; i < AddrConfig+0x0100; i++) {
        tmpMap[QString::number(i)] = config[QString::number(i)];
    }
    for (int i=0; i < 400; i++) {
        tmpMap[QString::number(AddrWaveTP + i)] = config[QString::number(AddrWaveTP + i)];
    }
    qDebug() << "can copy:" << t.elapsed();
    t.restart();
    tmpMap.insert("enum", Qt::Key_News);
    tmpMap.insert("text", currAddr);
    emit sendAppMap(tmpMap);
    tmpMap.clear();
    qDebug() << "can time:" << t.elapsed();
}

void DevSetCan::recvAppMap(QVariantMap msg)
{
    switch (msg.value("enum").toInt()) {
    case Qt::Key_Copy:
        config = msg;
        break;
    case Qt::Key_Play:  // 启动测试
        currAddr = msg.value("text").toInt();
        if (currAddr >= AddrHighAG && currAddr <= AddrHighLC) {
            selectAcw();
        }
        if (currAddr == AddrSetIMP) {
            selectImp();
        }
        break;
    case Qt::Key_Send:  // 下发配置/采样
        currAddr = msg.value("text").toInt();
        if (currAddr == AddrConfig) {
            configAcw();
            configImp();
        }
        if (currAddr == AddrSetIMP) {
            sampleImp();
        }
        break;
    case Qt::Key_Plus:
        getAllDat();
        putAllDat();
        updateAll();
        break;
    default:
        break;
    }
}
