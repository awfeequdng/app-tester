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
    numb = 0;
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
    tmpio = 0;
    timeOut = 0;
    currItem = Qt::Key_0;
    currTemp = 0;
    sendDevData(CAN_ID_DCR, QByteArray::fromHex("08"));
    sendDevData(CAN_ID_ACW, QByteArray::fromHex("08"));
    sendDevData(CAN_ID_IMP, QByteArray::fromHex("08"));
    sendDevData(CAN_ID_DCR, QByteArray::fromHex("00"));
    sendDevData(CAN_ID_ACW, QByteArray::fromHex("00"));
    sendDevData(CAN_ID_IMP, QByteArray::fromHex("00"));
    sendDevData(CAN_ID_DCR, QByteArray::fromHex("05"));
    sendDevData(CAN_ID_DCR, QByteArray::fromHex("0A"));
}

void DevSetCan::sendDevData(int id, QByteArray msg)
{
    tmpMsg.insert(Qt::Key_0, id);
    tmpMsg.insert(Qt::Key_1, msg);
    sender.append(tmpMsg);
    tmpMsg.clear();
}

void DevSetCan::readDevData(int id, QByteArray msg)
{
    tmpMsg.insert(Qt::Key_0, id);
    tmpMsg.insert(Qt::Key_1, msg);
    recver.append(tmpMsg);
    tmpMsg.clear();
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
        timeOut = 0;
        tmpMsg = sender.dequeue();
        frame.can_id = tmpMsg[Qt::Key_0].toInt();
        QByteArray msg = tmpMsg[Qt::Key_1].toByteArray();
        frame.can_dlc = msg.size();
        for (int i=0; i < frame.can_dlc; i++) {
            frame.data[i] = msg.at(i);
        }
        sendDevPort(frame);
        tmpMsg.clear();
    }
}

void DevSetCan::updateAll()
{
    for (int i=0; i < 100; i++) {
        if (recver.isEmpty())
            break;
        timeOut = 0;
        tmpMsg = recver.dequeue();
        int id = tmpMsg[Qt::Key_0].toInt();
        QByteArray msg = tmpMsg[Qt::Key_1].toByteArray();
        tmpMsg.clear();
        switch (id) {
        case 0x0041:
            parseDCR(msg);
            break;
        case 0x0061:
            parseACW(msg);
            break;
        case 0x0081:
        case 0x0481:
            parseIMP(id, msg);
            break;
        default:
            break;
        }
    }
}

void DevSetCan::updateDat()
{
    tmpDat.insert(Qt::Key_0, Qt::Key_News);
    tmpDat.insert(Qt::Key_1, currItem);
    emit sendAppMsg(tmpDat);
}

void DevSetCan::setupDCR(QTmpMap map)
{
    volatile int back = tmpSet.value(1000 + Qt::Key_0).toInt();  // 后台配置地址
    volatile int conf = tmpSet.value(4000 + Qt::Key_0).toInt();  // 综合配置地址
    volatile int parm = tmpSet.value(4000 + Qt::Key_1).toInt();  // 片间配置地址
    volatile int quan = tmpSet.value(conf + AddrDCRSC).toInt();  // 电枢片数
    volatile int tool = tmpSet.value(conf + AddrDEVSC).toInt() * quan;  // 夹具针数
    volatile int time = tmpSet.value(parm + TIMEDCR1).toInt();  // 测试时间
    volatile int freq = tmpSet.value(parm + GEARDCR1).toInt();  // 档位
    volatile int work = tmpSet.value(back + 5).toInt(); // 工位数
    int spac = 0;  // 间隔片数
    switch (currItem) {
    case Qt::Key_1:
        freq = map.value(Qt::Key_5).isNull() ? freq : map.value(Qt::Key_5).toInt();
        break;
    case Qt::Key_3:
        spac = quan/2 -1;
        break;
    default:
        break;
    }
    QByteArray msg;
    // 03配置;01工位数;01板卡数;0C夹具数
    msg = QByteArray::fromHex("0301010C");
    msg[1] = work;
    msg[2] = ((tool%12 == 0) ? tool/12 : tool/12+1) * work;
    msg[3] = tool;
    sendDevData(CAN_ID_DCR, msg);
    qDebug() << "dcr send:" << msg.toHex();
    // 04配置;0C电枢片数;02间隔;E803采样数;01自动档;05档位
    msg = QByteArray::fromHex("040C02E8030105");
    msg[1] = quan;  // 电枢片数
    msg[2] = spac;  // 间隔片数
    msg[3] = time%256;
    msg[4] = time/256;
    msg[5] = (freq == 0) ? 0x01 : 0x00;
    msg[6] = freq;
    sendDevData(CAN_ID_DCR, msg);
    qDebug() << "dcr send:" << msg.toHex();
}

