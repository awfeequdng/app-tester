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

const int DataOK = 0x00;
const int DataNG = 0x01;

const int DataFree = 0x00;
const int DataTest = 0x01;

const int AddrEnum = 0x00;  // 命令
const int AddrText = 0x01;  // 文本
const int AddrRate = 0x02;  // 比率
const int AddrBeep = 0x03;  // 音量
const int AddrData = 0x04;  // 数据
const int AddrFreq = 0x05;  // 频率

const int AddrDevA = 1000;  // 设备信息区地址,默认1100
const int AddrWeld = 1001;  // 片间结果区地址,默认1200
const int AddrChip = 1002;
const int AddrDiag = 1003;
const int AddrINRA = 1004;  // 绝缘结果区地址,默认1150
const int AddrACWL = 1005;  // 轴铁结果区地址,默认1155
const int AddrACWC = 1006;  // 轴线结果区地址,默认1160
const int AddrACWA = 1007;  // 铁线结果区地址,默认1165
const int AddrIMPA = 1008;  // 匝间结果区地址,默认1400
const int AddrIMPW = 1009;  // 匝间波形区地址,默认1600
const int AddrBack = 1010;  // 后台信息区地址,默认2020
const int AddrDCRB = 1011;  // 电阻后台区地址,默认2040
const int AddrINRB = 1012;
const int AddrACWB = 1013;
const int AddrIMPB = 1014;
const int AddrSyst = 1015;
const int AddrInfo = 1016;
const int AddrLoad = 1017;
const int AddrType = 1018;
const int AddrUser = 1019;
const int AddrShow = 1020;

const int AddrModel = 1050;  // 综合配置地址
const int AddrDCRS1 = 1051;  // 片机配置地址
const int AddrDCRS2 = 1052;  // 焊接配置地址
const int AddrDCRS3 = 1053;  // 跨间配置地址
const int AddrACWS1 = 1054;  // 绝缘配置地址
const int AddrACWS2 = 1055;  // 轴铁配置地址
const int AddrACWS3 = 1056;  // 轴线配置地址
const int AddrACWS4 = 1057;  // 铁线配置地址
const int AddrIMPS1 = 1058;  // 匝间配置地址
const int AddrIMPSW = 1059;  // 标准波形地址
const int AddrDCRSW = 1060;  // 片间电阻标准

const int DataSoft = 1110;  // 软件版本
const int DataStat = 1111;  // 软件状态
const int DataTime = 1112;  // 开机时间
const int DataSign = 1113;  // 登录状态
const int DataPlay = 1114;  // 启动时间
const int DataStop = 1115;  // 完成时间
const int DataCode = 1116;  // 条码状态
const int DataTemp = 1117;  // 当前温度
const int DataOKNG = 1118;  // 测试判定

const int DataDCRV = 1120;  // 电阻版本
const int DataDCRS = 1121;  // 电阻状态
const int DataDCRJ = 1123;  // 电阻判定

const int DataACWV = 1130;  // 耐压版本
const int DataACWS = 1131;  // 耐压状态

const int DataIMPV = 1140;  // 匝间版本
const int DataIMPS = 1141;  // 匝间状态
const int DataIMPQ = 1142;  // 匝间编号
const int DataIMPJ = 1143;  // 匝间判定
const int DataIMPU = 1144;  // 匝间电压
const int DataIMPG = 1145;  // 档位
const int DataIMPF = 1146;  // 频率

const int DataFile = 2000;  // 当前型号
const int DataUser = 2001;  // 当前用户
const int DataAuto = 2002;  // 自动保存
const int DataSave = 2003;  // 日志输出

#endif // APPDEFINE

