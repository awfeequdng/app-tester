/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       数据初始化
*******************************************************************************/
#include "sqlcreate.h"

SqlCreate::SqlCreate(QObject *parent) : QObject(parent)
{
}

void SqlCreate::initSqlDir()
{
    QDir dir;
    bool ok = !dir.exists("nandflash") ? dir.mkdir("nandflash") : true;
    if (ok) {
        initTmpDat();
        openRecord(dir.exists("./nandflash/record.db"));
        openSystem(dir.exists("./nandflash/system.db"));
        openConfig(dir.exists("./nandflash/config.db"));
    }
}

void SqlCreate::initTmpDat()
{
    tmpSet.insert(AddrDevA, 1100);  // 设备贪睡存放在1100
    tmpSet.insert(AddrINRA, 1150);  // 绝缘结果存放在1150
    tmpSet.insert(AddrACWL, 1155);  // 轴铁结果存放在1155
    tmpSet.insert(AddrACWC, 1160);  // 轴线结果存放在1160
    tmpSet.insert(AddrACWA, 1165);  // 铁线结果存放在1165
    tmpSet.insert(AddrWeld, 1200);  // 片间电阻存放在1200
    tmpSet.insert(AddrChip, 1300);  // 焊接电阻存入在1300
    tmpSet.insert(AddrDiag, 1350);  // 跨间电阻存放在1350
    tmpSet.insert(AddrIMPA, 1400);  // 匝间结果存放在1400
    tmpSet.insert(AddrIMPW, 1600);  // 匝间波形存放在1600
    tmpSet.insert(AddrMisc, 2000);  // 零散数据存入在2000
    tmpSet.insert(AddrBack, 2020);  // 后台数据存放在2020
    tmpSet.insert(AddrDCRB, 2040);  // 电阻后台存放在2040
    tmpSet.insert(AddrINRB, 2060);  // 绝缘后台存放在2060
    tmpSet.insert(AddrACWB, 2080);  // 交耐后台存放在2080
    tmpSet.insert(AddrIMPB, 2100);  // 匝间后台存放在2100
    tmpSet.insert(AddrSyst, 2120);  // 系统设置存放在2120
    tmpSet.insert(AddrInfo, 2140);  // 系统网络存放在2140
    tmpSet.insert(AddrLoad, 2160);  // 数据上传存放在2160
    tmpSet.insert(AddrShow, 2200);  // 界面信息存入在2200
    tmpSet.insert(AddrUser, 2300);  // 用户信息存放在2300
    tmpSet.insert(AddrType, 2500);  // 型号信息存放在2500

    tmpSet.insert(AddrModel, 3000);  // 综合设置存放在3000
    tmpSet.insert(AddrDCRS1, 3020);  // 片间配置存放在3020
    tmpSet.insert(AddrDCRS2, 3030);  // 焊接配置存放在3030
    tmpSet.insert(AddrDCRS3, 3040);  // 跨间配置存放在3040
    tmpSet.insert(AddrACWS1, 3050);  // 绝缘配置存放在3050
    tmpSet.insert(AddrACWS2, 3060);  // 轴铁配置存放在3060
    tmpSet.insert(AddrACWS3, 3070);  // 轴线配置存放在3070
    tmpSet.insert(AddrACWS4, 3080);  // 铁线配置存放在3080
    tmpSet.insert(AddrIMPS1, 3090);  // 匝间配置存放在3090
    tmpSet.insert(AddrDCRSW, 3200);  // 片间标准存放在3200
    tmpSet.insert(AddrIMPSW, 3600);  // 匝间波形存放在3600
    tmpSet.insert(AddrEnum, Qt::Key_Xfer);
    emit sendAppMsg(tmpSet);
}

void SqlCreate::openRecord(bool isExist)
{
    if (!isExist) {
        QFile file("./nandflash/record.db");
        file.open(QIODevice::ReadWrite | QIODevice::Text);
        file.close();
    }
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "record");
    db.setDatabaseName("./nandflash/record.db");
    if (!db.open()) {
        qDebug() << "record:" << db.lastError();
        return;
    }
    if (!isExist) {
        QSqlQuery query(db);
        QString cmd;
        cmd = "create table if not exists aip_record (";
        cmd += "R_UUID bigint primary key, R_GUID bigint,R_ITEM integer,R_TEXT text)";
        if (!query.exec(cmd))
            qDebug() << "aip_record:" << query.lastError();
        query.clear();
    }
}