void DevSetCan::startDCR(QTmpMap map)
{
    volatile int addr = tmpSet.value(currItem + 3000).toInt();
    QByteArray msg = QByteArray::fromHex("010103");  // 01启动;01片间;03单工位
    msg[2] = (map.value(Qt::Key_4).toInt() == 0x14) ? 2 : 1;  // 工位
    switch (currItem) {
    case Qt::Key_1:
        msg[1] = 0x01;
        sendDevData(CAN_ID_DCR, msg);
        qDebug() << "dcr send:" << msg.toHex();
        break;
    case Qt::Key_2:
        msg[1] = 0x02;
        sendDevData(CAN_ID_DCR, msg);
        qDebug() << "dcr send:" << msg.toHex();
        break;
    case Qt::Key_3:
        tmpSet.insert(addr + STATDCRA, 2);  // 修改电阻板状态
        renewDCR();  // 直接计算跨间电阻
        qDebug() << "dcr calc:" << tr("%1ms").arg(t.elapsed(), 4, 10, QChar('0'));
        break;
    default:
        qDebug() << "dcr know:" << map.value(Qt::Key_1).toInt();
        break;
    }
}

void DevSetCan::parseDCR(QByteArray msg)
{
    quint32 numb = 0;
    quint32 volt = 0;
    quint32 real = 0;
    volatile quint32 addr = tmpSet.value(currItem + 3000).toInt();
    volatile quint32 parm = tmpSet.value(currItem + 4000).toInt();
    volatile quint32 temp = tmpSet.value(3000 + Qt::Key_0).toInt();
    switch (quint8(msg.at(0))) {
    case 0x00:  // 状态
        if (currItem == Qt::Key_0) {
            tmpSta.insert(temp + TEMPDCRS, quint8(msg.at(1)));
        } else {
            qDebug() << "dcr recv:" << msg.toHex().toUpper();
            tmpSet.insert(addr + STATDCRA, quint8(msg.at(1)));  // 修改电阻板状态
            orderDCR();  // 自动排序
            judgeDCR();  // 电阻判定
            renewDCR();  // 刷新显示
        }
        break;
    case 0x01:  // 结果
        numb += quint32(msg.at(2));  // 电阻序号
        volt += quint32(msg.at(3));  // 电阻档位
        real += quint32(msg.at(4)) * (0x00000001 << 0x00);
        real += quint32(msg.at(5)) * (0x00000001 << 0x08);
        real += quint32(msg.at(6)) * (0x00000001 << 0x10);
        real += quint32(msg.at(7)) * (0x00000001 << 0x18);
        if (tmpSet.value(parm + ISTMDCR1).toInt() == 1) {  // 温度补偿
            double stdtmp = tmpSet.value(parm + TEMPDCR1).toDouble() / 100;
            real += real * (stdtmp - currTemp) / 10 * 0.0039;
        }
        qDebug() << "dcr recv:" << msg.toHex().toUpper() << volt << numb << real;
        real = (real * 10 / (((volt%3 == 0)) ? 1 : qPow(10, 3 - volt%3)) + 5) / 10;  // 四舍五入
        real = qMin(quint32(9999), real);
        tmpPow.insert(numb, volt);
        tmpDCR.insert(numb, real);
        break;
    case 0x05:  // 温度
        currTemp = quint32(msg.at(1)) + quint32(msg.at(2)) * 0x0100;
        currTemp = qMin(999, currTemp);
        tmpMsg.insert(Qt::Key_0, Qt::Key_Shop);
        tmpMsg.insert(temp + TEMPTEMP, currTemp);
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
        break;
    case 0x06:  // 校准
        break;
    case 0x08:  // 版本
        real += quint32(msg.at(6)) * (0x00000001 << 0x08);
        real += quint32(msg.at(7)) * (0x00000001 << 0x00);
        tmpSta.insert(temp + TEMPDCRV, real);
        break;
    case 0x0A:  // 启动停止
        qDebug() << "dcr recv:" << msg.toHex().toUpper();
        real = quint8(msg.at(1));
        tmpio = (tmpio == 0) ? real : tmpio;
        if (((tmpio << 0x00) & 0x08) != ((real << 0x00) & 0x08)) {
            tmpMsg.insert(Qt::Key_0, (((real << 0x00) & 0x08) == 0) ? Qt::Key_Play : Qt::Key_Stop);
            tmpMsg.insert(Qt::Key_1, "R");
            emit sendAppMsg(tmpMsg);
            tmpMsg.clear();
        }
        if (((tmpio << 0x01) & 0x08) != ((real << 0x01) & 0x08)) {
            tmpMsg.insert(Qt::Key_0, (((real << 0x01) & 0x08) == 0) ? Qt::Key_Play : Qt::Key_Stop);
            tmpMsg.insert(Qt::Key_1, "R");
            emit sendAppMsg(tmpMsg);
            tmpMsg.clear();
        }
        if (((tmpio << 0x02) & 0x08) != ((real << 0x02) & 0x08)) {
            tmpMsg.insert(Qt::Key_0, (((real << 0x02) & 0x08) == 0) ? Qt::Key_Play : Qt::Key_Stop);
            tmpMsg.insert(Qt::Key_1, "L");
            emit sendAppMsg(tmpMsg);
            tmpMsg.clear();
        }
        if (((tmpio << 0x03) & 0x08) != ((real << 0x03) & 0x08)) {
            tmpMsg.insert(Qt::Key_0, (((real << 0x03) & 0x08) == 0) ? Qt::Key_Play : Qt::Key_Stop);
            tmpMsg.insert(Qt::Key_1, "L");
            emit sendAppMsg(tmpMsg);
            tmpMsg.clear();
        }
        tmpio = real;
        break;
    default:
        break;
    }
}

