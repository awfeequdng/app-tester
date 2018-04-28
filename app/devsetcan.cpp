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
    initDevPort();
}

void DevSetCan::initDevPort()
{
    int ret = 0;

    fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);  //打开套接字
    if (fd < 0) {
        QString tmp = tr("CAN口打开失败");
        qDebug() << "dev show:" << tmp;
        return;
    }

    system("ifconfig can0 down");
    system("ip link set can0 type can bitrate 500000 restart-ms 100");
    system("ifconfig can0 up");

    struct ifreq ifr;
    strcpy(ifr.ifr_name, "can0");
    ret = ioctl(fd, SIOCGIFINDEX, &ifr);
    if (ret < 0) {
        QString tmp = tr("CAN口设置失败");
        qDebug() << "dev show:" << tmp;
        return;
    }

    struct sockaddr_can addr;
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    ret = bind(fd, (struct sockaddr *)&addr, sizeof(addr));
    if (ret < 0) {
        QString tmp = tr("CAN口绑定失败");
        qDebug() << "dev show:" << tmp;
        return;
    }
}

bool DevSetCan::readDevPort()
{
    if (fd <= 0)
        return false;
    int nbytes;
    int ret;
    fd_set rdfds;
    struct timeval tv;

    FD_ZERO(&rdfds);
    FD_SET(fd, &rdfds);
    tv.tv_sec  = 0;                                         // 0秒
    tv.tv_usec = 0;                                         // 0微秒
    ret = select(fd+1, &rdfds, NULL, NULL, &tv);            // 监听是否可读
    if (ret < 0)
        return false;
    if (FD_ISSET(fd, &rdfds)) {
        nbytes = read(fd, &frame, sizeof(struct can_frame));  // 读取缓冲区
        return true;
    }
    return false;
}

bool DevSetCan::sendDevPort(can_frame can)
{
    if (fd <= 0)
        return false;
    int nbytes;
    int ret;
    fd_set rdfds;
    struct timeval tv;

    FD_ZERO(&rdfds);
    FD_SET(fd, &rdfds);
    tv.tv_sec  = 0;                                         // 0秒
    tv.tv_usec = 0;                                         // 0微秒
    ret = select(fd+1, NULL, &rdfds, NULL, &tv);            // 监听是否可写
    if (ret < 0)
        return false;
    if (FD_ISSET(fd, &rdfds)) {
        nbytes = write(fd, &can, sizeof(struct can_frame));
        if (nbytes != sizeof(struct can_frame)) {
            perror("can raw socket write error");
            return false;
        }
        return true;
    }
    return false;
}

void DevSetCan::recvAppMap(QVariantMap msg)
{
    switch (msg.value("enum").toInt()) {
    case Qt::Key_Send:
        if (!msg.value("data").isNull()) {
            break;
        }
        break;
    default:
        break;
    }
}

