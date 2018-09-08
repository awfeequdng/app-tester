/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       1302设置
*******************************************************************************/
#include "devds1302.h"
#include <stdio.h>
#include <linux/types.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <assert.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <linux/rtc.h>
#include <QDebug>

#define GPIO20 (1 << 20)
#define GPIO21 (1 << 21)
#define GPIO22 (1 << 22)
#define GPIO23 (1 << 23)
#define GPIO24 (1 << 24)
#define GPIO25 (1 << 25)
#define GPIO26 (1 << 26)
#define GPIO27 (1 << 27)
#define GPIO28 (1 << 28)
#define GPIO29 (1 << 29)

#define EM335X_GPIO_OUTPUT_ENABLE 0
#define EM335X_GPIO_OUTPUT_DISABLE 1
#define EM335X_GPIO_OUTPUT_SET 2
#define EM335X_GPIO_OUTPUT_CLEAR 3
#define EM335X_GPIO_INPUT_STATE 5

/*
* Emlinix JUN-2-2010: double input parameters can be used in more than one driver
*/
struct double_pars {
    unsigned int par1;
    unsigned int par2;
};


DevDS1302::DevDS1302(QObject *parent) : QObject(parent)
{
    I2COpen();
}

int DevDS1302::I2COpen()
{
    Gpio_fd = open("/dev/em335x_gpio", O_RDWR);
    GPIO_OutEnable(GPIO23);
    GPIO_OutEnable(GPIO27);
    GPIO_OutEnable(GPIO26);

    GPIO_OutClear(GPIO23); // CE
    GPIO_OutClear(GPIO27); // 初始化 DS1302 通信引脚

    qDebug() << "iic open:" << Gpio_fd;

    return Gpio_fd;
}

int DevDS1302::GPIO_PinState(unsigned int* pPinState)
{
    int rc;
    struct double_pars dpars;
    dpars.par1 = EM335X_GPIO_INPUT_STATE;  // 5
    dpars.par2 = *pPinState;
    rc = read(Gpio_fd, &dpars, sizeof(struct double_pars));
    if (!rc) {
        *pPinState = dpars.par2;
    }
    return rc;
}

int DevDS1302::GPIO_OutEnable(unsigned int dwEnBits)
{
    int                 rc;
    struct double_pars  dpars;

    dpars.par1 = EM335X_GPIO_OUTPUT_ENABLE; // 0
    dpars.par2 = dwEnBits;

    rc = write(Gpio_fd, &dpars, sizeof(struct double_pars));
    return rc;
}

int DevDS1302::GPIO_OutDisable(unsigned int dwDisBits)
{
    int                 rc;
    struct double_pars  dpars;

    dpars.par1 = EM335X_GPIO_OUTPUT_DISABLE; // 1
    dpars.par2 = dwDisBits;

    rc = write(Gpio_fd, &dpars, sizeof(struct double_pars));
    return rc;
}
int DevDS1302::GPIO_OutSet(unsigned int dwSetBits)
{
    int                 rc;
    struct double_pars  dpars;

    dpars.par1 = EM335X_GPIO_OUTPUT_SET;   // 2
    dpars.par2 = dwSetBits;

    rc = write(Gpio_fd, &dpars, sizeof(struct double_pars));
    return rc;
}
int DevDS1302::GPIO_OutClear(unsigned int dwClearBits)
{
    int                 rc;
    struct double_pars  dpars;

    dpars.par1 = EM335X_GPIO_OUTPUT_CLEAR;  // 3
    dpars.par2 = dwClearBits;

    rc = write(Gpio_fd, &dpars, sizeof(struct double_pars));
    return rc;
}

void DevDS1302::DS1302ByteWrite(unsigned char dat)
{
    unsigned char a_dat = 0;
    unsigned char mask = 0;

    a_dat = 0;
    a_dat = 0;
    a_dat = 0;
    for (mask=0x01; mask != 0; mask <<= 1) {
        if ((mask & dat) != 0) {
            GPIO_OutSet(GPIO26);
        } else {
            GPIO_OutClear(GPIO26);
        }
        GPIO_OutSet(GPIO27);
        GPIO_OutClear(GPIO27);
    }
    GPIO_OutSet(GPIO26);
}