void DevSetCan::orderDCR()
{
    //    QList<int> cache;
    //    cache << 2000 << 2500 << 2300 << 2100 << 2400 << 2200;
    //    xxx = (xxx > 5) ? 0 : xxx + 1;
    //    tmpDCR.clear();
    //    tmpPow.clear();
    //    for (int i=0; i < 6; i++) {
    //        tmpDCR.insert(i, cache.at((i + xxx) % 6));
    //        tmpPow.insert(i, 2);
    //        tmpdff.insert(i, cache.at((i + xxx) % 6));
    //    }
    if (currItem == Qt::Key_1 && tmpDCR.size() >= 5) {  // 片间电阻自动排序
        double addr = tmpSet.value(3000 + Qt::Key_1).toDouble();
        double parm = tmpSet.value(4000 + Qt::Key_1).toDouble();
        // 找到最小电阻
        double minrow = 0;
        double mindat = tmpDCR.value(0).toInt();
        for (int i=0; i < tmpDCR.size(); i++) {
            double t = tmpDCR.value(i).toDouble();
            minrow = (t <= mindat) ? i : minrow;
            mindat = (t <= mindat) ? t : mindat;
        }
        tmpSet.insert(addr + FROMDCRA, minrow);
        // 计算挂钩顺序
        int prerow = (minrow - 1) < 0 ? tmpDCR.size() - 1 : minrow - 1;  // 最低点前一个
        int nxtrow = (minrow + 1) < tmpDCR.size() ? minrow + 1 : 0;  // 最低点后一个
        int t1 = tmpSet.value(parm + TURNDCR1).toInt();
        int t2 = tmpDCR.value(prerow).toDouble() > tmpDCR.value(nxtrow).toDouble() ? 0 : 1;
        tmpSet.insert(addr + TURNDCRA, t2);
        tmpSet.insert(addr + TURNISOK, t2 == t1 ? DATAOK : DATANG);
        if (t1 != t2)
            tmpSet.insert(addr + OKNGDCRA, DATANG);
        // 电阻排序
        tmpRow.clear();
        for (int i=0; i < tmpDCR.size(); i++) {
            int t = (t2 == 0) ? minrow + tmpDCR.size() + i : minrow + tmpDCR.size() - i;
            t = t % tmpDCR.size();
            tmpRow[i] = t;
        }
        qDebug() << xxx << t2 << prerow << minrow  << nxtrow;
    }
    if (tmpRow.isEmpty()) {  // 未排序时默认值
        for (int i=0; i < tmpDCR.size(); i++) {
            tmpRow[i] = i;
        }
    }
}

