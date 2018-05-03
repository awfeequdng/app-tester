/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       蜂鸣器设置
*******************************************************************************/
#include "devbuzzer.h"

DevBuzzer::DevBuzzer(QObject *parent) : QObject(parent)
{
    initBuzzer();
}

void DevBuzzer::initBuzzer()
{
    fd = open("/dev/em335x_pwm2", O_RDWR);  // 打开蜂鸣器
    if (fd < 0) {
        QString tmp = tr("蜂鸣器打开失败");
        qDebug() << "pwm show:" << tmp;
    }
}

void DevBuzzer::sendBuzzer(double duty)
{
    if (fd < 0) {
        QString tmp = tr("蜂鸣器打开失败");
        qDebug() << "pwm show:" << tmp;
        return;
    }

    struct pwm_config_info conf;
    conf.freq = 2000;
    conf.duty = duty;
    conf.polarity = 1;
    conf.count = 0;

    if (duty == 0)
        memset(&conf, 0, sizeof(struct pwm_config_info));

    write(fd, &conf, sizeof(struct pwm_config_info));
}

void DevBuzzer::recvAppMap(QVariantMap msg)
{
    switch (msg.value("enum").toInt()) {
    case Qt::Key_View:
        if (!msg.value("data").isNull()) {
            sendBuzzer(msg.value("data").toDouble());
        }
        break;
    default:
        break;
    }
}