unsigned char DevDS1302::DS1302ByteRead()
{
    int y = 0;
    unsigned char mask;
    unsigned char dat_read = 0;
    unsigned int dwPinState;
    dwPinState = GPIO26;
    for (y=0; y < 8; y++) {
        mask = (1 << (y));
        dwPinState = GPIO26;
        GPIO_PinState(&dwPinState);
        if (dwPinState == (1 << 26)) {
            dat_read |= mask;
        } else {
            //
        }
        if (y == 7) {
            break;
        }
        GPIO_OutSet(GPIO27);
        GPIO_OutClear(GPIO27);
    }
    return dat_read;
}

void DevDS1302::DS1302SingleWrite(unsigned char reg, unsigned char dat)
{
    GPIO_OutSet(GPIO23);
    usleep(1);
    GPIO_OutEnable(GPIO26);
    DS1302ByteWrite((reg << 1) | 0x80);
    DS1302ByteWrite(dat);
    usleep(1);
    GPIO_OutClear(GPIO23);
}

unsigned char DevDS1302::DS1302SingleRead(unsigned char reg)
{
    unsigned char dat;
    GPIO_OutSet(GPIO23);
    usleep(1);
    GPIO_OutEnable(GPIO26);
    DS1302ByteWrite((reg << 1) | 0x81);
    GPIO_OutDisable(GPIO26);
    dat = DS1302ByteRead();
    usleep(1);
    GPIO_OutClear(GPIO23);
    return dat;
}

void DevDS1302::readDevIIC()
{
    QList<int> reals;
    for (int i=0; i < 7; i++)
        reals << (QString::number(DS1302SingleRead(i), 16)).toInt();
    QTime readTime(reals.at(2), reals.at(1), reals.at(0));
    QDate readDate(reals.at(6)+2000, reals.at(4), reals.at(3));

    QDateTime currDate(readDate, readTime);
    qDebug() << "iic read:" << currDate;
    if (currDate.isValid()) {
        tmpMsg.insert(Qt::Key_0, Qt::Key_Xfer);
        tmpMsg.insert(Qt::Key_1, "ds1302");
        tmpMsg.insert(Qt::Key_2, currDate);
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
    }
}

void DevDS1302::saveDevIIC(QDateTime currDate)
{
    bool ok;
    QDate saveDate = currDate.date();
    QTime saveTime = currDate.time();
    int year = saveDate.year()%100;
    QList<int> tt;
    tt << QString::number(saveTime.second()).toInt(&ok, 16);
    tt << QString::number(saveTime.minute()).toInt(&ok, 16);
    tt << QString::number(saveTime.hour()).toInt(&ok, 16);
    tt << QString::number(saveDate.day()).toInt(&ok, 16);
    tt << QString::number(saveDate.month()).toInt(&ok, 16);
    tt << QString::number(saveDate.dayOfWeek()).toInt(&ok, 16);
    tt << QString::number(year).toInt(&ok, 16);

    DS1302SingleWrite(7, 0x00);  // 撤销写保护以允许写入数据
    for (int i=0; i < 7; i++) {  // 设置 DS1302 为默认的初始时间
        DS1302SingleWrite(i, tt[i]);
    }
    qDebug() << "iic save:" << currDate;
}

void DevDS1302::recvAppMsg(QTmpMap msg)
{
    switch (msg.value(Qt::Key_0).toInt()) {
    case Qt::Key_Xfer:
        if (msg.value(Qt::Key_1).toString() == "system")
            saveDevIIC(msg.value(Qt::Key_2).toDateTime());
        break;
    case Qt::Key_Game:
        readDevIIC();
        break;
    default:
        break;
    }
}
