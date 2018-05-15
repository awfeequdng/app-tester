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
    timeOut = 0;
    currItem = 0;
    QList<int> ids;
    ids << 0x0022 << 0x0023 << 0x0024;
    for (int i=0; i < ids.size(); i++) {
        sendDevData(ids.at(i), QByteArray::fromHex("00"));
    }
    for (int i=0; i < ids.size(); i++) {
        sendDevData(ids.at(i), QByteArray::fromHex("08"));
    }
    sendDevData(0x0022, QByteArray::fromHex("05"));
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
}

void DevSetCan::updateDCR(QByteArray msg)
{
    quint8 cmd = quint8(msg.at(0));
    quint8 num = quint8(msg.at(1));
    if (cmd == 0x00) {
        tmpDat[DataDCRS] = num;  // 修改电阻板状态
        updateDat();
    }
    if (cmd == 0x01) {
        tmpDat[DataDCRS] = 0x01;  // 修改电阻板状态
        if (num == 1 && currItem == AddrDCRS1) {
            int s = tmpSet[AddrWeld].toInt();
            int n = quint8(msg.at(2));
            int g = quint8(msg.at(3));
            quint32 r = quint32(msg.at(4)) + quint32(msg.at(5)) * 0x0010 + quint32(msg.at(6))*0x1000;
            r = g > 3 ? r / 1000 : r;  // 换算为mΩ
            tmpDat[s + n] = r;
            int w = tmpSet.value(AddrDCRSW).toInt();
            int t = tmpSet[AddrDCRS1].toInt();
            int h = tmpSet[t + 1].toInt();
            double std = tmpSet[w + n - 1].toInt();
            if (abs(r-std)/std*1000 >= h) {
                tmpDat[DataDCRJ] = DataNG;
                tmpDat[DataOKNG] = DataNG;
            }
        }
        if (num == 2 && currItem == AddrDCRS2) {
            int s = tmpSet[AddrChip].toInt();
            int n = quint8(msg.at(2));
            quint32 r = quint32(msg.at(4)) + quint32(msg.at(5)) * 0x0010 + quint32(msg.at(6))*0x1000;
            tmpDat[s + n] = r;
        }
        if (num == 1 && currItem == AddrDCRS3) {
            int s = tmpSet[AddrDiag].toInt();
            int n = quint8(msg.at(2));
            quint32 r = quint32(msg.at(4)) + quint32(msg.at(5)) * 0x0010 + quint32(msg.at(6))*0x1000;
            tmpDat[s + n] = r;
        }
    }
    if (cmd == 0x05) {
        tmpMsg.insert(AddrEnum, Qt::Key_Shop);
        tmpMsg.insert(DataTemp, quint8(msg.at(1)) + quint8(msg.at(2))*256);
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
    }
}

void DevSetCan::sampleImp()
{
    sendDevData(0x0024, QByteArray::fromHex("0103110001"));  // 匝间采样
}

