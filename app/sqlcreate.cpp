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
        openBackup(dir.exists("./nandflash/backup.db"));
        openSystem(dir.exists("./nandflash/system.db"));
        openConfig(dir.exists("./nandflash/config.db"));
        openRecord(dir.exists("./nandflash/record.db"));
    }
}

void SqlCreate::initTmpDat()
{
    tmpSet.insert(AddrBack, 10000 + 0x00);  // 后台数据地址
    tmpSet.insert(AddrDCRB, 10000 + 0x10);  // 电阻后台地址
    tmpSet.insert(AddrINRB, 10000 + 0x20);  // 绝缘后台地址
    tmpSet.insert(AddrACWB, 10000 + 0x30);  // 交耐后台地址
    tmpSet.insert(AddrIMPB, 10000 + 0x40);  // 匝间后台地址

    tmpSet.insert(AddrSyst, 20000 + 2120);  // 系统设置存放在2120
    tmpSet.insert(AddrInfo, 20000 + 2140);  // 系统网络存放在2140
    tmpSet.insert(AddrLoad, 20000 + 2160);  // 数据上传存放在2160
    tmpSet.insert(AddrShow, 20000 + 2200);  // 界面信息存入在2200
    tmpSet.insert(AddrUser, 20000 + 2300);  // 用户信息存放在2300
    tmpSet.insert(AddrType, 20000 + 2500);  // 型号信息存放在2500

    tmpSet.insert(AddrOther, 22000 + 0x0000);  // 零散数据地址

    tmpSet.insert(AddrDCRR1, 30000 + 0x0000);  // 片间结果地址
    tmpSet.insert(AddrDCRR2, 30000 + 0x0400);  // 焊接结果地址
    tmpSet.insert(AddrDCRR3, 30000 + 0x0800);  // 跨间结果地址
    tmpSet.insert(AddrACWR1, 30000 + 0x0A00);  // 绝缘结果地址
    tmpSet.insert(AddrACWR2, 30000 + 0x0A08);  // 轴铁结果地址
    tmpSet.insert(AddrACWR3, 30000 + 0x0A10);  // 轴线结果地址
    tmpSet.insert(AddrACWR4, 30000 + 0x0A18);  // 铁线结果地址
    tmpSet.insert(AddrIMPR1, 30000 + 0x0A20);  // 匝间结果地址
    tmpSet.insert(AddrIMPW1, 30000 + 0x0E20);  // 匝间波形地址

    tmpSet.insert(AddrModel, 40000 + 0x0000);  // 综合设置地址
    tmpSet.insert(AddrDCRS1, 40000 + 0x0010);  // 片间配置地址
    tmpSet.insert(AddrDCRS2, 40000 + 0x0020);  // 焊接配置地址
    tmpSet.insert(AddrDCRS3, 40000 + 0x0030);  // 跨间配置地址
    tmpSet.insert(AddrACWS1, 40000 + 0x0040);  // 绝缘配置地址
    tmpSet.insert(AddrACWS2, 40000 + 0x0050);  // 轴铁配置地址
    tmpSet.insert(AddrACWS3, 40000 + 0x0060);  // 轴线配置地址
    tmpSet.insert(AddrACWS4, 40000 + 0x0070);  // 铁线配置地址
    tmpSet.insert(AddrIMPS1, 40000 + 0x0080);  // 匝间配置地址
    tmpSet.insert(AddrDCRSW, 40000 + 0x0100);  // 焊接配置地址
    tmpSet.insert(AddrIMPSW, 40000 + 0x0300);  // 匝间波形地址

    tmpSet.insert(tmpSet.value(AddrDCRR1).toInt() + AddrDataS, 0xff);
    tmpSet.insert(tmpSet.value(AddrACWR1).toInt() + AddrDataS, 0xff);
    tmpSet.insert(tmpSet.value(AddrIMPR1).toInt() + AddrDataS, 0xff);

    tmpSet.insert(AddrEnum, Qt::Key_Xfer);
    emit sendAppMsg(tmpSet);
}