void SqlCreate::openSystem(bool isExist)
{
    if (!isExist) {
        QFile file("./nandflash/system.db");
        file.open(QIODevice::ReadWrite | QIODevice::Text);
        file.close();
    }
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "system");
    db.setDatabaseName("./nandflash/system.db");
    if (!db.open()) {
        qDebug() << "system:" << db.lastError();
        return;
    }
    if (!isExist) {
        QSqlQuery query(db);
        QString cmd;
        cmd = "create table if not exists aip_system (";
        cmd += "uuid integer primary key, parm text)";
        if (!query.exec(cmd)) {
            qDebug() << "aip_system:" << query.lastError();
        }
        db.transaction();
        initMisc(query);
        initBack(query);
        initSyst(query);
        initInfo(query);
        initLoad(query);
        initShow(query);
        initUser(query);
        initType(query);
        db.commit();
        query.clear();
    }
}

void SqlCreate::openConfig(bool isExist)
{
    if (!isExist) {
        QFile file("./nandflash/config.db");
        file.open(QIODevice::ReadWrite | QIODevice::Text);
        file.close();
    }
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "config");
    db.setDatabaseName("./nandflash/config.db");
    if (!db.open()) {
        qDebug() << "config:" << db.lastError();
        return;
    }
    if (!isExist) {
        QSqlQuery query(db);
        QString cmd = "create table if not exists aip_config (";
        cmd += "uuid integer primary key, parm text)";
        if (!query.exec(cmd)) {
            qDebug() << "aip_config:" << query.lastError();
        }
        db.transaction();
        initConf(query);
        db.commit();
        query.clear();
    }
}