void DevSetCan::sampleDcr()
{
    currItem = AddrDCRS1;
    sendDevData(0x0022, QByteArray::fromHex("010103"));  // 电阻采样
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

void DevSetCan::updateImp(int id, QByteArray msg)
{
    t.restart();
    if (id == 0x0081) {
        quint8 cmd = quint8(msg.at(0));
        quint8 num = quint8(msg.at(1));
        if (cmd == 0x00) {
            tmpDat[DataIMPS] = num;
            tmpDat[AddrData] = 0;
            updateDat();
            qDebug() << "imp time:" << t.elapsed();
        }
        if (cmd == 0x02) {
            tmpDat[DataIMPG] = quint8(msg.at(2));  // 匝间档位
            tmpDat[DataIMPF] = quint8(msg.at(3));  // 匝间频率
            tmpDat[DataIMPU] = quint8(msg.at(4))*256 + (quint8)msg.at(5);  // 匝间电压
        }
        if (cmd == 0x03) {
            if (num == 0xFF) {
                int s = tmpSet[AddrIMPW].toInt();
                for (int i=0; i < wave.size(); i++) {
                    tmpDat[s + i] = wave.at(i);
                }
                calc();
                wave.clear();
                updateDat();
                qDebug() << "imp time:" << t.elapsed();
            } else {
                tmpDat[AddrData] = num;  // 波形编号
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
    int n = tmpDat[AddrData].toInt() - 1;
    int s = tmpSet[AddrIMPA].toInt() + n;  // 匝间结果地址
    int r = tmpSet[AddrIMPS1].toInt() + AddrIMPSH;  // 匝间上限地址
    int t = tmpSet[AddrIMPSW].toInt();  // 匝间波形地址
    double h = tmpSet[r].toDouble();  // 匝间上限
    quint32 Area1=0;
    quint32 Area2=0;
    quint32 Area3=0;
    quint32 D = 0;

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
    tmpDat[s] = D;

    if (D >= h*1000) {
        tmpDat[DataIMPJ] = DataNG;
        tmpDat[DataOKNG] = DataNG;
    }
}

void DevSetCan::configAcw()
{
    QByteArray msg;
    for (int i=0; i < 4; i++) {
        int r = tmpSet[AddrACWS1 + i].toInt();
        msg = QByteArray::fromHex("0300040001000400");
        msg[1] = i;
        msg[2] = (i == 0) ? 0x04 : 0x05;
        msg[4] = (i == 3) ? 0x02 : 0x01;
        msg[6] = (i == 1) ? 0x02 : 0x04;
        msg[7] = (i == 0) ? 0x00 : 0x32;
        sendDevData(0x0023, msg);

        msg = QByteArray::fromHex("040001F4000A0000");
        int v = tmpSet[r + AddrACWSV].toInt();
        int t = tmpSet[r + AddrACWST].toInt();
        msg[1] = i;
        msg[2] = v/256;
        msg[3] = v%256;
        msg[4] = t/256;
        msg[5] = t%256;
        sendDevData(0x0023, msg);

        msg = QByteArray::fromHex("0500000000030A");
        msg[1] = i;
        msg[2] = (i == 0) ? 0x00 : 0x09;
        msg[3] = (i == 0) ? 0x00 : 0xC4;
        sendDevData(0x0023, msg);
    }
}

void DevSetCan::updateAcw(QByteArray msg)
{
    t.restart();
    quint8 cmd = quint8(msg.at(0));
    quint8 num = quint8(msg.at(1));
    if (cmd == 0x00)
        tmpDat[DataACWS] = num;  // 修改高压板状态
    if (cmd == 0x01) {
        tmpDat[DataACWS] = 0x01;  // 修改高压板状态
        int t = (currItem%AddrACWS1);  // 第几个测试组
        int k = tmpSet[AddrACWS1 + t].toInt();  // 测试参数存储地址
        double h = tmpSet[k + AddrACWSH].toDouble();
        double l = tmpSet[k + AddrACWSL].toDouble();
        int s = tmpSet[AddrINRA + t].toInt();  // 测试结果存储地址
        double v = quint8(msg.at(1))*256 + (quint8)msg.at(2);  // 实测电压
        tmpDat[s + AddrACWU] = v;
        double r = quint8(msg.at(3))*256 + (quint8)msg.at(4);  // 实测结果
        tmpDat[s + AddrACWR] = r;
        double p = quint8(msg.at(5));  // 小数位数
        tmpDat[s + AddrACWP] = p;
        r *= qPow(10, -quint8(msg.at(5)));
        h = (currItem == AddrACWS1) ? h : h/100;  // 耐压电流倍数100
        l = (currItem == AddrACWS1) ? l : l/100;  // 耐压电流倍数100
        if (((h == 0) && (r > l)) || ((h != 0) && (r > l && r < h))) {
            tmpDat[s + AddrACWJ] = DataOK;
        } else {
            tmpDat[s + AddrACWJ] = DataNG;
            tmpDat[DataOKNG] = DataNG;
        }
    }
    if (cmd == 0x08) {
        tmpDat[DataACWV] = msg;
    }
    updateDat();
    qDebug() << "acw time:" << t.elapsed();
}

void DevSetCan::updateDat()
{
    tmpDat.insert(AddrEnum, Qt::Key_News);
    tmpDat.insert(AddrText, currItem);
    emit sendAppMsg(tmpDat);
}

void DevSetCan::startTest(QTmpMap map)
{
    int r = tmpSet[AddrModel].toInt();
    tmpDat.clear();
    currItem = map.value(AddrText).toInt() + AddrModel;
    QByteArray msg;
    switch (currItem) {
    case AddrDCRS1:
    case AddrDCRS2:
    case AddrDCRS3:
        msg = QByteArray::fromHex("040C02E8030101");
        msg[1] = tmpSet[r + 0].toInt();  // 电枢片数
        msg[2] = (currItem == AddrDCRS3) ? tmpSet[r + 0].toInt()/2 -1 : 0;  // 间隔片数
        sendDevData(0x0022, msg);
        msg = QByteArray::fromHex("010103");
        msg[1] = (currItem == AddrDCRS2) ? 0x02 : 0x01;
        sendDevData(0x0022, msg);
        tmpDat[DataDCRS] = 1;
        break;
    case AddrACWS1:
    case AddrACWS2:
    case AddrACWS3:
    case AddrACWS4:
        msg = QByteArray::fromHex("0104001100");  // 启动绝缘
        msg[1] = (currItem == AddrACWS1) ? 0x04 : 0x05;
        msg[4] = currItem - AddrACWS1;
        sendDevData(0x0023, msg);
        tmpDat[DataACWS] = 1;
        break;
    case AddrIMPS1:
        sendDevData(0x0024, QByteArray::fromHex("0100110001"));
        tmpDat[DataIMPS] = 1;
        break;
    default:
        break;
    }
    tmpDat[DataOKNG] = "0";
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
        timeOut++;
        if (timeOut % 500 == 0) {
            if (sender.isEmpty() && recver.isEmpty())
                sendDevData(0x0022, QByteArray::fromHex("05"));
        }
        break;
    case Qt::Key_Send:
        if (msg.value(AddrText).toInt() == AddrModel) {
            configDCR();
            configAcw();
            configImp();
        }
        if (msg.value(AddrText).toInt() == AddrDCRS1) {
            sampleDcr();
        }
        if (msg.value(AddrText).toInt() == AddrIMPS1) {
            sampleImp();
        }
        break;
    case Qt::Key_Play:
        startTest(msg);
        break;
    default:
        break;
    }
}