void SqlCreate::openBackup(bool isExist)
{
    if (!isExist) {
        QFile file("./nandflash/backup.db");
        file.open(QIODevice::ReadWrite | QIODevice::Text);
        file.close();
    }
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "backup");
    db.setDatabaseName("./nandflash/backup.db");
    if (!db.open()) {
        qWarning() << "backup:" << db.lastError();
        return;
    }
    if (!isExist) {
        QSqlQuery query(db);
        QString cmd;
        cmd = "create table if not exists aip_backup (";
        cmd += "uuid integer primary key, parm text)";
        if (!query.exec(cmd)) {
            qWarning() << "aip_backup:" << query.lastError();
        }
        db.transaction();
        initBack(query);
        initINRB(query);
        initACWB(query);
        initIMPB(query);
        db.commit();
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
        qWarning() << "system:" << db.lastError();
        return;
    }
    if (!isExist) {
        QSqlQuery query(db);
        QString cmd;
        cmd = "create table if not exists aip_sqlite (";
        cmd += "R_UUID bigint, R_ITEM integer,R_QUAN integer,";
        cmd += "R_OKNG integer,primary key (R_UUID,R_ITEM))";
        if (!query.exec(cmd)) {
            qWarning() << "aip_sqlite:" << query.lastError();
        }
        cmd = "create table if not exists aip_system (";
        cmd += "uuid integer primary key, parm text)";
        if (!query.exec(cmd)) {
            qWarning() << "aip_system:" << query.lastError();
        }
        db.transaction();
        initMisc(query);
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
        qWarning() << "config:" << db.lastError();
        return;
    }
    if (!isExist) {
        QSqlQuery query(db);
        QString cmd = "create table if not exists aip_config (";
        cmd += "uuid integer primary key, parm text)";
        if (!query.exec(cmd)) {
            qWarning() << "aip_config:" << query.lastError();
        }
        db.transaction();
        initConf(query);
        db.commit();
        query.clear();
    }
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
        qWarning() << "record:" << db.lastError();
        return;
    }
    if (!isExist) {
        QSqlQuery query(db);
        QString cmd;
        cmd = "create table if not exists aip_record (";
        cmd += "R_UUID bigint, R_ITEM integer,R_TEXT text,primary key (R_UUID,R_ITEM))";
        if (!query.exec(cmd))
            qWarning() << "aip_record:" << query.lastError();
        query.clear();
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
        query.prepare("insert into aip_backup values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        if (!query.exec())
            qWarning() << "aip_system" << query.lastError();
    }
}

