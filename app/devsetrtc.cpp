/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       RTC设置
*******************************************************************************/
#include "devsetrtc.h"

DevSetRtc::DevSetRtc(QObject *parent) : QObject(parent)
{
    initDevRtc();
}

void DevSetRtc::initDevRtc()
{
    fd = open("/dev/rtc0", O_RDWR, 0);      // 打开RTC
    if (fd < 0) {
        QString tmp = tr("RTC打开失败");
        qDebug() << "dev show:" << tmp;
    }
}

void DevSetRtc::saveDevRtc(QDateTime t)
{
    if (fd < 0) {
        QString tmp = tr("RTC打开失败");
        qDebug() << "dev show:" << tmp;
        return;
    }

    rtc_time rt;
    rt.tm_year = t.date().year() - 1900;  // 年从1900开始
    rt.tm_mon  = t.date().month() -1;  // 月从0开始
    rt.tm_mday = t.date().day();
    rt.tm_hour = t.time().hour();
    rt.tm_min  = t.time().minute();;
    rt.tm_sec  = t.time().second();

    time_t t1 = timelocal(reinterpret_cast<tm*>(&rt));  // 设置系统时间
    stime(&t1);

    int ret = ioctl(fd, RTC_SET_TIME, &rt); // 设置RTC时间
    if (ret < 0) {
        QString tmp = tr("RTC设置失败");
        qDebug() << "dev show:" << tmp;
    }
}

void DevSetRtc::recvAppMsg(QTmpMap msg)
{
    int c = msg.value(AddrEnum).toInt();
    switch (c) {
    case Qt::Key_Time:
        if (msg.value(AddrText).toString() == "time")
            saveDevRtc(msg.value(AddrData).toDateTime());
        break;
    default:
        break;
    }
}