void DevSetCan::judgeDCR()
{
    volatile double addr = tmpSet.value(3000 + currItem).toInt();  // 电阻测试结果地址
    volatile double parm = tmpSet.value(4000 + currItem).toInt();  // 电阻参数地址
    volatile double save = tmpSet.value(4000 + Qt::Key_9).toInt();  // 标准电阻地址
    if (currItem == Qt::Key_1) {  // 片间电阻结果判断
        int isOK = tmpSet.value(addr + OKNGDCRA).toInt();
        for (int i=0; i < tmpDCR.size(); i++) {
            double numb = tmpRow.value(i).toInt();
            double real = tmpDCR.value(numb).toInt();
            double gear = tmpPow.value(numb).toInt();
            double stdw = tmpSet.value(save + i*2 + 0).toDouble();  // 标准电阻
            double stdp = tmpSet.value(save + i*2 + 1).toDouble();
            double smax = tmpSet.value(parm + SMAXDCR1).toDouble();  // 电阻上限100%*1000
            tmpSet.insert(addr + CACHEDCR + 0x04*i + NUMBDCRR, numb);
            tmpSet.insert(addr + CACHEDCR + 0x04*i + DATADCRR, real);
            tmpSet.insert(addr + CACHEDCR + 0x04*i + GEARDCRR, gear);
            real = qMax(0.001, real * qPow(10, 3-gear));
            stdw = qMax(0.001, stdw * qPow(10, 3-stdp));
            double stdr = abs((real-stdw)/stdw*100*1000);
            double stdj = (stdr >= smax) ? DATANG : DATAOK;
            tmpSet.insert(addr + CACHEDCR + 0x04*i + OKNGDCRR, stdj);
            isOK =  (stdj == DATANG) ? DATANG : isOK;
            qDebug() << "dcr calc:" << i << numb << real << gear << stdw << stdr << smax;
        }
        tmpSet.insert(addr + OKNGDCRA, isOK);
    }
    if (currItem == Qt::Key_2) {  // 焊接电阻结果判断
        int isOK = DATAOK;
        for (int i=0; i < tmpDCR.size(); i++) {
            double numb = tmpRow.value(i).toInt();
            double real = tmpDCR.value(numb).toInt();
            double gear = tmpPow.value(numb).toInt();
            double smax = tmpSet.value(parm + SMAXDCR2).toDouble();  // 电阻上限100%*1000
            tmpSet.insert(addr + CACHEDCR + 0x04*i + NUMBDCRR, numb);
            tmpSet.insert(addr + CACHEDCR + 0x04*i + DATADCRR, real);
            tmpSet.insert(addr + CACHEDCR + 0x04*i + GEARDCRR, gear);
            real = qMax(0.001, real * qPow(10, 3-gear));
            double stdj = (real*1000 >= smax) ? DATANG : DATAOK;
            tmpSet.insert(addr + CACHEDCR + 0x04*i + OKNGDCRR, stdj);
            isOK =  (stdj == DATANG) ? DATANG : isOK;
            qDebug() << "dcr calc:" << i << real << gear << real << smax;
        }
        tmpSet.insert(addr + OKNGDCRA, isOK);
        qDebug() << isOK << addr + OKNGDCRA;
    }
    if (currItem == Qt::Key_3) {
        int isOK = DATAOK;
        for (int i=0; i < tmpDCR.size(); i++) {
            double real = tmpDCR.value(i).toInt();
            double gear = tmpPow.value(i).toInt();
            double smin = tmpSet.value(parm + SMINDCR3).toDouble();  // 电阻下限
            double smax = tmpSet.value(parm + SMAXDCR3).toDouble();  // 电阻上限
            tmpSet.insert(addr + CACHEDCR + 0x04*i + DATADCRR, real);
            tmpSet.insert(addr + CACHEDCR + 0x04*i + GEARDCRR, gear);
            real = qMax(0.001, real * qPow(10, 3-gear));
            double stdj = (real >= smax || real <= smin) ? DATANG : DATAOK;
            tmpSet.insert(addr + CACHEDCR + 0x04*i + OKNGDCRR, stdj);
            isOK =  (stdj == DATANG) ? DATANG : isOK;
            qDebug() << "dcr calc:" << i << real << gear << smin << smax << real;
        }
        tmpSet.insert(addr + OKNGDCRA, isOK);
    }
    parm = tmpSet.value((4000 + Qt::Key_3)).toInt();  // 跨间电阻参数地址
    addr = tmpSet.value((3000 + Qt::Key_3)).toInt();  // 跨间电阻结果地址
    // 计算跨间电阻
    if (currItem == Qt::Key_1) {
        int quan = tmpDCR.size();
        int isOK = DATAOK;
        for (int i=0; i < quan/2; i++) {
            double all1 = 0;
            double all2 = 0;
            for (int j=0; j < quan/2; j++) {
                all1 += (tmpDCR.value((j + i) % quan)).toInt();
                all2 += (tmpDCR.value((j + i + quan/2) % quan)).toInt();
            }
            double real = all1*all2/(all1+all2) * quan / (quan -1);
            double gear = tmpPow.value(i).toInt();
            double smin = tmpSet.value(parm + SMINDCR3).toDouble();  // 电阻下限
            double smax = tmpSet.value(parm + SMAXDCR3).toDouble();  // 电阻上限
            gear = (real > 9999) ? gear - 1 : gear;
            real = (real > 9999) ? real / 10 : real;
            tmpSet.insert(addr + CACHEDCR + 0x04*i + DATADCRR, int(real));
            tmpSet.insert(addr + CACHEDCR + 0x04*i + GEARDCRR, int(gear));
            real = qMax(0.001, real * qPow(10, 3-gear));
            double stdj = (real >= smax || real <= smin) ? DATANG : DATAOK;
            tmpSet.insert(addr + CACHEDCR + 0x04*i + OKNGDCRR, stdj);
            isOK =  (stdj == DATANG) ? DATANG : isOK;
            qDebug() << "dcr calc:" << i << real << gear << smin << smax;
        }
        tmpSet.insert(addr + OKNGDCRA, isOK);
    }
    tmpDCR.clear();
    tmpPow.clear();
}

void DevSetCan::renewDCR()
{
    quint32 addr = tmpSet.value(currItem + 3000).toInt();  // 测试结果存储地址
    for (int i=0; i < 255; i++) {
        tmpDat.insert(addr + i, tmpSet.value(addr + i));
    }
    tmpDat.insert(Qt::Key_0, Qt::Key_News);
    tmpDat.insert(Qt::Key_1, currItem);
    emit sendAppMsg(tmpDat);
    tmpDat.clear();
    tmpSet.insert(addr + OKNGDCRA, DATAOK);  // 清除判定
}

void DevSetCan::setupACW()
{
    QByteArray msg;
    for (int i=0; i < 4; i++) {
        int addr = tmpSet.value(4000 + Qt::Key_4 + i).toInt();  // 高压配置地址
        int volt = tmpSet.value(addr + AddrACWSV).toInt();  // 电压
        int time = tmpSet.value(addr + AddrACWST).toInt();  // 时间
        // 03配置;00序号;04绝缘;0001轴;0004线;00频率
        msg = QByteArray::fromHex("0300040001000400");
        msg[1] = i;
        msg[2] = (i == NUMBINRS) ? 0x04 : 0x05;  // 绝缘04,耐压05
        msg[4] = (i == 2) ? 0x02 : 0x01;  // 轴铁,轴线,铁线,轴线 高
        msg[6] = (i == 0) ? 0x02 : 0x04;  // 轴铁,轴线,铁线,轴线 低
        msg[7] = (i == NUMBINRS) ? 0x00 : 0x32;  // 绝缘00Hz,耐压50Hz
        sendDevData(CAN_ID_ACW, msg);
        qDebug() << "acw send:" << msg.toHex().toUpper();
        // 04配置;00序号;01F4电压;000A时间;0000下限
        msg = QByteArray::fromHex("040001F4000A0000");
        msg[1] = i;
        msg[2] = volt/256;
        msg[3] = volt%256;
        msg[4] = time/256;
        msg[5] = time%256;
        sendDevData(CAN_ID_ACW, msg);
        qDebug() << "acw send:" << msg.toHex().toUpper();
        // 05配置;00序号;0000上限;00电弧;030A缓升缓降
        msg = QByteArray::fromHex("0500000000030A");
        msg[1] = i;
        msg[2] = (i == NUMBINRS) ? 0x00 : 0x09;  // 绝缘上限0,耐压上限2500
        msg[3] = (i == NUMBINRS) ? 0x00 : 0xC4;  // 绝缘上限0,耐压上限2500
        sendDevData(CAN_ID_ACW, msg);
        qDebug() << "acw send:" << msg.toHex().toUpper();
    }
}

