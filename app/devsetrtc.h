/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       RTC设置
*******************************************************************************/
#ifndef DEVSETRTC_H
#define DEVSETRTC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <sys/ioctl.h>
#include <linux/rtc.h>
#include <linux/ioctl.h>

#include <QDebug>
#include <QObject>
#include <QVariant>
#include <QDateTime>

class DevSetRtc : public QObject
{
    Q_OBJECT
public:
    explicit DevSetRtc(QObject *parent = 0);
signals:
    void sendAppMap(QVariantMap msg);
private slots:
    void initDevRtc();
    void saveDevRtc(QDateTime t);
    void recvAppMap(QVariantMap msg);
private:
    int fd;
};

#endif // DEVSETRTC_H
