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

#include <QString>

const int AddrVerNub = 0x0000;  // 版本信息
const int AddrBackup = 0x0010;  // 后台信息
const int AddrNumber = 0x0011;  // 产品编号
const int AddrLogger = 0x001F;  // 调试信息
const int AddrSystem = 0x0020;  // 系统参数
const int AddrTimeOK = 0x0027;
const int AddrTimeNG = 0x0028;
const int AddrSignIn = 0x0040;  // 登录参数
const int AddrConfig = 0x0050;  // 当前型号
const int AddrMaster = 0x0100;  // 用户信息
const int AddrAction = 0x0200;  // 权限信息
const int AddrModels = 0x0300;  // 型号信息

const int AddrSC = 0x0400;  // 下发配置
const int AddrTM = 0x00;  // 型号
const int AddrTC = 0x01;  // 片数
const int AddrTD = 0x02;  // 夹具
const int AddrTP = 0x03;  // 批次
const int AddrTT = 0x04;  // 类型
const int AddrAG = 0X0470;  // 绝缘参数Check
const int AddrAC = 0X0480;  // 轴铁参数Check
const int AddrAL = 0X0490;  // 轴线参数Check
const int AddrLC = 0X04A0;  // 线铁参数Check
const int AddrHC = 0x00;  // 测试
const int AddrHN = 0x01;  // 名称
const int AddrHV = 0x02;  // 电压
const int AddrHT = 0x03;  // 时间
const int AddrHH = 0x04;  // 上限
const int AddrHL = 0x05;  // 下限
const int AddrHO = 0x06;  // 补偿
const int AddrHA = 0x07;  // 电弧
const int AddrHU = 0x08;  // 实测电压
const int AddrHR = 0x09;  // 实测结果
const int AddrHD = 0x0A;  // 小数位数
const int AddrHS = 0x0B;  // 实时状态
const int AddrHJ = 0x0F;  // 结果判定

#endif // APPDEFINE