void DevSetCan::startACW(QTmpMap map)
{
    int curr = currItem - Qt::Key_4;
    int addr = tmpSet.value(3000 + currItem).toInt();
    QByteArray msg = QByteArray::fromHex("0104001100");  // 01启动;04绝缘;00自动档;11工位;00序号
    msg[1] = (curr == NUMBINRS) ? 0x04 : 0x05;  // 04绝缘/05交耐
    msg[3] = map.value(Qt::Key_4).toInt();
    msg[4] = curr;
    sendDevData(CAN_ID_ACW, msg);
    tmpSet.insert(addr + STATACWA, DataTest);  // 修改高压板状态
    qDebug() << "acw send:" << msg.toHex() << map.value(Qt::Key_4).toInt();
}

void DevSetCan::parseACW(QByteArray msg)
{
    quint8 cmd = quint8(msg.at(0));
    quint8 num = quint8(msg.at(1));
    int curr = currItem - Qt::Key_4;
    int addr = tmpSet.value(3000 + currItem).toInt();  // 测试结果存储地址
    if (cmd == 0x00) {
        if (currItem == Qt::Key_0) {
            quint32 temp = tmpSet.value(3000 + Qt::Key_0).toInt();
            tmpSta.insert(temp + TEMPACWS, quint8(msg.at(1)));
        } else {
            qDebug() << "acw recv:" << msg.toHex();
            tmpSet.insert(addr + STATACWA, num);  // 修改高压板状态
            renewACW();
        }
    }
    if (cmd == 0x01) {
        qDebug() << "acw recv:" << msg.toHex() << currItem;
        double parm = tmpSet.value(4000 + currItem).toInt();  // 测试参数存储地址
        double smax = tmpSet.value(parm + AddrACWSH).toDouble();  // 上限
        double smin = tmpSet.value(parm + AddrACWSL).toDouble();  // 下限
        double volt = quint8(msg.at(1))*256 + (quint8)msg.at(2);  // 实测电压
        double real = quint8(msg.at(3))*256 + (quint8)msg.at(4);  // 实测结果
        double gear = quint8(msg.at(5));  // 小数位数
        tmpSet.insert(addr + CACHEACW + VOLTACWR, volt);
        tmpSet.insert(addr + CACHEACW + DATAACWR, real);
        tmpSet.insert(addr + CACHEACW + GEARACWR, gear);
        real *= qPow(10, -gear);
        smax = (curr == NUMBINRS) ? smax : smax/100;  // 耐压电流倍数100
        smin = (curr == NUMBINRS) ? smin : smin/100;  // 耐压电流倍数100
        if (((smax == 0) && (real > smin)) || ((smax != 0) && (real > smin && real < smax))) {
            tmpSet.insert(addr + CACHEACW + OKNGACWR, DATAOK);
        } else {
            tmpSet.insert(addr + CACHEACW + OKNGACWR, DATANG);
            tmpSet.insert(addr + OKNGACWA, DATANG);
        }
    }
    if (cmd == 0x06) {
        qDebug() << "acw recv:" << msg.toHex();
        tmpMsg.insert(Qt::Key_0, Qt::Key_News);
        tmpMsg.insert(Qt::Key_1, msg.toHex());
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
    }
    if (cmd == 0x08) {  // 版本
        quint32 temp = tmpSet.value(3000 + Qt::Key_0).toInt();
        quint32 real = 0;
        real += quint32(msg.at(6)) * (0x00000001 << 0x08);
        real += quint32(msg.at(7)) * (0x00000001 << 0x00);
        tmpSta.insert(temp + TEMPACWV, real);
        //        qDebug() << "acw vern:" << msg.toHex();
    }
}

void DevSetCan::renewACW()
{
    int addr = tmpSet.value(3000 + currItem).toInt();  // 测试结果存储地址
    for (int i=0; i < 8; i++) {
        tmpDat.insert(addr + i, tmpSet.value(addr + i));
    }
    tmpDat.insert(Qt::Key_0, Qt::Key_News);
    tmpDat.insert(Qt::Key_1, currItem);
    emit sendAppMsg(tmpDat);
    tmpDat.clear();
    tmpSet.insert(addr + OKNGACWA, DATAOK);  // 清除判定
}

