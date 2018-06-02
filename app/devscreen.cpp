/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       屏幕亮度设置
*******************************************************************************/
#include "devscreen.h"

DevScreen::DevScreen(QObject *parent) : QObject(parent)
{
    initScreen();
}

void DevScreen::initScreen()
{
    fd = open("/dev/em335x_pwm1", O_RDWR);  // 打开蜂鸣器
    if (fd < 0) {
        qWarning() << "pwm show:" << "open fail";
    }
}

void DevScreen::sendScreen(double duty)
{
    QList<double> rates;
    rates << 1 << 2 << 3 << 5 << 8 << 9 << 10 << 20 << 30 << 100;
    duty = qMin(rates.size()-1.00, duty);

    struct pwm_config_info conf;

    conf.freq = 20000;
    conf.duty = rates.at(duty);
    conf.polarity = 0;
    conf.count = 0;

    if (duty == 0)
        memset(&conf, 0, sizeof(struct pwm_config_info));

    int ret = write(fd, &conf, sizeof(struct pwm_config_info));
    if (ret < 0) {
        qWarning() << "pwm show:" << "send fail";
    }
}

void DevScreen::recvAppMsg(QTmpMap msg)
{
    int c = msg.value(AddrEnum).toInt();
    switch (c) {
    case Qt::Key_Copy:
        sendScreen(msg[msg[AddrSyst].toInt() + 3].toInt());
        break;
    default:
        break;
    }
}

