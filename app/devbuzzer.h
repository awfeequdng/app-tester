/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       蜂鸣器设置
*******************************************************************************/
#ifndef DEVBUZZER_H
#define DEVBUZZER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <QDebug>
#include <QObject>
#include <QVariant>

#include "devdriver.h"

class DevBuzzer : public QObject
{
    Q_OBJECT
public:
    explicit DevBuzzer(QObject *parent = 0);
signals:
    void sendAppMap(QVariantMap msg);
private slots:
    void initBuzzer();
    void sendBuzzer(double duty);
    void recvAppMap(QVariantMap msg);
private:
    int fd;
};

#endif // DEVBUZZER_H
