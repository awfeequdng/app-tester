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
        QString tmp = tr("屏幕打开失败");
        qDebug() << "lcd show:" << tmp;
    }
}

void DevScreen::sendScreen(double duty)
{
    struct pwm_config_info conf;

    conf.freq = 20000;
    conf.duty = duty;
    conf.polarity = 0;
    conf.count = 0;

    if (duty == 0)
        memset(&conf, 0, sizeof(struct pwm_config_info));

    write(fd, &conf, sizeof(struct pwm_config_info));
}

void DevScreen::recvAppMap(QVariantMap msg)
{
    switch (msg.value("enum").toInt()) {
    case Qt::Key_BrightnessAdjust:
        if (!msg.value("data").isNull()) {
            sendScreen(msg.value("data").toDouble());
        }
        break;
    default:
        break;
    }
}

