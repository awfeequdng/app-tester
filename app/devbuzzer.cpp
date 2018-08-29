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
        qWarning() << "pwm beep:" << "open fail";
    }
}

void DevBuzzer::sendBuzzer(double duty)
{
    struct pwm_config_info conf;
    conf.freq = 2000;
    conf.duty = duty;
    conf.polarity = 1;
    conf.count = 0;

    if (duty == 0)
        memset(&conf, 0, sizeof(struct pwm_config_info));

    int ret = write(fd, &conf, sizeof(struct pwm_config_info));
    if (ret < 0) {
        qWarning() << "pwm beep:" << "send fail";
    }
}

void DevBuzzer::recvAppMsg(QTmpMap msg)
{
    switch (msg.value(Qt::Key_0).toInt()) {
    case Qt::Key_Call:
        if (!msg.value(Qt::Key_3).isNull())
            sendBuzzer(msg.value(Qt::Key_3).toDouble());
        break;
    default:
        break;
    }
}

