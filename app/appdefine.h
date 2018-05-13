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

typedef QMap<int, QVariant> QTmpMap;

struct pwm_config_info {
    unsigned int freq;
    unsigned int duty;
    unsigned int polarity;
    unsigned int count;
};

const int AddrEnum = 0x00;  // 命令
const int AddrText = 0x01;  // 文本
const int AddrRate = 0x02;  // 比率
const int AddrBeep = 0x03;  // 音量
const int AddrData = 0x04;  // 数据

const int AddrUser = 1019;

const int AddrSoft = 1110;  // 软件版本
const int AddrStat = 1111;  // 软件状态
const int AddrTime = 1112;  // 开机时间
const int AddrSign = 1113;  // 登录状态
const int AddrPlay = 1114;  // 启动时间
const int AddrStop = 1115;  // 完成时间
const int AddrCode = 1116;  // 条码状态

const int AddrFile = 2000;
const int AddrCurr = 2001;
const int AddrAuto = 2002;

const int AddrBackup = 0x0010;  // 后台信息
const int AddrNumber = 0x0011;  // 产品编号
const int AddrLogger = 0x001F;  // 调试信息
const int AddrSystem = 0x0020;  // 系统参数
const int AddrTimeOK = 0x0027;
const int AddrTimeNG = 0x0028;

const int AddrSignIn = 0x0040;  // 登录参数

const int AddrTpName = 0x0050;  // 当前型号

const int AddrModels = 0x0300;  // 型号信息

#endif // APPDEFINE

