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
    initDevStat();
}

void DevSetCan::initDevPort()
{
    fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);  //打开套接字
    if (fd < 0)
        perror("can0 open error");
    if (system("ifconfig can0 down") < 0)
        perror("can0 down error");
    if (system("ip link set can0 type can bitrate 500000 restart-ms 100") < 0)
        perror("can0 set error");
    if (system("ifconfig can0 up") < 0)
        perror("can0 up error");

    struct ifreq ifr;
    strcpy(ifr.ifr_name, "can0");
    if (ioctl(fd, SIOCGIFINDEX, &ifr) < 0)
        perror("can0 ioctl error");

    struct sockaddr_can addr;
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        perror("can0 bind error");

    struct can_filter rfilter[4];         //  rfilter 记录总数
    rfilter[0].can_id = 0x0041;           // 电阻板ID
    rfilter[0].can_mask = CAN_SFF_MASK;
    rfilter[1].can_id = 0x0061;           // 绝缘板ID
    rfilter[1].can_mask = CAN_SFF_MASK;
    rfilter[2].can_id = 0x0081;           // 匝间板ID
    rfilter[2].can_mask = CAN_SFF_MASK;
    rfilter[3].can_id = 0x0481;           // 匝间板ID
    rfilter[3].can_mask = CAN_SFF_MASK;
    if (setsockopt(fd, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter)) < 0)
        perror("can0 filter error");
}

void DevSetCan::initDevStat()
{
    currItem = 0;
    QList<int> ids;
    ids << 0x0022 << 0x0023 << 0x0024;
    for (int i=0; i < ids.size(); i++) {
        sendDevData(ids.at(i), QByteArray::fromHex("00"));
    }
    for (int i=0; i < ids.size(); i++) {
        sendDevData(ids.at(i), QByteArray::fromHex("08"));
    }
}

void DevSetCan::sendDevData(int id, QByteArray msg)
{
    tmpMsg.insert(AddrEnum, id);
    tmpMsg.insert(AddrText, msg);
    sender.append(tmpMsg);
    tmpMsg.clear();
}

void DevSetCan::readDevData(int id, QByteArray msg)
{
    tmpMsg.insert(AddrEnum, id);
    tmpMsg.insert(AddrText, msg);
    recver.append(tmpMsg);
    tmpMsg.clear();
    if (id != 0x481)
        qDebug() << "can recv:"
                 << QString("%1").arg(id, 4, 16, QChar('0'))
                 << msg.toHex().toUpper();
}

bool DevSetCan::readDevPort()
{
    struct timeval tv;
    tv.tv_sec  = 0;
    tv.tv_usec = 0;
    fd_set rdfds;
    FD_ZERO(&rdfds);
    FD_SET(fd, &rdfds);
    if (select(fd+1, &rdfds, NULL, NULL, &tv) >= 0) {  // 监听是否可读
        if (FD_ISSET(fd, &rdfds)) {
            if (read(fd, &frame, sizeof(struct can_frame)) > 0)   // 读取缓冲区
                return true;
        }
    }
    return false;
}

bool DevSetCan::sendDevPort(can_frame can)
{
    struct timeval tv;
    tv.tv_sec  = 0;
    tv.tv_usec = 0;
    fd_set rdfds;
    FD_ZERO(&rdfds);
    FD_SET(fd, &rdfds);
    if (select(fd+1, NULL, &rdfds, NULL, &tv) >= 0) {  // 监听是否可写
        if (FD_ISSET(fd, &rdfds)) {
            if (write(fd, &can, sizeof(struct can_frame)) == sizeof(struct can_frame))
                return true;
        }
    }
    return false;
}

void DevSetCan::getAllDat()
{
    while (readDevPort()) {
        QByteArray msg;
        for (int i=0; i < frame.can_dlc; i++) {
            msg.append(frame.data[i]);
        }
        readDevData(frame.can_id, msg);
    }
}

void DevSetCan::putAllDat()
{
    while (!sender.isEmpty()) {
        tmpMsg = sender.dequeue();
        frame.can_id = tmpMsg[AddrEnum].toInt();
        QByteArray msg = tmpMsg[AddrText].toByteArray();
        frame.can_dlc = msg.size();
        for (int i=0; i < frame.can_dlc; i++) {
            frame.data[i] = msg.at(i);
        }
        sendDevPort(frame);
        tmpMsg.clear();
        qDebug() << "can send:"
                 << QString("%1").arg(frame.can_id, 4, 16, QChar('0'))
                 << msg.toHex().toUpper();
    }
}

