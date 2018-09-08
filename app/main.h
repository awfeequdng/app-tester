/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       宏定义
*******************************************************************************/
#ifndef APPDEFINE
#define APPDEFINE

#include <QMap>
#include <QString>
#include <QVariant>
#include <QMetaType>

typedef QMap<int, QVariant> QTmpMap;

struct pwm_config_info {
    unsigned int freq;
    unsigned int duty;
    unsigned int polarity;
    unsigned int count;
};

#define WORKL 0x11
#define WORKR 0x14

#define MODEB 0x00
#define MODES 0x01
#define MODEF 0x02
#define MODEN 0x03

const int DataFree = 0x00;
const int DataTest = 0x01;

const int DataFile = 22000;  // 当前型号
const int DataUser = 22001;  // 当前用户
const int DataAuto = 22002;  // 自动保存
const int DataSave = 22003;  // 日志输出
const int DataType = 22004;  // 当前型号
const int DataSoft = 22015;  // 软件版本

#endif // APPDEFINE