void DevSetCan::setupIMP(QTmpMap map)
{
    int addr = tmpSet.value(4000 + Qt::Key_0).toInt();  // 综合配置地址
    int conf = tmpSet.value(4000 + Qt::Key_8).toInt();  // 匝间配置地址
    int quan = tmpSet.value(addr + AddrDCRSC).toInt();  // 电枢片数
    int spac = tmpSet.value(conf + AddrIMPSA).toInt();  // 间隔(>=1)
    int volt = tmpSet.value(conf + AddrIMPSV).toInt();  // 电压
    int time = tmpSet.value(conf + AddrIMPST).toInt();  // 次数
    int freq = tmpSet.value(conf + AddrIMPSF).toInt();  // 频率
    freq = (map.value(Qt::Key_5).isNull()) ? freq : map.value(Qt::Key_5).toInt();  // 采样配置
    // 03配置;00序号;0C电枢片数;01间隔;01F4电压;11工位;07频率
    QByteArray msg = QByteArray::fromHex("03000C0101F41107");
    msg[2] = quan;
    msg[3] = spac;
    msg[4] = volt/256;
    msg[5] = volt%256;
    msg[6] = (((volt - 10) / 1000 + 1) << 4) + time;  // 档位+次数
    msg[7] = freq;
    sendDevData(CAN_ID_IMP, msg);
    qDebug() << "imp send:" << msg.toHex();
}

void DevSetCan::startIMP(QTmpMap map)
{
    int parm = tmpSet.value(4000 + Qt::Key_0).toInt();  // 综合配置地址
    int addr = tmpSet.value(3000 + Qt::Key_8).toInt();  // 匝间结果地址
    int quan = tmpSet.value(parm + AddrDCRSC).toInt();
    int tool = tmpSet.value(parm + AddrDEVSC).toInt() * quan;  // 夹具针数
    QByteArray msg = QByteArray::fromHex("010011000106");  // 01启动;00序号;11工位;0001序号
    msg[1] = (map.value(Qt::Key_5).isNull()) ? 0x00 : 0x00;  // 00测试;03采样
    msg[2] = (map.value(Qt::Key_4).toInt() == 0x14) ? 0x14 : 0x11;  // 工位
    msg[5] = tool;
    sendDevData(CAN_ID_IMP, msg);
    tmpSet.insert(addr + STATIMPA, DataTest);
    qDebug() << "imp send:" << msg.toHex();
    numb = 1;
}

void DevSetCan::parseIMP(int id, QByteArray msg)
{
    if (id == 0x0081) {
        quint8 cmd = quint8(msg.at(0));
        quint8 num = quint8(msg.at(1));
        volatile int addr = tmpSet.value(3000 + Qt::Key_8).toInt();  // 匝间结果地址
        if (cmd == 0x00) {
            if (currItem == Qt::Key_0) {
                quint32 temp = tmpSet.value(3000 + Qt::Key_0).toInt();
                tmpSta.insert(temp + TEMPIMPS, num);
            } else {
                qDebug() << "imp recv:" << msg.toHex();
                tmpSet.insert(addr + STATIMPA, num);
                renewIMP();
            }
        }
        if (cmd == 0x02) {
            qDebug() << "imp recv:" << msg.toHex();
            tmpIMP.insert(numb - 1, quint8(msg.at(4))*256 + (quint8)msg.at(5));  // 匝间电压
        }
        if (cmd == 0x03) {
            qDebug() << "imp recv:" << msg.toHex();
            if (num == 0xFF) {  // 波形结束
                judgeIMP();
                renewIMP();
                numb++;
            } else {
                numb = num;  // 波形编号
                tmpSet.insert(addr + NUMBIMPA, numb);
            }
        }
        if (cmd == 0x06) {
            qDebug() << "imp recv:" << msg.toHex();
            tmpMsg.insert(Qt::Key_0, Qt::Key_News);
            tmpMsg.insert(Qt::Key_1, msg.toHex());
            emit sendAppMsg(tmpMsg);
            tmpMsg.clear();
        }
        if (cmd == 0x08) {
            quint32 temp = tmpSet.value(3000 + Qt::Key_0).toInt();
            quint32 real = 0;
            real += quint32(msg.at(6)) * (0x00000001 << 0x08);
            real += quint32(msg.at(7)) * (0x00000001 << 0x00);
            tmpSta.insert(temp + TEMPIMPV, real);
        }
    }
    if (id == 0x0481) {
        for (int i=0; i < 8; i++) {
            wave.append(quint8(msg.at(i)));
        }
    }
}

