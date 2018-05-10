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
const int AddrSkewSU = 0x0000;
const int AddrSkewSP = 0x0001;
const int AddrSkewSS = 0x0002;
const int AddrSkewSA = 0x0003;

const int AddrTpName = 0x0050;  // 当前型号

const int AddrMaster = 0x0100;  // 用户信息
const int AddrSkewMU = 0x0000;
const int AddrSkewMP = 0x0001;
const int AddrSkewMG = 0x0002;
const int AddrSkewMT = 0x0003;

const int AddrAction = 0x0200;  // 权限信息
const int AddrModels = 0x0300;  // 型号信息

const int AddrConfig = 0x0400;  // 下发配置
const int AddrSkewTM = 0x0000;  // 型号
const int AddrSkewTC = 0x0001;  // 片数
const int AddrSkewTD = 0x0002;  // 夹具
const int AddrSkewTP = 0x0003;  // 批次
const int AddrSkewTT = 0x0004;  // 类型

const int AddrHighAG = 0x0470;  // 绝缘参数Check
const int AddrHighAC = 0x0480;  // 轴铁参数Check
const int AddrHighAL = 0x0490;  // 轴线参数Check
const int AddrHighLC = 0x04A0;  // 线铁参数Check
const int AddrSkewHC = 0x0000;  // 测试
const int AddrSkewHN = 0x0001;  // 名称
const int AddrSkewHV = 0x0002;  // 电压
const int AddrSkewHT = 0x0003;  // 时间
const int AddrSkewHH = 0x0004;  // 上限
const int AddrSkewHL = 0x0005;  // 下限
const int AddrSkewHO = 0x0006;  // 补偿
const int AddrSkewHA = 0x0007;  // 电弧
const int AddrSkewHU = 0x0008;  // 实测电压
const int AddrSkewHR = 0x0009;  // 实测结果
const int AddrSkewHD = 0x000A;  // 小数位数
const int AddrSkewHS = 0x000B;  // 实时状态
const int AddrSkewHJ = 0x000F;  // 结果判定

const int AddrSetIMP = 0x04B0;  // 匝间参数Check
const int AddrSkewIC = 0x0000;  // 测试
const int AddrSkewIN = 0x0001;  // 名称
const int AddrSkewIV = 0x0002;  // 电压
const int AddrSkewIT = 0x0003;  // 次数
const int AddrSkewIH = 0x0004;  // 上限
const int AddrSkewIL = 0x0005;  // 下限
const int AddrSkewIO = 0x0006;  // 补偿
const int AddrSkewIB = 0x0007;  // 间隔
const int AddrSkewIU = 0x0008;  // 实测电压
const int AddrSkewIR = 0x0009;  // 实测结果
const int AddrSkewID = 0x000A;  // 小数位数
const int AddrSkewIS = 0x000B;  // 实时状态
const int AddrSkewIG = 0x000C;  // 档位
const int AddrSkewIF = 0x000D;  // 频率
const int AddrSkewIJ = 0x000F;  // 结果判定

const int AddrWaveTP = 0x0C00;  // 临时波形

#endif // APPDEFINE

