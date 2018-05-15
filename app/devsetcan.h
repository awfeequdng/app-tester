/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       CAN总线设置
*******************************************************************************/
#ifndef DEVSETCAN_H
#define DEVSETCAN_H

#include <qmath.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <linux/netlink.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/in.h>
#include <net/if.h>
#include <fcntl.h>
#include <sys/un.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>

#include <QDebug>
#include <QQueue>
#include <QObject>
#include <QVariant>
#include <QElapsedTimer>

#include "appdefine.h"
#include "typconfig.h"
#include "typsetdcr.h"
#include "typsetacw.h"
#include "typsetimp.h"





const int AddrACWJ = 0x00;  // 耐压判定
const int AddrACWU = 0x01;  // 耐压电压
const int AddrACWR = 0x02;  // 耐压结果
const int AddrACWP = 0x03;  // 耐压小数

const int AddrIMPV = 1140;  // 匝间版本
const int AddrIMPS = 1141;  // 匝间状态
const int AddrIMPQ = 1142;  // 匝间编号
const int AddrIMPJ = 1143;  // 匝间判定
const int AddrIMPU = 1144;  // 匝间电压
const int AddrIMPG = 1145;  // 档位
const int AddrIMPF = 1146;  // 频率

class DevSetCan : public QObject
{
    Q_OBJECT
public:
    explicit DevSetCan(QObject *parent = 0);
signals:
    void sendAppMsg(QTmpMap msg);
private slots:
    void initDevPort();
    bool readDevPort();
    bool sendDevPort(can_frame can);
    void initDevStat();
    void sendDevData(int id, QByteArray msg);
    void readDevData(int id, QByteArray msg);
    void getAllDat();
    void putAllDat();
    void updateAll();
    void configDCR();
    void selectDCR();
    void updateDCR(QByteArray msg);
    void sampleImp();
    void configImp();
    void selectImp();
    void updateImp(int id, QByteArray msg);
    void calc();
    void configAcw();
    void selectAcw();
    void updateAcw(QByteArray msg);
    void updateDat();
    void startPlay(QTmpMap msg);
    void recvAppMsg(QTmpMap msg);
private:
    int fd;
    can_frame frame;
    QQueue<QTmpMap> sender;
    QQueue<QTmpMap> recver;
    int currItem;
    QList<int>wave;
    QElapsedTimer t;
    QTmpMap tmpSet;
    QTmpMap tmpMsg;
    int timeOut;
};

#endif // DEVSETCAN_H
