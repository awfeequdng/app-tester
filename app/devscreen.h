/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       屏幕亮度设置
*******************************************************************************/
#ifndef DEVSCREEN_H
#define DEVSCREEN_H

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

class DevScreen : public QObject
{
    Q_OBJECT
public:
    explicit DevScreen(QObject *parent = 0);
signals:
    void sendAppMap(QVariantMap msg);
private slots:
    void initScreen();
    void sendScreen(double duty);
    void recvAppMap(QVariantMap msg);
private:
    int fd;
};

#endif // DEVSCREEN_H