void DevSetCan::updateAll()
{
    for (int i=0; i < 100; i++) {
        if (recver.isEmpty())
            break;
        tmpMsg = recver.dequeue();
        int id = tmpMsg[AddrEnum].toInt();
        QByteArray msg = tmpMsg[AddrText].toByteArray();
        tmpMsg.clear();
        if (msg.size() < 2)
            continue;
        switch (id) {
        case 0x0041:
            updateDCR(msg);
            break;
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

void DevSetCan::configDCR()
{
    int r = tmpSet[AddrModel].toInt();
    QByteArray msg;
    msg = QByteArray::fromHex("0301010C");
    msg[3] = tmpSet[r + 0].toInt();
    sendDevData(0x0022, msg);
    msg = QByteArray::fromHex("040C02E8030101");
    msg[1] = tmpSet[r + 0].toInt();
    sendDevData(0x0022, msg);
}

void DevSetCan::selectDCR()
{
    QByteArray msg = QByteArray::fromHex("010103");
    if (currItem == AddrDCRS2)
        msg[1] = 0x02;
    sendDevData(0x0022, msg);
}

void DevSetCan::updateDCR(QByteArray msg)
{
    quint8 cmd = quint8(msg.at(0));
    quint8 num = quint8(msg.at(1));
    if (cmd == 0x00) {
        tmpSet[AddrDCRS] = num;  // 修改电阻板状态
        updateDat();
    }
    if (cmd == 0x01) {
        if (num == 1) {
            int s = tmpSet[AddrWeld].toInt();
            int n = quint8(msg.at(2));
            quint32 r = quint32(msg.at(4)) + quint32(msg.at(5)) * 0x0010 + quint32(msg.at(6))*0x1000;
            tmpSet[s + n] = r;
        }
    }
}

void DevSetCan::sampleImp()
{
    sendDevData(0x0024, QByteArray::fromHex("0103110001"));  // 匝间采样
}

void DevSetCan::configImp()
{
    int s = tmpSet[AddrModel].toInt();
    QByteArray msg = QByteArray::fromHex("03000C0101F41107");
    int r = tmpSet[AddrIMPS1].toInt();
    int v = tmpSet[r + AddrIMPSV].toInt();  // 电压
    int c = tmpSet[r + AddrIMPST].toInt();  // 次数
    msg[2] = tmpSet[s].toInt();
    msg[4] = v/256;
    msg[5] = v%256;
    msg[6] = ((v/1000+1) << 4) + c;  // 档位+次数
    sendDevData(0x0024, msg);
}

void DevSetCan::selectImp()
{
    QByteArray msg = QByteArray::fromHex("0100110001");  // 匝间采样
    sendDevData(0x0024, msg);
    tmpSet[AddrIMPS] = 1;
}

void DevSetCan::updateImp(int id, QByteArray msg)
{
    t.restart();
    if (id == 0x0081) {
        quint8 cmd = quint8(msg.at(0));
        quint8 num = quint8(msg.at(1));
        if (cmd == 0x00) {
            tmpSet[AddrIMPS] = num;
            tmpSet[AddrData] = 0;
            updateDat();
            qDebug() << "imp time:" << t.elapsed();
        }
        if (cmd == 0x02) {
            tmpSet[AddrIMPG] = quint8(msg.at(2));  // 匝间档位
            tmpSet[AddrIMPF] = quint8(msg.at(3));  // 匝间频率
            tmpSet[AddrIMPU] = quint8(msg.at(4))*256 + (quint8)msg.at(5);  // 匝间电压
        }
        if (cmd == 0x03) {
            if (num == 0xFF) {
                int s = tmpSet[AddrIMPW].toInt();
                for (int i=0; i < wave.size(); i++) {
                    tmpSet[s + i] = wave.at(i);
                }
                calc();
                wave.clear();
                updateDat();
                qDebug() << "imp time:" << t.elapsed();
            } else {
                tmpSet[AddrData] = num;  // 波形编号
            }
        }
    }
    if (id == 0x0481) {
        for (int i=0; i < 4; i++) {
            wave.append(quint8(msg.at(i*2+0))*256 + quint8(msg.at(i*2+1)));
        }
    }

}

void DevSetCan::calc()
{
    int s = tmpSet[AddrIMPA].toInt() + tmpSet[AddrData].toInt() - 1;
    quint32 Area1=0;
    quint32 Area2=0;
    quint32 Area3=0;
    quint32 D = 0;
    int t = tmpSet[AddrIMPSW].toInt();
    for (int i=1; i<wave.size()-1; i++) {
        int a1 = tmpSet[t + i].toInt();
        int a2 = wave.at(i);
        Area1 += abs(a1-0x200);
        Area2 += abs(a2-0x200);
        int b1 = tmpSet[t + i - 1].toInt();
        int b2 = tmpSet[t + i].toInt();
        int b3 = tmpSet[t + i + 1].toInt();
        int c1 = wave.at(i - 1);
        int c2 = wave.at(i);
        int c3 = wave.at(i + 1);
        Area3 += abs((b1+b2*2+b3)-(c1+c2*2+c3));
    }
    D = qMin(Area2,Area3/4)*100*1000/Area1;
    D = qMin(D, quint32(99990));
    tmpSet[s] = D;

    int r = tmpSet[AddrIMPS1].toInt() + AddrIMPSH;
    double h = tmpSet[r].toDouble();
    if (D < h*1000) {
        tmpSet[AddrIMPJ] = "1";
    }
    qDebug() << s << D;
}

void DevSetCan::configAcw()
{
    int v = 0;
    int t = 0;
    QByteArray msg;
    for (int i=0; i < 4; i++) {
        int r = tmpSet[AddrACWS1 + i].toInt();
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
        sendDevData(0x0023, msg);

        if (i == 0) {
            msg = QByteArray::fromHex("040001F4000A0000");
        } else {
            msg = QByteArray::fromHex("040001F4000A0000");
        }
        v = tmpSet[r + AddrACWSV].toInt();
        t = tmpSet[r + AddrACWST].toInt();
        msg[1] = i;
        msg[2] = v/256;
        msg[3] = v%256;
        msg[4] = t/256;
        msg[5] = t%256;
        sendDevData(0x0023, msg);

        if (i == 0) {
            msg = QByteArray::fromHex("0500000000030A");
        } else {
            msg = QByteArray::fromHex("050009C400030A");
        }
        msg[1] = i;
        sendDevData(0x0023, msg);
    }
}

void DevSetCan::selectAcw()
{
    QByteArray msg;
    switch (currItem) {
    case AddrACWS1:
        msg = QByteArray::fromHex("0104001100");  // 启动绝缘
        break;
    case AddrACWS2:
        msg = QByteArray::fromHex("0105011101");  // 启动交耐
        break;
    case AddrACWS3:
        msg = QByteArray::fromHex("0105011102");  // 启动交耐
        break;
    case AddrACWS4:
        msg = QByteArray::fromHex("0105011103");  // 启动交耐
        break;
    default:
        break;
    }
    sendDevData(0x0023, msg);
    tmpSet[AddrACWS] = 1;
}

void DevSetCan::updateAcw(QByteArray msg)
{
    t.restart();
    quint8 cmd = quint8(msg.at(0));
    quint8 num = quint8(msg.at(1));
    if (cmd == 0x00) {
        tmpSet[AddrACWS] = num;  // 修改高压板状态
    }
    if (cmd == 0x01) {
        int t = (currItem%AddrACWS1);  // 第几个测试组
        int k = tmpSet[AddrACWS1 + t].toInt();  // 测试参数存储地址
        double h = tmpSet[k + AddrACWSH].toDouble();
        double l = tmpSet[k + AddrACWSL].toDouble();
        int s = tmpSet[AddrINRA + t].toInt();  // 测试结果存储地址
        double v = quint8(msg.at(1))*256 + (quint8)msg.at(2);  // 实测电压
        tmpSet[s + AddrACWU] = v;
        double r = quint8(msg.at(3))*256 + (quint8)msg.at(4);  // 实测结果
        tmpSet[s + AddrACWR] = r;
        double p = quint8(msg.at(5));  // 小数位数
        tmpSet[s + AddrACWP] = p;
        r *= qPow(10, -quint8(msg.at(5)));
        h = (currItem == AddrACWS1) ? h : h/100;  // 耐压电流倍数100
        l = (currItem == AddrACWS1) ? l : l/100;  // 耐压电流倍数100
        tmpSet[s + AddrACWJ] = (h == 0) ? ((r > l) ? 1 : 0) : ((r > l && r < h) ? 1 : 0);
    }
    updateDat();
    qDebug() << "acw time:" << t.elapsed();
}

void DevSetCan::updateDat()
{
    tmpSet.insert(AddrEnum, Qt::Key_News);
    tmpSet.insert(AddrText, currItem);
    emit sendAppMsg(tmpSet);
}

void DevSetCan::startPlay(QTmpMap msg)
{
    currItem = msg.value(AddrText).toInt() + AddrModel;
    switch (currItem) {
    case AddrDCRS1:
    case AddrDCRS2:
    case AddrDCRS3:
        selectDCR();
        break;
    case AddrACWS1:
    case AddrACWS2:
    case AddrACWS3:
    case AddrACWS4:
        selectAcw();
        break;
    case AddrIMPS1:
        selectImp();
        break;
    default:
        break;
    }
}

void DevSetCan::recvAppMsg(QTmpMap msg)
{
    int c = msg.value(AddrEnum).toInt();
    switch (c) {
    case Qt::Key_Copy:
        tmpSet = msg;  // 保存设置参数
        break;
    case Qt::Key_Plus:
        getAllDat();
        putAllDat();
        updateAll();
        break;
    case Qt::Key_Send:
        if (msg.value(AddrText).toInt() == AddrModel) {
            configDCR();
            configAcw();
            configImp();
        }
        if (msg.value(AddrText).toInt() == AddrIMPS1) {
            sampleImp();
        }
        break;
    case Qt::Key_Play:
        startPlay(msg);
        break;
    default:
        break;
    }
}