void DevSetCan::judgeIMP()
{
    QList<int> tmpWave;
    int tmp = 512;
    for (int i=0; i < wave.size(); i++) {
        quint8 tmpV = quint8(wave.at(i));
        if (tmpV <= 0x78) {  // <=120 与上一位相加
            tmp = tmp + tmpV;
            tmpWave.append(tmp);
        } else if (tmpV <= 0x7F && tmpV > 0x78) {  // 127 下一位为实际值
            tmp = 0;
        } else if (tmpV <= 0xF7 && tmpV > 0x7F) {  // >127 <= 240 与上一位相减
            tmp = tmp + 0x7F - tmpV;
            tmpWave.append(tmp);
        } else {
            tmp = (tmpV - 0xF7) * 120;
        }
    }
    wave = tmpWave;

    volatile int addr = tmpSet.value(3000 + Qt::Key_8).toInt();  // 匝间结果地址
    volatile int conf = tmpSet.value(4000 + Qt::Key_0).toInt();  // 综合配置地址
    volatile int parm = tmpSet.value(4000 + Qt::Key_8).toInt();  // 匝间配置地址
    volatile int stdw = tmpSet.value(4000 + Qt::Key_A).toInt();  // 标准波形地址
    volatile int weld = tmpSet.value(conf + AddrDCRSC).toInt();  // 电枢片数
    volatile int need = tmpSet.value(parm + AddrIMPSA).toInt();  // 间隔片数
    volatile int save = (weld == need*2) ? ((numb - 1) % need) + 1 : numb;  // 对角测试
    for (int i=0; i < tmpRow.size(); i++) {
        if (tmpRow.value(i).toInt() == numb - 1)
            save = i + 1;
    }
    double sdot = tmpSet.value(parm + AddrIMPSL).toDouble();  // 采样点
    double smax = tmpSet.value(parm + AddrIMPSH).toDouble();  // 匝间上限
    int f1 = qMax(1, tmpSet.value(parm + AddrIMPF1).toInt());
    int f2 = qMin(wave.size()-1, tmpSet.value(parm + AddrIMPF2).toInt());
    stdw += (sdot == 0) ? (save-1)*IMP_SIZE : (sdot-1)*IMP_SIZE;
    quint32 area1 = 0;
    quint32 area2 = 0;
    quint32 area3 = 0;
    quint32 diff = 0;
    for (int i=f1; i < f2; i++) {
        int a1 = tmpSet.value(stdw + i).toInt();
        int a2 = wave.at(i);
        area1 += abs(a1-0x200);
        area2 += abs(a2-0x200);
        int b1 = tmpSet.value(stdw + i - 1).toInt();
        int b2 = tmpSet.value(stdw + i).toInt();
        int b3 = tmpSet.value(stdw + i + 1).toInt();
        int c1 = wave.at(i - 1);
        int c2 = wave.at(i);
        int c3 = wave.at(i + 1);
        area3 += abs((b1+b2*2+b3)-(c1+c2*2+c3));
    }
    diff = qMin(area2, area3/4)*100*1000/(qMax(quint32(1), area1));
    diff = qMin(diff, quint32(99990));

    if (diff >= smax*1000) {
        tmpSet.insert(addr + OKNGIMPA, DATANG);
        tmpSet.insert(addr + CACHEIMP + 4*(save-1) + OKNGIMPR, DATANG);
    } else {
        tmpSet.insert(addr + CACHEIMP + 4*(save-1) + OKNGIMPR, DATAOK);
    }
    int s = tmpSet.value(3000 + Qt::Key_A).toInt();  // 测试波形存储地址
    for (int i=0; i < wave.size(); i++) {
        tmpSet.insert(s + i, wave.at(i));  // 临时存储波形
    }
    tmpSet.insert(addr + NUMBIMPA, save);
    tmpSet.insert(addr + CACHEIMP + 4*(save - 1) + DATAIMPR, diff);
    tmpSet.insert(addr + CACHEIMP + 4*(save - 1) + VOLTIMPR, tmpIMP.value(save-1));
    wave.clear();
    qDebug() << "imp calc:" << numb << save << sdot << diff << smax*1000;
}

void DevSetCan::renewIMP()
{
    volatile int addr = tmpSet.value(3000 + Qt::Key_8).toInt();  // 测试结果存储地址
    for (int i=0; i < 255; i++) {
        tmpDat.insert(addr + i*4 + 0, tmpSet.value(addr + i*4 + 0));
        tmpDat.insert(addr + i*4 + 1, tmpSet.value(addr + i*4 + 1));
        tmpDat.insert(addr + i*4 + 2, tmpSet.value(addr + i*4 + 2));
        tmpDat.insert(addr + i*4 + 3, tmpSet.value(addr + i*4 + 3));
    }
    volatile int tmpw = tmpSet.value(3000 + Qt::Key_A).toInt();
    for (int i=0; i < IMP_SIZE; i++) {
        tmpDat.insert(tmpw + i, tmpSet.value(tmpw + i));
    }
    tmpDat.insert(Qt::Key_0, Qt::Key_News);
    tmpDat.insert(Qt::Key_1, currItem);
    emit sendAppMsg(tmpDat);
    tmpDat.clear();
    tmpSet.insert(addr + OKNGIMPA, DATAOK);  // 清除判定
}

void DevSetCan::setupSave(QTmpMap msg)
{
    QByteArray hex = QByteArray::fromHex(msg.value(Qt::Key_3).toByteArray());
    qDebug() << "hex send:" << hex.toHex();
    switch (msg.value(Qt::Key_2).toInt()) {
    case Qt::Key_1:  // 下发电阻调试参数
        sendDevData(CAN_ID_DCR, hex);
        break;
    case Qt::Key_4:  // 下发耐压调试参数
        sendDevData(CAN_ID_ACW, hex);
        break;
    case Qt::Key_8:  // 下发匝间调试参数
        sendDevData(CAN_ID_IMP, hex);
        break;
    default:
        break;
    }
}

