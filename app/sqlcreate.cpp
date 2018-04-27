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

void SqlCreate::createRecord()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "createRecord");
    db.setDatabaseName("./nandflash/record.db");
    if (!db.open()) {
        qDebug() << "record:" << db.lastError();
        return;
    }

    QSqlQuery query(db);
    QString cmd;
    cmd = "create table if not exists aip_record (";
    cmd += "uuid bigint primary key, guid bigint, numb integer,";
    cmd += "item text,parm text,rslt text,pass text)";
    if (!query.exec(cmd))
        qDebug() << "aip_record:" << query.lastError();
}

void SqlCreate::createSqlite()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "createSqlite");
    db.setDatabaseName("./nandflash/sqlite.db");
    if (!db.open()) {
        qDebug() << "sqlite:" << db.lastError();
        return;
    }

    QSqlQuery query(db);
    QString cmd;
    cmd = "create table if not exists G_DEFAULT (";
    cmd += "uuid integer primary key, parm text)";
    if (!query.exec(cmd)) {
        qDebug() << "create table G_DEFAULT Error";
    }
    db.transaction();
    insertStatInfo(query);
    insertBackinfo(query);
    insertSystInfo(query);
    insertUserInfo(query);
    insertSignInfo(query);
    insertTypeInfo(query);
    insertOffsetDcr(query);
    insertOffsetInr(query);
    insertOffsetAcw(query);
    insertOffsetImp(query);
    insertSqlNetwork(query);
    insertMasterInfo(query);
    insertSourceInfo(query);
    insertConfigInfo(query);
    insertSetAcwInfo(query);
    db.commit();

    query.clear();
}

