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
    currItem = 0;
    currTemp = 0;
    sendDevData(CAN_ID_DCR, QByteArray::fromHex("00"));
    sendDevData(CAN_ID_ACW, QByteArray::fromHex("00"));
    sendDevData(CAN_ID_IMP, QByteArray::fromHex("00"));
    sendDevData(CAN_ID_DCR, QByteArray::fromHex("05"));
    sendDevData(CAN_ID_DCR, QByteArray::fromHex("08"));
    sendDevData(CAN_ID_ACW, QByteArray::fromHex("08"));
    sendDevData(CAN_ID_IMP, QByteArray::fromHex("08"));
    sendDevData(CAN_ID_DCR, QByteArray::fromHex("0A"));
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
    //    if (id != 0x481)
    //        qDebug() << "can recv:"
    //                 << QString("%1").arg(id, 4, 16, QChar('0'))
    //                 << msg.toHex().toUpper();
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
        //        qDebug() << "can send:"
        //                 << QString("%1").arg(frame.can_id, 4, 16, QChar('0'))
        //                 << msg.toHex().toUpper();
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
    tmpDat.insert(AddrEnum, Qt::Key_News);
    tmpDat.insert(AddrText, currItem);
    emit sendAppMsg(tmpDat);
}

void DevSetCan::setupDCR(QTmpMap map)
{
    int addr = tmpSet[AddrModel].toInt();  // 综合配置地址
    int quan = tmpSet[addr + AddrDCRSC].toInt();  // 电枢片数
    int tool = tmpSet[addr + AddrDEVSC].toInt() * quan;  // 夹具针数
    int time = tmpSet[tmpSet[AddrDCRS1].toInt() + TIMEDCR1].toInt();  // 测试时间
    int freq = map.value(AddrFreq).toInt();  // 档位
    int spac = 0;  // 间隔片数
    if (map.value(AddrFreq).isNull()) {
        if (currItem == AddrDCRS1) {
            freq = tmpSet[tmpSet[AddrDCRS1].toInt() + GEARDCR1].toInt();  // 片间档位
        } else if (currItem == AddrDCRS3) {
            freq = tmpSet[tmpSet[AddrDCRS3].toInt() + GEARDCR3].toInt();  // 跨间档位
        }
    }
    if (currItem == AddrDCRS3) {
        spac = quan/2 -1;
    }
    QByteArray msg;
    // 03配置;01工位数;01板卡数;0C夹具数
    msg = QByteArray::fromHex("0301010C");
    msg[2] = (tool%12 == 0) ? tool/12 : tool/12+1;
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
    QByteArray msg = QByteArray::fromHex("010103");  // 01启动;01片间;03单工位
    currItem = map.value(AddrFreq).isNull() ? currItem : map.value(AddrText).toInt();
    msg[1] = (currItem == AddrDCRS2) ? 0x02 : 0x01;  // 01片间;02焊接
    if (map.value(AddrFreq).isNull() && currItem == AddrDCRS3) {
        double a = tmpSet.value(AddrDCRR3).toDouble();
        tmpSet.insert(a + STATDCRA, 2);  // 修改电阻板状态
        renewDCR();  // 直接计算跨间电阻
    } else {
        sendDevData(CAN_ID_DCR, msg);
    }
    qDebug() << "dcr send:" << msg.toHex();
}

