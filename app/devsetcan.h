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
#include <QObject>
#include <QVariant>

class DevSetCan : public QObject
{
    Q_OBJECT
public:
    explicit DevSetCan(QObject *parent = 0);
signals:
    void sendAppMap(QVariantMap msg);
private slots:
    void initDevPort();
    bool readDevPort();
    bool sendDevPort(can_frame can);
    void recvAppMap(QVariantMap msg);
private:
    int fd;
    can_frame frame;
};

#endif // DEVSETCAN_H