void SqlCreate::initMisc(QSqlQuery query)
{  // 零散数据
    int from = tmpSet.value(AddrMisc).toInt();
    QStringList parm;
    parm << "2500"  // 当前型号
         << "2300"  // 当前用户
         << "1"  // 自动保存
         << "0";  // 日志输出

    for (int i=parm.size(); i < 10; i++) {
        parm << "";
    }
    for (int i=0; i < parm.size(); i++) {
        query.prepare("insert into aip_system values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        if (!query.exec())
            qDebug() << "aip_system" << query.lastError();
    }
}

void SqlCreate::initBack(QSqlQuery query)
{  // 后台设置
    int from = tmpSet.value(AddrBack).toInt();
    QStringList parm;
    parm << "AIP8900";      // 产品型号
    parm << "189912001X";   // 产品编号
    parm << "G201801001";   // 评审编号
    parm << "2018-03";      // 出厂日期
    parm << "1";            // 语言支持
    parm << "1";            // 工位数量
    parm << "5000";         // 最高电压
    parm << "0";            // 自动测试
    parm << "2";            // 测试延时
    parm << "s.aipuo.com";  // 公司网址
    parm << "6000";         // 连接端口

    for (int i=0; i < parm.size(); i++) {
        query.prepare("insert into aip_system values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        if (!query.exec())
            qDebug() << "aip_system" << query.lastError();
    }
}

void SqlCreate::initSyst(QSqlQuery query)
{  // 系统设置
    int from = tmpSet.value(AddrSyst).toInt();
    QStringList parm;
    parm << "0"  // 语言设置
         << "0"  // 测试模式
         << "0"  // 启动方式
         << "5"  // 亮度设定
         << "5"  // 音量设定
         << "0"  // 条码起始
         << "9"  // 条码长度
         << "0.2"  //合格提示
         << "0.5";  // 报警时间

    for (int i=0; i < parm.size(); i++) {
        query.prepare("insert into aip_system values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        if (!query.exec())
            qDebug() << "aip_system" << query.lastError();
    }
}

void SqlCreate::initInfo(QSqlQuery query)
{
    int from = tmpSet.value(AddrInfo).toInt();
    QStringList parm;
    parm << "1"  // 自动地址
         << "192.168.1.1"  // 默认网关
         << "192.168.1.56"  // 网络地址
         << "255.255.255.0"  // 子网掩码
         << "192.168.1.186"  // 挂载地址
         << "/nfs"  // 挂载路径
         << ""  // 自动执行
         << "";  // 执行参数
    for (int i=0; i < parm.size(); i++) {
        query.prepare("insert into aip_system values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        if (!query.exec())
            qDebug() << "aip_system" << query.lastError();
    }
}

void SqlCreate::initLoad(QSqlQuery query)
{  // 数据上传
    int from = tmpSet.value(AddrLoad).toInt();
    QStringList parm;
    parm << tr("0") // 上传模式
         << tr("192.168.1.2") // 网络地址
         << tr("sa") // 登录用户
         << tr("Abc++123")// 登录密码
         << tr("aip-server") // 数据库名
         << tr("1433") // 网络端口
         << tr("0")
         << tr("192.168.1.55")
         << tr("sa")
         << tr("123")
         << tr("aip-server")
         << tr("1433") ;
    for (int i=0; i < parm.size(); i++) {
        query.prepare("insert into aip_system values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        if (!query.exec())
            qDebug() << "aip_system" << query.lastError();
    }
}

void SqlCreate::initShow(QSqlQuery query)
{   // 界面权限 name mark role form
    int from = tmpSet.value(AddrShow).toInt();
    QStringList parm;
    parm << "backup" << "后台管理" << "0" << "1";
    parm << "ioctrl" << "输出调试" << "0" << "1";
    parm << "logger" << "调试信息" << "0" << "1";
    parm << "offdcr" << "电阻后台" << "0" << "2";
    parm << "offacw" << "绝缘后台" << "0" << "2";
    parm << "offimp" << "匝间后台" << "0" << "2";
    parm << "author" << "系统主页" << "4" << "0";
    parm << "tester" << "测试界面" << "4" << "0";
    parm << "signin" << "用户登录" << "4" << "1";
    parm << "system" << "系统设置" << "2" << "1";
    parm << "master" << "用户管理" << "1" << "1";
    parm << "action" << "权限管理" << "1" << "1";
    parm << "config" << "型号管理" << "3" << "2";
    parm << "setdcr" << "电阻配置" << "2" << "2";
    parm << "setacw" << "介电强度" << "2" << "2";
    parm << "setimp" << "匝间配置" << "2" << "2";
    parm << "record" << "数据管理" << "1" << "3";
    parm << "upload" << "上传管理" << "1" << "3";
    parm << "sdcard" << "历史数据" << "1" << "3";
    parm << "unqual" << "不良分析" << "1" << "3";

    for (int i=0; i < parm.size(); i++) {
        query.prepare("insert into aip_system values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        if (!query.exec())
            qDebug() << "aip_system" << query.lastError();
    }
}

void SqlCreate::initUser(QSqlQuery query)
{  // 用户管理 name pass role last save
    int from = tmpSet.value(AddrUser).toInt();
    QStringList parm;
    parm << "supper" << "aip9918" << "0" << "" << "1";
    parm << "admin" << "6" << "1" << "" << "1";
    parm << "techo" << "6" << "2" << "" << "1";
    parm << "guest" << "6" << "3" << "" << "1";
    for (int i=parm.size(); i < 100; i+=5) {
        parm << "" << "" << "" << "" << "";
    }
    for (int i=0; i < parm.size(); i++) {
        query.prepare("insert into aip_system values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        if (!query.exec())
            qDebug() << "aip_system" << query.lastError();
    }
}

void SqlCreate::initType(QSqlQuery query)
{  // 型号管理
    int from = tmpSet.value(AddrType).toInt();
    QStringList parm;
    parm << tr("aip_config");
    for (int i=parm.size(); i < STEP; i++) {
        parm << "";
    }
    for (int i=0; i < STEP; i++) {
        query.prepare("insert into aip_system values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        if (!query.exec())
            qDebug() << "aip_system" << query.lastError();
    }
}

void SqlCreate::initConf(QSqlQuery query)
{
    int from = tmpSet.value(AddrModel).toInt();
    QStringList parm;
    parm << "12" << "12" << "" << "" << "" << "" << "" << "" << "" << "";  // 综合配置
    parm << "" << "" << "" << "" << "" << "" << "" << "" << "" << "";
    parm << "1" << "10000" << "1" << "25000" << "500" << "" << "" << "" << "" << "";  // 片间配置
    parm << "1" << "500" << "" << "" << "" << "" << "" << "" << "" << "";  // 焊接配置
    parm << "1" << "500" << "1500" << "" << "" << "" << "" << "" << "" << "";  // 跨间配置
    parm << "1" << tr("绝缘电阻") << "500" << "10" << "0" << "500" << "0" << "0" << "" << "";  // 绝缘配置
    parm << "1" << tr("轴铁耐压") << "1500" << "10" << "500" << "0" << "0" << "0" << "" << "";  // 轴铁配置
    parm << "1" << tr("轴线耐压") << "2500" << "10" << "500" << "0" << "0" << "0" << "" << "";  // 轴线配置
    parm << "1" << tr("铁线耐压") << "4500" << "10" << "500" << "0" << "0" << "0" << "" << "";  // 铁线配置
    parm << "1" << tr("匝间测试") << "500" << "1" << "10" << "2" << "0" << "1"<< "1" << "7";  // 匝间
    for (int i=0; i < parm.size(); i++) {
        query.prepare("insert into aip_config values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        if (!query.exec())
            qDebug() << "aip_config" << query.lastError();
    }
}