void DevSetCan::parseDCR(QByteArray msg)
{
    quint32 numb = 0;
    quint32 volt = 0;
    quint32 real = 0;
    quint32 curr = (currItem >AddrDCRS3 || currItem < AddrDCRS1) ? 0 : currItem;  // 测试组
    quint32 addr = tmpSet[AddrDCRR1 + curr - AddrDCRS1].toInt();  // 片间/焊接/跨间电阻结果地址
    quint32 parm = tmpSet[curr].toInt();
    switch (quint8(msg.at(0))) {
    case 0x00:  // 状态
        qDebug() << "dcr recv:" << msg.toHex().toUpper();
        tmpSet.insert(addr + STATDCRA, quint8(msg.at(1)));  // 修改电阻板状态
        orderDCR();  // 自动排序
        judgeDCR();  // 电阻判定
        renewDCR();  // 刷新显示
        break;
    case 0x01:  // 结果
        numb += quint32(msg.at(2));  // 电阻序号
        volt += quint32(msg.at(3));  // 电阻档位
        real += quint32(msg.at(4)) * (0x00000001 << 0x00);
        real += quint32(msg.at(5)) * (0x00000001 << 0x08);
        real += quint32(msg.at(6)) * (0x00000001 << 0x10);
        real += quint32(msg.at(7)) * (0x00000001 << 0x18);
        if (tmpSet[parm + ISTMDCR1].toInt() == 1) {  // 温度补偿
            double stdtmp = tmpSet[parm + TEMPDCR1].toDouble() / 100;
            real += real * (stdtmp - currTemp) / 10 * 0.0039;
        }
        qDebug() << "dcr recv:" << msg.toHex().toUpper() << volt << numb << real;
        real /= ((volt%3 == 0)) ? 1: qPow(10, 3 - volt%3);
        real = qMin(quint32(9999), real);
        tmpPow.insert(numb, volt);
        tmpDCR.insert(numb, real);
        break;
    case 0x05:  // 温度
        currTemp = quint32(msg.at(1)) + quint32(msg.at(2)) * 0x0100;
        currTemp = qMin(999, currTemp);
        tmpMsg.insert(AddrEnum, Qt::Key_Shop);
        tmpMsg.insert(DataTemp, currTemp);
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
        break;
    case 0x06:  // 校准
        break;
    case 0x08:  // 版本
        qDebug() << "dcr recv:" << msg.toHex().toUpper();
        real += quint32(msg.at(6)) * (0x00000001 << 0x08);
        real += quint32(msg.at(7)) * (0x00000001 << 0x00);
        tmpSet[addr + HARDDCRA] = real;
        break;
    case 0x0A:  // 启动停止
        qDebug() << "dcr recv:" << msg.toHex().toUpper();
        real = quint8(msg.at(1));
        tmpio = (tmpio == 0) ? real : tmpio;
        if (((tmpio << 0x00) & 0x08) != ((real << 0x00) & 0x08)) {
            tmpMsg.insert(AddrEnum, (((real << 0x00) & 0x08) == 0) ? Qt::Key_Play : Qt::Key_Stop);
            tmpMsg.insert(AddrText, "R");
            emit sendAppMsg(tmpMsg);
            tmpMsg.clear();
        }
        if (((tmpio << 0x01) & 0x08) != ((real << 0x01) & 0x08)) {
            tmpMsg.insert(AddrEnum, (((real << 0x01) & 0x08) == 0) ? Qt::Key_Play : Qt::Key_Stop);
            tmpMsg.insert(AddrText, "R");
            emit sendAppMsg(tmpMsg);
            tmpMsg.clear();
        }
        if (((tmpio << 0x02) & 0x08) != ((real << 0x02) & 0x08)) {
            tmpMsg.insert(AddrEnum, (((real << 0x02) & 0x08) == 0) ? Qt::Key_Play : Qt::Key_Stop);
            tmpMsg.insert(AddrText, "L");
            emit sendAppMsg(tmpMsg);
            tmpMsg.clear();
        }
        if (((tmpio << 0x03) & 0x08) != ((real << 0x03) & 0x08)) {
            tmpMsg.insert(AddrEnum, (((real << 0x03) & 0x08) == 0) ? Qt::Key_Play : Qt::Key_Stop);
            tmpMsg.insert(AddrText, "L");
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
    currItem = (currItem > AddrDCRS3 || currItem < AddrDCRS1) ? AddrDCRS1 : currItem;
    if (currItem == AddrDCRS1 && tmpDCR.size() >= 5) {  // 片间电阻自动排序
        double addr = tmpSet.value(AddrDCRR1).toDouble();
        // 找到最小电阻
        double minrow = 0;
        double mindat = tmpDCR.value(0).toInt();
        for (int i=0; i < tmpDCR.size(); i++) {
            double t = tmpDCR.value(i).toDouble();
            minrow = (t <= mindat) ? i : minrow;
            mindat = (t <= mindat) ? t : mindat;
        }
        tmpSet[addr + FROMDCRA] = minrow;
        // 计算挂钩顺序
        int t1 = (minrow - 1) < 0 ? tmpDCR.size() : minrow - 1;  // 最低点前一个
        int t3 = (minrow + 1) < tmpDCR.size() ? minrow + 1 : 0;  // 最低点后一个
        int t2 = tmpDCR.value(t1).toDouble() > tmpDCR.value(t3).toDouble() ? 0 : 1;
        tmpSet[addr + TURNDCRA] = t2;
        // 电阻排序
        tmpRow.clear();
        for (int i=0; i < tmpDCR.size(); i++) {
            int t = (t2 == 0) ? minrow + tmpDCR.size() + i : minrow + tmpDCR.size() - i;
            t = t % tmpDCR.size();
            tmpRow[i] = t;
        }
    }
    if (tmpRow.isEmpty()) {  // 未排序时默认值
        for (int i=0; i < tmpDCR.size(); i++) {
            tmpRow[i] = i;
        }
    }
}

void DevSetCan::judgeDCR()
{
    int isOK = DataOK;
    if (currItem == AddrDCRS1) {  // 片间电阻结果判断
        for (int i=0; i < tmpDCR.size(); i++) {
            double n = tmpRow.value(i).toInt();
            double r = tmpDCR.value(n).toInt();
            double p = tmpPow.value(n).toInt();
            double a = tmpSet.value(AddrDCRS1).toInt();  // 片间电阻参数地址
            double s = tmpSet.value(AddrDCRR1).toInt() + CACHEDCR;  // 片间电阻测试结果地址
            double w = tmpSet[AddrDCRSW].toInt();  // 标准电阻地址
            tmpSet[s + 0x04*i + NUMBDCRR] = n;
            tmpSet[s + 0x04*i + DATADCRR] = r;
            tmpSet[s + 0x04*i + GEARDCRR] = p;
            double real = r * qPow(10, 3-p);
            double stdw = tmpSet[w + i*2 + 0].toDouble();  // 标准电阻
            double stdp = tmpSet[w + i*2 + 1].toDouble();
            double std1 = tmpSet[a + SMAXDCR1].toDouble();  // 电阻上限100%*1000
            stdw = stdw * qPow(10, 3-stdp);
            stdw = qMax(0.001, stdw);
            double stdr = abs((real-stdw)/stdw*100*1000);
            tmpSet[s + 0x04*i + OKNGDCRR] = (stdr >= std1) ? DataNG : DataOK;
            isOK =  (stdr >= std1) ? DataNG : isOK;
            tmpSet[tmpSet.value(AddrDCRR1).toInt() + OKNGDCRA] = isOK;
            qDebug() << i << n << r << p << stdw << stdr << std1;
        }
    }
    if (currItem == AddrDCRS2) {  // 焊接电阻结果判断
        for (int i=0; i < tmpDCR.size(); i++) {
            double r = tmpDCR.value(i).toInt();
            double p = tmpPow.value(i).toInt();
            double s = tmpSet.value(AddrDCRR2).toInt() + CACHEDCR;
            tmpSet[s + 0x04*i + DATADCRR] = r;
            tmpSet[s + 0x04*i + GEARDCRR] = p;
            double real = r * qPow(10, 3-p);
            double std1 = tmpSet[currItem + SMAXDCR2].toDouble();  // 电阻上限
            tmpSet[s + 0x04*i + OKNGDCRR] = (real*1000 >= std1) ? DataNG : DataOK;
            isOK =  (real*1000 >= std1) ? DataNG : isOK;
            tmpSet[tmpSet.value(AddrDCRR2).toInt() + TEMPDCRA] = isOK;
        }
    }
    double a = tmpSet.value(AddrDCRS3).toInt();  // 跨间电阻参数地址
    double s = tmpSet.value(AddrDCRR3).toInt() + CACHEDCR;  // 跨间电阻结果地址
    if (currItem == AddrDCRS3) {
        for (int i=0; i < tmpDCR.size(); i++) {
            double r = tmpDCR.value(i).toInt();
            double p = tmpPow.value(i).toInt();
            tmpSet[s + 0x04*i + DATADCRR] = r;
            tmpSet[s + 0x04*i + GEARDCRR] = p;
            double real = r * qPow(10, 3-p);
            double std1 = tmpSet[a + SMINDCR3].toDouble();  // 电阻下限
            double std2 = tmpSet[a + SMAXDCR3].toDouble();  // 电阻上限
            tmpSet[s + 0x04*i + OKNGDCRR] = (real >= std2 || real <= std1) ? DataNG : DataOK;
            isOK =  (real >= std2 || real <= std1) ? DataNG : isOK;
            tmpSet[tmpSet.value(AddrDCRR3).toInt() + OKNGDCRA] = isOK;
            qDebug() << i << r << p << std1 << std2 << real;
        }
    }
    // 计算跨间电阻
    if (currItem == AddrDCRS1 && tmpSet[a + ISCHDCR3].toInt() == 1) {
        double s = tmpSet.value(AddrDCRR3).toInt() + CACHEDCR;  // 跨间电阻结果地址
        int quan = tmpDCR.size();
        for (int i=0; i < quan/2; i++) {
            double b1 = 0;
            double b2 = 0;
            for (int j=0; j < quan/2; j++) {
                b1 += (tmpDCR.value((j + i) % quan)).toInt();
                b2 += (tmpDCR.value((j + i + quan/2) % quan)).toInt();
            }
            double r = b1*b2/(b1+b2) * quan / (quan -1);
            double p = tmpPow.value(i).toInt();
            p = (r > 9999) ? p - 1 : p;
            r = (r > 9999) ? r / 10 : r;

            tmpSet[s + 0x04*i + DATADCRR] = int(r);
            tmpSet[s + 0x04*i + GEARDCRR] = int(p);
            double real = r * qPow(10, 3-p);
            double std1 = tmpSet[a + SMINDCR3].toDouble();  // 电阻下限
            double std2 = tmpSet[a + SMAXDCR3].toDouble();  // 电阻上限
            tmpSet[s + 0x04*i + OKNGDCRR] = (real >= std2 || real <= std1) ? DataNG : DataOK;
            isOK =  (real >= std2 || real <= std1) ? DataNG : isOK;
            tmpSet[tmpSet.value(AddrDCRR3).toInt() + OKNGDCRA] = isOK;
            qDebug() << i << r << p << std1 << std2 << real;
        }
    }
    tmpDCR.clear();
    tmpPow.clear();
}

void DevSetCan::renewDCR()
{
    quint32 curr = (currItem - AddrDCRS1 > 3) ? 0 : currItem - AddrDCRS1;  // 测试组
    quint32 addr = tmpSet[AddrDCRR1 + curr].toInt();  // 测试结果存储地址
    for (int i=0; i < 255; i++) {
        tmpDat[addr + i] = tmpSet[addr + i];
    }
    tmpDat.insert(AddrEnum, Qt::Key_News);
    tmpDat.insert(AddrText, currItem);
    emit sendAppMsg(tmpDat);
    tmpDat.clear();
}

void DevSetCan::setupACW()
{
    QByteArray msg;
    for (int i=0; i < 4; i++) {
        int addr = tmpSet[AddrACWS1 + i].toInt();  // 高压配置地址
        int v = tmpSet[addr + AddrACWSV].toInt();  // 电压
        int t = tmpSet[addr + AddrACWST].toInt();  // 时间
        // 03配置;00序号;04绝缘;0001轴;0004线;00频率
        msg = QByteArray::fromHex("0300040001000400");
        msg[1] = i;
        msg[2] = (i == NUMBINRS) ? 0x04 : 0x05;  // 绝缘04,耐压05
        msg[4] = (i == 2) ? 0x02 : 0x01;  // 轴铁,轴线,铁线,轴线 高
        msg[6] = (i == 0) ? 0x02 : 0x04;  // 轴铁,轴线,铁线,轴线 低
        msg[7] = (i == NUMBINRS) ? 0x00 : 0x32;  // 绝缘00Hz,耐压50Hz
        sendDevData(CAN_ID_ACW, msg);
        // 04配置;00序号;01F4电压;000A时间;0000下限
        msg = QByteArray::fromHex("040001F4000A0000");
        msg[1] = i;
        msg[2] = v/256;
        msg[3] = v%256;
        msg[4] = t/256;
        msg[5] = t%256;
        sendDevData(CAN_ID_ACW, msg);
        // 05配置;00序号;0000上限;00电弧;030A缓升缓降
        msg = QByteArray::fromHex("0500000000030A");
        msg[1] = i;
        msg[2] = (i == NUMBINRS) ? 0x00 : 0x09;  // 绝缘上限0,耐压上限2500
        msg[3] = (i == NUMBINRS) ? 0x00 : 0xC4;  // 绝缘上限0,耐压上限2500
        sendDevData(CAN_ID_ACW, msg);
    }
}

void DevSetCan::startACW(QTmpMap map)
{
    int curr = currItem - AddrACWS1;
    QByteArray msg = QByteArray::fromHex("0104001100");  // 01启动;04绝缘;00自动档;11工位;00序号
    msg[1] = (curr == NUMBINRS) ? 0x04 : 0x05;  // 04绝缘/05交耐
    msg[3] = (map.value(AddrData).toInt());
    msg[4] = curr;
    sendDevData(CAN_ID_ACW, msg);
    qDebug() << "acw send:" << msg.toHex();
}

void DevSetCan::parseACW(QByteArray msg)
{
    quint8 cmd = quint8(msg.at(0));
    quint8 num = quint8(msg.at(1));
    int curr = (currItem - AddrACWS1);  // 第几个测试组
    curr = (curr < 0 || curr > 3) ? 0 : curr;  // 限制0~3;
    int addr = tmpSet[AddrACWR1 + curr].toInt();  // 测试结果存储地址
    if (cmd == 0x00) {
        qDebug() << "acw recv:" << msg.toHex();
        tmpSet[addr + STATACWA] = num;  // 修改高压板状态
        renewACW();
    }
    if (cmd == 0x01) {
        int parm = tmpSet[AddrACWS1 + curr].toInt();  // 测试参数存储地址
        double h = tmpSet[parm + AddrACWSH].toDouble();  // 上限
        double l = tmpSet[parm + AddrACWSL].toDouble();  // 下限
        double v = quint8(msg.at(1))*256 + (quint8)msg.at(2);  // 实测电压
        tmpSet[addr + CACHEACW + VOLTACWR] = v;
        double r = quint8(msg.at(3))*256 + (quint8)msg.at(4);  // 实测结果
        tmpSet[addr + CACHEACW + DATAACWR] = r;
        double p = quint8(msg.at(5));  // 小数位数
        tmpSet[addr + CACHEACW + GEARACWR] = p;
        r *= qPow(10, -quint8(msg.at(5)));
        h = (curr == NUMBINRS) ? h : h/100;  // 耐压电流倍数100
        l = (curr == NUMBINRS) ? l : l/100;  // 耐压电流倍数100
        if (((h == 0) && (r > l)) || ((h != 0) && (r > l && r < h))) {
            tmpSet[addr + CACHEACW + OKNGACWR] = DataOK;
        } else {
            tmpSet[addr + CACHEACW + OKNGACWR] = DataNG;
            tmpSet[addr + OKNGACWA] = DataNG;
        }
        tmpSet[addr + STATACWA] = DataTest;  // 修改高压板状态
    }
    if (cmd == 0x08) {  // 版本
        tmpSet[HARDACWA + addr] = quint8(msg.at(6))*256 + quint8(msg.at(7));
        qDebug() << "acw vern:" << msg.toHex();
    }
}

void DevSetCan::renewACW()
{
    int curr = (currItem - AddrACWS1);  // 第几个测试组
    curr = (curr < 0 || curr > 3) ? 0 : curr;  // 限制0~3;
    int addr = tmpSet[AddrACWR1 + curr].toInt();  // 测试结果存储地址
    for (int i=0; i < 8; i++) {
        tmpDat.insert(addr + i, tmpSet.value(addr + i));
    }
    tmpDat.insert(AddrEnum, Qt::Key_News);
    tmpDat.insert(AddrText, currItem);
    emit sendAppMsg(tmpDat);
    tmpDat.clear();
}

void DevSetCan::setupIMP(QTmpMap map)
{
    int addr = tmpSet[AddrModel].toInt();  // 综合配置地址
    int quan = tmpSet[addr + AddrDCRSC].toInt();  // 电枢片数
    int conf = tmpSet[AddrIMPS1].toInt();  // 匝间配置地址
    int time = tmpSet[conf + AddrIMPSA].toInt(); // 间隔(>=1)
    int v = tmpSet[conf + AddrIMPSV].toInt();  // 电压
    int c = tmpSet[conf + AddrIMPST].toInt();  // 次数
    int f = tmpSet[conf + AddrIMPSF].toInt();  // 频率
    f = (map.value(AddrFreq).isNull()) ? f : map.value(AddrFreq).toInt();  // 采样配置
    // 03配置;00序号;0C电枢片数;01间隔;01F4电压;11工位;07频率
    int volt = 1;
    if (v >= 1010) {
        volt = 2;
    }
    if (v >= 2010) {
        volt = 3;
    }
    if (v >= 3010) {
        volt = 4;
    }
    if (v >= 4010) {
        volt = 5;
    }
    QByteArray msg = QByteArray::fromHex("03000C0101F41107");
    msg[2] = quan;
    msg[3] = time;
    msg[4] = v/256;
    msg[5] = v%256;
    msg[6] = (volt << 4) + c;  // 档位+次数
    msg[7] = f;
    sendDevData(CAN_ID_IMP, msg);
    qDebug() << "imp send:" << msg.toHex();
}

void DevSetCan::startIMP(QTmpMap map)
{
    int addr = tmpSet[AddrModel].toInt();  // 综合配置地址
    int quan = tmpSet[addr + AddrDCRSC].toInt();
    int tool = tmpSet[addr + AddrDEVSC].toInt() * quan;  // 夹具针数
    QByteArray msg = QByteArray::fromHex("010011000106");  // 01启动;00序号;11工位;0001序号
    msg[1] = (map.value(AddrFreq).isNull()) ? 0x00 : 0x00;  // 00测试;03采样
    msg[5] = tool;
    sendDevData(CAN_ID_IMP, msg);
    qDebug() << "imp send:" << msg.toHex();
    numb = 0;
}

void DevSetCan::parseIMP(int id, QByteArray msg)
{
    if (id == 0x0081) {
        quint8 cmd = quint8(msg.at(0));
        quint8 num = quint8(msg.at(1));
        int addr = tmpSet[AddrIMPR1].toInt();  // 测试结果存储地址
        if (cmd == 0x00) {
            tmpSet[addr + STATIMPA] = num;
            renewIMP();
            qDebug() << "imp recv:" << msg.toHex();
        }
        if (cmd == 0x02) {
            tmpSet[addr + STATIMPA] = DataTest;
            numb++;
            tmpSet[addr + CACHEIMP + (numb-1)*4 + FREQIMPR] = quint8(msg.at(3));  // 匝间频率
            tmpSet[addr + CACHEIMP + (numb-1)*4 + VOLTIMPR] = quint8(msg.at(4))*256 + (quint8)msg.at(5);
        }
        if (cmd == 0x03) {
            if (num == 0xFF) {  // 波形结束
                judgeIMP();
                renewIMP();
            } else {
                qDebug() << "imp recv:" << msg.toHex();
                numb = num;  // 波形编号
                tmpSet[addr + NUMBIMPA] = numb;
            }
        }
        if (cmd == 0x08) {
            tmpSet[addr + HARDIMPA] = quint8(msg.at(6))*256 + quint8(msg.at(7));
            qDebug() << "imp vern:" << msg.toHex();
        }
    }
    if (id == 0x0481) {
        for (int i=0; i < 8; i++) {
            wave.append(quint8(msg.at(i)));
        }
        //        for (int i=0; i < 4; i++) {
        //            wave.append(quint8(msg.at(i*2+0))*256 + quint8(msg.at(i*2+1)));
        //        }
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
    int save = tmpRow.isEmpty() ? numb : tmpRow.value(numb-1).toInt() + 1;
    int addr = tmpSet[AddrIMPR1].toInt();  // 匝间结果地址
    int conf = tmpSet[AddrIMPS1].toInt();  // 匝间配置地址
    int stdw = tmpSet[AddrIMPSW].toInt();  // 标准波形地址
    tmpSet[addr + NUMBIMPA] = save;
    double c = tmpSet[conf + AddrIMPSL].toDouble();  // 采样点
    double h = tmpSet[conf + AddrIMPSH].toDouble();  // 匝间上限
    int f1 = qMax(1, tmpSet.value(conf + AddrIMPF1).toInt());
    int f2 = qMin(wave.size()-1, tmpSet.value(conf + AddrIMPF2).toInt());
    stdw += (c == 0) ? (save-1)*IMP_SIZE : (c-1)*IMP_SIZE;
    quint32 area1 = 0;
    quint32 area2 = 0;
    quint32 area3 = 0;
    quint32 diff = 0;
    for (int i=f1; i < f2; i++) {
        int a1 = tmpSet[stdw + i].toInt();
        int a2 = wave.at(i);
        area1 += abs(a1-0x200);
        area2 += abs(a2-0x200);
        int b1 = tmpSet[stdw + i - 1].toInt();
        int b2 = tmpSet[stdw + i].toInt();
        int b3 = tmpSet[stdw + i + 1].toInt();
        int c1 = wave.at(i - 1);
        int c2 = wave.at(i);
        int c3 = wave.at(i + 1);
        area3 += abs((b1+b2*2+b3)-(c1+c2*2+c3));
    }
    diff = qMin(area2, area3/4)*100*1000/area1;
    diff = qMin(diff, quint32(99990));
    tmpSet[addr + 4*save + DATAIMPR] = diff;
    tmpSet[addr + CACHEIMP + 4*(save-1) + OKNGIMPR] = diff < h*1000 ? DataOK : DataNG;
    if (diff >= h*1000) {
        tmpSet[addr + OKNGIMPA] = DataNG;
    }
    int s = tmpSet[AddrIMPW1].toInt();  // 测试波形存储地址
    for (int i=0; i < wave.size(); i++) {
        tmpSet[s + i] = wave.at(i);  // 临时存储波形
    }
    wave.clear();
}

void DevSetCan::renewIMP()
{
    int addr = tmpSet[AddrIMPR1].toInt();  // 测试结果存储地址
    for (int i=0; i < numb*4; i++) {
        tmpDat[addr + i*4 + 0] = tmpSet[addr + i*4 + 0];
        tmpDat[addr + i*4 + 1] = tmpSet[addr + i*4 + 1];
        tmpDat[addr + i*4 + 2] = tmpSet[addr + i*4 + 2];
        tmpDat[addr + i*4 + 3] = tmpSet[addr + i*4 + 3];
    }
    int test = tmpSet[AddrIMPW1].toInt();
    for (int i=0; i < IMP_SIZE; i++) {
        tmpDat[test + i] = tmpSet[test + i];
    }
    tmpDat.insert(AddrEnum, Qt::Key_News);
    tmpDat.insert(AddrText, currItem);
    emit sendAppMsg(tmpDat);
    tmpDat.clear();
    //    qDebug() << "imp show:" << tr("%1ms").arg(t.elapsed(), 4, 10, QChar('0'));
}

void DevSetCan::setupPump(QTmpMap msg)
{
    QString tmp = msg.value(AddrText).toString();
    if (msg.value(AddrBeep).isNull() && tmp == "LEDY") {
        int s = msg.value(AddrData).toInt();
        sendDevData(CAN_ID_DCR, QByteArray::fromHex((s == 0x11) ? "0901" : "0904"));
    } else {
        sendDevData(CAN_ID_DCR, QByteArray::fromHex("0900"));
        resetTest(msg);
    }
    qDebug() << "dcr send:" << "pump";
}

void DevSetCan::setupTest(QTmpMap msg)
{
    int c = msg.value(AddrText).toInt();
    switch (c) {
    case AddrModel:  // 进入测试界面下发配置
        sendDevData(CAN_ID_DCR, QByteArray::fromHex("0900"));
        qDebug() << "dcr send:" << "pump";
        wait(200);
        setupACW();
        setupIMP(msg);
        break;
    case AddrDCRS1:  // 片间电阻采样
        sendDevData(CAN_ID_DCR, QByteArray::fromHex("0901"));
        qDebug() << "dcr send:" << "pump";
        wait(200);
        currItem = AddrDCRS1;
        setupDCR(msg);
        startDCR(msg);
        break;
    case AddrDCRS2:
        break;
    case AddrDCRS3:  // 跨间电阻采样
        sendDevData(CAN_ID_DCR, QByteArray::fromHex("0901"));
        qDebug() << "dcr send:" << "pump";
        wait(200);
        currItem = AddrDCRS3;
        setupDCR(msg);
        startDCR(msg);
        break;
    case AddrIMPS1:  // 匝间波形采样
        sendDevData(CAN_ID_DCR, QByteArray::fromHex("0901"));
        qDebug() << "dcr send:" << "pump";
        wait(200);
        currItem = AddrIMPS1;
        setupIMP(msg);
        startIMP(msg);
        break;
    default:
        break;
    }
}

void DevSetCan::startTest(QTmpMap map)
{
    tmpDat.clear();
    tmpDat[DataOKNG] = DataOK;
    currItem = map.value(AddrText).toInt() + AddrModel;
    switch (currItem) {
    case AddrDCRS1:
    case AddrDCRS2:
    case AddrDCRS3:
        setupDCR(map);
        startDCR(map);
        break;
    case AddrACWS1:
    case AddrACWS2:
    case AddrACWS3:
    case AddrACWS4:
        tmpSet[tmpSet.value(AddrACWR1 + currItem - AddrACWS1).toInt() + OKNGIMPA] = DataOK;
        startACW(map);
        break;
    case AddrIMPS1:
        tmpSet[tmpSet.value(AddrIMPR1).toInt() + OKNGIMPA] = DataOK;
        startIMP(map);
        break;
    default:
        break;
    }
}

void DevSetCan::resetTest(QTmpMap msg)
{
    switch (currItem) {
    case AddrDCRS1:
    case AddrDCRS2:
    case AddrDCRS3:
        sendDevData(CAN_ID_DCR, QByteArray::fromHex("02"));
        qDebug() << "dcr stop:" << "02";
        break;
    case AddrACWS1:
    case AddrACWS2:
    case AddrACWS3:
    case AddrACWS4:
        sendDevData(CAN_ID_ACW, QByteArray::fromHex("02"));
        qDebug() << "acw stop:" << "02";
        break;
    case AddrIMPS1:
        sendDevData(CAN_ID_IMP, QByteArray::fromHex("02"));
        qDebug() << "imp stop:" << "02";
        break;
    default:
        sendDevData(CAN_ID_DCR, QByteArray::fromHex("02"));
        sendDevData(CAN_ID_ACW, QByteArray::fromHex("02"));
        sendDevData(CAN_ID_IMP, QByteArray::fromHex("02"));
        qDebug() << "all stop:" << "02" << msg.value(AddrText).toInt();
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
    case Qt::Key_Time:
        getAllDat();
        putAllDat();
        updateAll();
        timeOut++;
        if (timeOut % 500 == 0) {
            if (sender.isEmpty() && recver.isEmpty()) {
                sendDevData(CAN_ID_DCR, QByteArray::fromHex("05"));
            }
        }
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