void DevSetCan::setupView(QTmpMap msg)
{
    switch (msg.value(Qt::Key_2).toInt()) {
    case Qt::Key_1:  // 电阻参数
        sendDevData(CAN_ID_DCR, QByteArray::fromHex("06EE"));
        break;
    case Qt::Key_4:  // 耐压参数
        sendDevData(CAN_ID_ACW, QByteArray::fromHex("06EE"));
        break;
    case Qt::Key_8:  // 匝间参数
        sendDevData(CAN_ID_IMP, QByteArray::fromHex("06EE"));
        break;
    default:
        break;
    }
}

void DevSetCan::setupPump(QTmpMap msg)
{
    QString tmp = msg.value(Qt::Key_1).toString();
    if (msg.value(Qt::Key_3).isNull() && tmp == "LEDY") {
        int s = msg.value(Qt::Key_4).toInt();
        sendDevData(CAN_ID_DCR, QByteArray::fromHex((s == 0x11) ? "0901" : "0904"));
    } else {
        sendDevData(CAN_ID_DCR, QByteArray::fromHex("0900"));
        resetTest(msg);
    }
    qDebug() << "dcr send:" << "pump";
}

void DevSetCan::setupTest(QTmpMap msg)
{
    switch (msg.value(Qt::Key_1).toInt()) {
    case Qt::Key_0:  // 进入测试界面下发配置
        sendDevData(CAN_ID_DCR, QByteArray::fromHex("0900"));
        qDebug() << "dcr send:" << "0900";
        wait(200);
        setupACW();
        setupIMP(msg);
        break;
    case Qt::Key_1:  // 片间电阻采样
        sendDevData(CAN_ID_DCR, QByteArray::fromHex("0901"));
        qDebug() << "dcr send:" << "0901";
        wait(200);
        currItem = Qt::Key_1;
        setupDCR(msg);
        startDCR(msg);
        break;
    case Qt::Key_8:  // 匝间波形采样
        sendDevData(CAN_ID_DCR, QByteArray::fromHex("0901"));
        qDebug() << "dcr send:" << "0901";
        wait(200);
        currItem = Qt::Key_8;
        setupIMP(msg);
        startIMP(msg);
        break;
    case Qt::Key_View:
        setupView(msg);
        break;
    case Qt::Key_Save:
        setupSave(msg);
        break;
    default:
        break;
    }
}

void DevSetCan::startTest(QTmpMap map)
{
    currItem = map.value(Qt::Key_1).toInt();
    switch (currItem) {
    case Qt::Key_1:
    case Qt::Key_2:
    case Qt::Key_3:
        setupDCR(map);
        startDCR(map);
        break;
    case Qt::Key_4:
    case Qt::Key_5:
    case Qt::Key_6:
    case Qt::Key_7:
        startACW(map);
        break;
    case Qt::Key_8:
        startIMP(map);
        break;
    default:
        qDebug() << "can recv:" << currItem;
        break;
    }
}

void DevSetCan::resetTest(QTmpMap msg)
{
    switch (currItem) {
    case Qt::Key_1:
    case Qt::Key_2:
    case Qt::Key_3:
        sendDevData(CAN_ID_DCR, QByteArray::fromHex("02"));
        qDebug() << "dcr stop:" << "02";
        break;
    case Qt::Key_4:
    case Qt::Key_5:
    case Qt::Key_6:
    case Qt::Key_7:
        sendDevData(CAN_ID_ACW, QByteArray::fromHex("02"));
        qDebug() << "acw stop:" << "02";
        break;
    case Qt::Key_8:
        sendDevData(CAN_ID_IMP, QByteArray::fromHex("02"));
        qDebug() << "imp stop:" << "02";
        break;
    default:
        sendDevData(CAN_ID_DCR, QByteArray::fromHex("02"));
        sendDevData(CAN_ID_ACW, QByteArray::fromHex("02"));
        sendDevData(CAN_ID_IMP, QByteArray::fromHex("02"));
        qDebug() << "all stop:" << "02" << msg.value(Qt::Key_1).toInt();
        break;
    }
}

void DevSetCan::recvAppMsg(QTmpMap msg)
{
    switch (msg.value(Qt::Key_0).toInt()) {
    case Qt::Key_Copy:
        tmpSet = msg;  // 保存设置参数
        break;
    case Qt::Key_Time:
        getAllDat();
        putAllDat();
        updateAll();
        //        timeOut++;
        //        if (timeOut % 500 == 0) {
        //            currItem = Qt::Key_0;
        //            sendDevData(CAN_ID_DCR, QByteArray::fromHex("08"));
        //            sendDevData(CAN_ID_ACW, QByteArray::fromHex("08"));
        //            sendDevData(CAN_ID_IMP, QByteArray::fromHex("08"));
        //            sendDevData(CAN_ID_DCR, QByteArray::fromHex("05"));
        //            tmpSta.insert(Qt::Key_0, Qt::Key_Menu);
        //            emit sendAppMsg(tmpSta);
        //            tmpSta.clear();
        //        }
        break;
    case Qt::Key_Send:
        setupTest(msg);
        break;
    case Qt::Key_Play:
        startTest(msg);
        break;
    case Qt::Key_Call:
        setupPump(msg);
        break;
    default:
        break;
    }
}

void DevSetCan::wait(int ms)
{
    QElapsedTimer t;
    t.start();
    while (t.elapsed() < ms)
        QCoreApplication::processEvents();
}
