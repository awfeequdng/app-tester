/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       1302设置
*******************************************************************************/
#ifndef DEVDS1302_H
#define DEVDS1302_H

#include <QObject>
#include <QDateTime>

#include "main.h"

/*
SlaveAddr： 要操作的I2C器件的设备物理地址
RegAddr：   要操作的设备的存贮器地址
pInData：   要写入/读出的数据
BUFFiDLen： 要写入/读出的数据的字节长度
*/

typedef struct {
    unsigned char SlaveAddr;
    unsigned int RegAddr;
    unsigned char *pDataBuff;
    int iDLen;
} I2CParameter, *pI2CParameter;

class DevDS1302 : public QObject
{
    Q_OBJECT
public:
    explicit DevDS1302(QObject *parent = 0);
signals:
    void sendAppMsg(QTmpMap msg);
public slots:
    int I2COpen();
    int GPIO_OutEnable(unsigned int dwEnBits);
    int GPIO_OutDisable(unsigned int dwDisBits);
    int GPIO_OutSet(unsigned int dwSetBits);
    int GPIO_OutClear(unsigned int dwClearBits);
    int GPIO_PinState(unsigned int* dwClearBits);
    void DS1302ByteWrite(unsigned char dat);
    unsigned char DS1302ByteRead();
    void DS1302SingleWrite(unsigned char reg, unsigned char dat);
    unsigned char DS1302SingleRead(unsigned char reg);
    void readDevIIC();
    void saveDevIIC(QDateTime currDate);
    void recvAppMsg(QTmpMap msg);
private:
    int  Gpio_fd;
    QTmpMap tmpMsg;
};

#endif // DEVDS1302_H