void SqlCreate::initINRB(QSqlQuery query)
{  // 参数校准
    int from = tmpSet.value(AddrINRB).toInt();
    QStringList parm;
    parm << "500"
         << "1000"
         << "100"
         << "500"
         << "50"
         << "500"
         << "50"
         << "500";
    for (int i=0; i < parm.size(); i++) {
        query.prepare("insert into aip_backup values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        if (!query.exec())
            qWarning() << "aip_backup" << query.lastError();
    }
}

void SqlCreate::initACWB(QSqlQuery query)
{
    int from = tmpSet.value(AddrACWB).toInt();
    QStringList parm;
    parm << "500"
         << "4500"
         << "300"
         << "1500"
         << "10"
         << "300";
    for (int i=0; i < parm.size(); i++) {
        query.prepare("insert into aip_backup values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        if (!query.exec())
            qWarning() << "aip_backup" << query.lastError();
    }
}

void SqlCreate::initIMPB(QSqlQuery query)
{
    int from = tmpSet.value(AddrIMPB).toInt();
    QStringList parm;
    parm << "500"
         << "1000"
         << "1000"
         << "2000"
         << "2000"
         << "3000"
         << "3000"
         << "4000"
         << "4000"
         << "5000";
    for (int i=0; i < parm.size(); i++) {
        query.prepare("insert into aip_backup values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        if (!query.exec())
            qWarning() << "aip_backup" << query.lastError();
    }
}

void SqlCreate::initMisc(QSqlQuery query)
{  // 零散数据
    int from = tmpSet.value(AddrOther).toInt();
    QStringList parm;
    parm << "22500"  // 当前型号
         << "22300"  // 当前用户
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
            qWarning() << "aip_system" << query.lastError();
    }
}

void SqlCreate::initSyst(QSqlQuery query)
{  // 系统设置
    int from = tmpSet.value(AddrSyst).toInt();
    QStringList parm;
    parm << "0"  // 语言设置
         << "0"  // 测试模式
         << "0"  // 启动方式
         << "9"  // 亮度设定
         << "9"  // 音量设定
         << "0"  // 条码起始
         << "9"  // 条码长度
         << "0.2"  //合格提示
         << "0.5";  // 报警时间

    for (int i=0; i < parm.size(); i++) {
        query.prepare("insert into aip_system values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        if (!query.exec())
            qWarning() << "aip_system" << query.lastError();
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
            qWarning() << "aip_system" << query.lastError();
    }
}


void SqlCreate::initLoad(QSqlQuery query)
{  // 数据上传
    int from = tmpSet.value(AddrLoad).toInt();
    QStringList parm;
    parm << tr("0") // 上传模式
         << tr("192.168.1.2") // 网络地址
         << tr("sa") // 登录用户
         << tr("Abc++123")  // 登录密码
         << tr("aip-server") // 数据库名
         << tr("1433") // 网络端口
         << tr("0")
         << tr("192.168.1.55")
         << tr("sa")
         << tr("123")
         << tr("aip-server")
         << tr("1433");
    for (int i=0; i < parm.size(); i++) {
        query.prepare("insert into aip_system values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        if (!query.exec())
            qWarning() << "aip_system" << query.lastError();
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
    parm << "online" << "在线设备" << "2" << "1";
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
            qWarning() << "aip_system" << query.lastError();
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
            qWarning() << "aip_system" << query.lastError();
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
            qWarning() << "aip_system" << query.lastError();
    }
}

void SqlCreate::initConf(QSqlQuery query)
{
    int from = tmpSet.value(AddrModel).toInt();
    QStringList parm;
    parm.clear();
    from = tmpSet.value(AddrDCRS1).toInt();
    parm << "1" << "1000" << "1" << "25000" << "50" << "0" << "" << "" << "" << "";
    for (int i=0; i < parm.size(); i++) {
        query.prepare("insert into aip_config values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        if (!query.exec())
            qWarning() << "aip_config" << query.lastError();
    }
    parm.clear();
    from = tmpSet.value(AddrDCRS2).toInt();
    parm << "1" << "100" << "" << "" << "" << "" << "" << "" << "" << "";
    for (int i=0; i < parm.size(); i++) {
        query.prepare("insert into aip_config values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        if (!query.exec())
            qWarning() << "aip_config" << query.lastError();
    }
    parm.clear();
    from = tmpSet.value(AddrDCRS3).toInt();
    parm << "1" << "500" << "1500" << "" << "" << "" << "" << "" << "" << "";
    for (int i=0; i < parm.size(); i++) {
        query.prepare("insert into aip_config values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        if (!query.exec())
            qWarning() << "aip_config" << query.lastError();
    }
    parm.clear();
    from = tmpSet.value(AddrACWS1).toInt();
    parm << "1" << tr("绝缘电阻") << "500" << "5" << "0" << "500" << "0" << "0" << "" << "";
    for (int i=0; i < parm.size(); i++) {
        query.prepare("insert into aip_config values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        if (!query.exec())
            qWarning() << "aip_config" << query.lastError();
    }
    parm.clear();
    from = tmpSet.value(AddrACWS2).toInt();
    parm << "1" << tr("轴铁耐压") << "500" << "5" << "500" << "0" << "0" << "0" << "" << "";
    for (int i=0; i < parm.size(); i++) {
        query.prepare("insert into aip_config values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        if (!query.exec())
            qWarning() << "aip_config" << query.lastError();
    }
    parm.clear();
    from = tmpSet.value(AddrACWS3).toInt();
    parm << "1" << tr("轴线耐压") << "500" << "5" << "500" << "0" << "0" << "0" << "" << "";
    for (int i=0; i < parm.size(); i++) {
        query.prepare("insert into aip_config values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        if (!query.exec())
            qWarning() << "aip_config" << query.lastError();
    }
    parm.clear();
    from = tmpSet.value(AddrACWS4).toInt();
    parm << "1" << tr("铁线耐压") << "500" << "5" << "500" << "0" << "0" << "0" << "" << "";
    for (int i=0; i < parm.size(); i++) {
        query.prepare("insert into aip_config values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        if (!query.exec())
            qWarning() << "aip_config" << query.lastError();
    }
    parm.clear();
    from = tmpSet.value(AddrIMPS1).toInt();
    parm << "1" << tr("匝间测试") << "500" << "1" << "15" << "2" << "0" << "1"<< "1" << "7";
    for (int i=0; i < parm.size(); i++) {
        query.prepare("insert into aip_config values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        if (!query.exec())
            qWarning() << "aip_config" << query.lastError();
    }
    parm.clear();
    from = tmpSet.value(AddrModel).toInt();
    parm << "10" << "1" << "" << "" << "" << "" << "" << "" << "" << "";  // 综合配置
    for (int i=0; i < parm.size(); i++) {
        query.prepare("insert into aip_config values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        if (!query.exec())
            qWarning() << "aip_config" << query.lastError();
    }
}