void SqlCreate::insertStatInfo(QSqlQuery query)
{  // 软件状态区,默认为空
    int from = 0x0000;
    QStringList parm;

    for (int i=parm.size(); i < STEP; i++) {
        parm << "";
    }
    for (int i=0; i < parm.size(); i++) {
        query.prepare("insert into G_DEFAULT values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        query.exec();
    }
}

void SqlCreate::insertBackinfo(QSqlQuery query)
{  // 后台设置
    int from = 0x0010;
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

    for (int i=parm.size(); i < STEP; i++) {
        parm << "";
    }
    for (int i=0; i < parm.size(); i++) {
        query.prepare("insert into G_DEFAULT values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        query.exec();
    }
}

void SqlCreate::insertSystInfo(QSqlQuery query)
{  // 系统设置
    int from = 0x0020;
    QStringList parm;
    parm << "0" << "0" << "0" << "5" << "5"     // 语言设置,测试模式,启动方式,亮度设定,音量设定
         << "0" << "9" << "0.2" << "0.5";       // 条码起始,条码长度,合格报警,报警提示
    for (int i=parm.size(); i < STEP; i++) {
        parm << "";
    }
    for (int i=0; i < parm.size(); i++) {
        query.prepare("insert into G_DEFAULT values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        query.exec();
    }
}

void SqlCreate::insertUserInfo(QSqlQuery query)
{  // 自动地址,默认网关,网络地址,子网掩码,挂载地址,挂载路径,自动执行,执行参数
    int from = 0x0030;
    QStringList parm;
    parm << "1" << "192.168.1.1" << "192.168.1.56" << "255.255.255.0";
    parm << "192.168.1.186" << "/nfs" << "" << "";
    for (int i=parm.size(); i < STEP; i++) {
        parm << "";
    }
    for (int i=0; i < parm.size(); i++) {
        query.prepare("insert into G_DEFAULT values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        query.exec();
    }
}

void SqlCreate::insertSignInfo(QSqlQuery query)
{ // 用户名,密码,记住密码,自动登录
    int from = 0x0040;
    QStringList parm;
    parm << tr("admin") << tr("6") << tr("1") << tr("1");
    for (int i=parm.size(); i < STEP; i++) {
        parm << "";
    }
    for (int i=0; i < STEP; i++) {
        query.prepare("insert into G_DEFAULT values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        query.exec();
    }
}

void SqlCreate::insertTypeInfo(QSqlQuery query)
{  // 当前型号
    int from = 0x0050;
    QStringList parm;
    parm << tr("DEFAULT");
    for (int i=parm.size(); i < STEP; i++) {
        parm << "";
    }
    for (int i=0; i < STEP; i++) {
        query.prepare("insert into G_DEFAULT values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        query.exec();
    }
}

void SqlCreate::insertOffsetDcr(QSqlQuery query)
{  // 电阻K,b
    int from = 0x0060;
    QStringList parm;
    parm << "32767" << "0" << "32767" << "0" << "32767" << "0" << "32767" << "0"
         << "32767" << "0" << "32767" << "0" << "32767" << "0" << "32767" << "0";
    for (int i=parm.size(); i < STEP*2; i++) {
        parm << "";
    }
    for (int i=0; i < parm.size(); i++) {
        query.prepare("insert into G_DEFAULT values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        query.exec();
    }
}

void SqlCreate::insertOffsetInr(QSqlQuery query)
{  // 绝缘K,b
    int from = 0x0080;
    QStringList parm;
    parm << "32767" << "0" << "32767" << "0" << "32767" << "0" << "32767" << "0"
         << "32767" << "0" << "32767" << "0" << "32767" << "0" << "32767" << "0";
    for (int i=parm.size(); i < STEP*2; i++) {
        parm << "";
    }
    for (int i=0; i < parm.size(); i++) {
        query.prepare("insert into G_DEFAULT values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        query.exec();
    }
}

void SqlCreate::insertOffsetAcw(QSqlQuery query)
{  // 交耐K,b
    int from = 0x00A0;
    QStringList parm;
    parm << "32767" << "0" << "32767" << "0" << "32767" << "0" << "32767" << "0"
         << "32767" << "0" << "32767" << "0" << "32767" << "0" << "32767" << "0";
    for (int i=parm.size(); i < STEP*2; i++) {
        parm << "";
    }
    for (int i=0; i < parm.size(); i++) {
        query.prepare("insert into G_DEFAULT values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        query.exec();
    }
}

void SqlCreate::insertOffsetImp(QSqlQuery query)
{  // 匝间K,b
    int from = 0x00C0;
    QStringList parm;
    parm << "32767" << "0" << "32767" << "0" << "32767" << "0" << "32767" << "0"
         << "32767" << "0" << "32767" << "0" << "32767" << "0" << "32767" << "0";
    for (int i=parm.size(); i < STEP*2; i++) {
        parm << "";
    }
    for (int i=0; i < parm.size(); i++) {
        query.prepare("insert into G_DEFAULT values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        query.exec();
    }
}

void SqlCreate::insertSqlNetwork(QSqlQuery query)
{  // 上传模式,网络地址,登录用户,登录密码,数据库名,网络端口
    int from = 0x00E0;
    QStringList parm;
    parm << tr("0") << tr("192.168.1.2") << tr("sa") << tr("Abc++123")
         << tr("aip-server") << tr("1433") << tr("") << tr("");
    parm << tr("0") << tr("192.168.1.55") << tr("sa") << tr("123")
         << tr("aip-server") << tr("1433") << tr("") << tr("");
    for (int i=parm.size(); i < STEP*2; i++) {
        parm << "";
    }
    for (int i=0; i < STEP; i++) {
        query.prepare("insert into G_DEFAULT values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        query.exec();
    }
}


void SqlCreate::insertMasterInfo(QSqlQuery query)
{  // 用户组别:0-超级用户;1-管理员;2-技术员;3-操作员;4-未登录;
    int from = 0x0100;
    QStringList parm;
    QString time = QDateTime::currentDateTime().toString("yy-MM-dd hh:mm:ss");
    parm << "supper" << "aip9918" << "0" << time;
    parm << "admin" << "6" << "1" << time;
    parm << "techo" << "6" << "2" << time;
    parm << "guest" << "6" << "3" << time;
    for (int i=parm.size(); i < STEP*0x10; i+=4) {
        parm << "" << "" << "" << "";
    }
    for (int i=0; i < parm.size(); i++) {
        query.prepare("insert into G_DEFAULT values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        query.exec();
    }
}

void SqlCreate::insertSourceInfo(QSqlQuery query)
{   // 界面组别:0-所有界面;1-系统设置;2-型号管理;3-数据管理;
    // 界面权限:0-超级用户;1-管理员;2-技术员;3-操作员;4-未登录;
    int from = 0x0200;
    QStringList parm;
    parm << "backup" << "后台管理" << "1" << "0";
    parm << "ioctrl" << "输出调试" << "1" << "0";
    parm << "logger" << "调试信息" << "1" << "0";
    parm << "offdcr" << "电阻后台" << "2" << "0";
    parm << "offacw" << "绝缘后台" << "2" << "0";
    parm << "offimp" << "匝间后台" << "2" << "0";
    for (int i=parm.size(); i < 0x0020; i+=4) {
        parm << "" << "" << "" << "";
    }

    parm << "author" << "系统主页" << "0" << "4";
    parm << "tester" << "测试界面" << "0" << "4";
    parm << "signin" << "用户登录" << "1" << "4";
    for (int i=parm.size(); i < 0x0040; i+=4) {
        parm << "" << "" << "" << "";
    }

    parm << "system" << "系统设置" << "1" << "1";
    parm << "master" << "用户管理" << "1" << "1";
    parm << "action" << "权限管理" << "1" << "1";
    parm << "config" << "型号管理" << "2" << "2";
    parm << "setall" << "综合配置" << "2" << "2";
    parm << "setdcr" << "电阻配置" << "2" << "2";
    parm << "setacw" << "介电强度" << "2" << "2";
    parm << "setimp" << "匝间配置" << "2" << "2";

    parm << "record" << "数据管理" << "3" << "1";
    parm << "upload" << "上传管理" << "3" << "1";
    parm << "sdcard" << "历史数据" << "3" << "1";
    parm << "unqual" << "不良分析" << "3" << "1";

    for (int i=parm.size(); i < STEP*0x10; i+=4) {
        parm << "" << "" << "" << "";
    }
    for (int i=0; i < STEP*0x10; i++) {
        query.prepare("insert into G_DEFAULT values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        query.exec();
    }
}

void SqlCreate::insertConfigInfo(QSqlQuery query)
{
    int from = 0x0300;
    QStringList parm;
    parm << "DEFAULT";

    for (int i=parm.size(); i < STEP*0x10; i++) {
        parm << "";
    }
    for (int i=0; i < STEP*0x10; i++) {
        query.prepare("insert into G_DEFAULT values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        query.exec();
    }

    QString cmd = "create table if not exists M_DEFAULT (";
    cmd += "uuid integer primary key, parm text)";
    if (!query.exec(cmd)) {
        qDebug() << "create table M_DEFAULT Error";
    }
}

void SqlCreate::insertSetAcwInfo(QSqlQuery query)
{  // 绝缘参数
    int from = 0x0460;
    QStringList parm;
    for (int i=parm.size(); i < STEP*0x01; i++) {
        parm << "";
    }
    parm << "1";
    for (int i=parm.size(); i < STEP*0x02; i++) {
        parm << "";
    }
    parm << "1";
    for (int i=parm.size(); i < STEP*0x03; i++) {
        parm << "";
    }
    parm << "1";
    for (int i=parm.size(); i < STEP*0x04; i++) {
        parm << "";
    }
    parm << "1";
    for (int i=parm.size(); i < STEP*0x05; i++) {
        parm << "";
    }

    for (int i=0; i < STEP*0x05; i++) {
        query.prepare("insert into M_DEFAULT values(?,?)");
        query.addBindValue(from + i);
        query.addBindValue(parm.at(i));
        query.exec();
    }
}

