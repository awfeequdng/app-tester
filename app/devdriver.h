/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       PWM常用参数
*******************************************************************************/
#ifndef DEVDRIVER
#define DEVDRIVER

struct pwm_config_info {
    unsigned int freq;
    unsigned int duty;
    unsigned int polarity;
    unsigned int count;
};

#endif // DEVDRIVER

