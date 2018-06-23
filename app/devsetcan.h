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
#include <QCoreApplication>

#include "main.h"
#include "typconfig.h"
#include "typsetdcr.h"
#include "typsetacw.h"
#include "typsetimp.h"

const int CAN_ID_DCR = 0x0022;
const int CAN_ID_ACW = 0x0023;
const int CAN_ID_IMP = 0x0024;
const int CAN_ID_WAVE = 0x0481;

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
    void updateDat();
    void setupDCR(QTmpMap map);
    void startDCR(QTmpMap map);
    void parseDCR(QByteArray msg);
    void orderDCR();
    void judgeDCR();
    void renewDCR();
    void setupACW();
    void startACW(QTmpMap map);
    void parseACW(QByteArray msg);
    void renewACW();
    void setupIMP(QTmpMap map);
    void startIMP(QTmpMap map);
    void parseIMP(int id, QByteArray msg);
    void judgeIMP();
    void renewIMP();
    void setupPump(QTmpMap msg);
    void setupTest(QTmpMap msg);
    void startTest(QTmpMap msg);
    void resetTest(QTmpMap msg);
    void recvAppMsg(QTmpMap msg);
    void wait(int ms);
private:
    int fd;
    can_frame frame;
    QQueue<QTmpMap> sender;
    QQueue<QTmpMap> recver;
    int currItem;
    QList<int>wave;
    QElapsedTimer t;
    QTmpMap tmpSet;
    QTmpMap tmpDat;
    QTmpMap tmpMsg;
    int timeOut;
    int numb;
    QTmpMap tmpDCR;
    QTmpMap tmpPow;
    QTmpMap tmpRow;
    int currTemp;
    quint8 tmpio;
};

#endif // DEVSETCAN_H
