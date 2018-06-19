/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       数据导入
*******************************************************************************/
#include "sqlimport.h"

SqlImport::SqlImport(QObject *parent) : QObject(parent)
{
}

void SqlImport::initData()
{
    qint64 uuid = QDate::currentDate().toJulianDay();
    QSqlQuery query(QSqlDatabase::database("system"));
    query.prepare("select * from aip_sqlite where R_UUID = ?");
    query.addBindValue(uuid);
    query.exec();
    while (query.next()) {
        int item = query.value(1).toInt();
        int quan = query.value(2).toInt();
        int okng = query.value(3).toInt();
        tmpQuan[item] = quan;
        tmpOKNG[item] = okng;
    }
}

void SqlImport::initFlash()
{
#ifdef __arm__
    if (readSdcard()) {
        if (readRecord()) {
            initBackup();
            initRecord();
        }
    }
#endif
}

bool SqlImport::readRecord()
{
    bool isExist = false;
    QProcess cmddf;
    cmddf.start("df -h");
    if (cmddf.waitForFinished()) {
        QByteArray bytedf = cmddf.readAll();
        QStringList listdf = QString(bytedf).split("\n");
        for (int i=0; i < listdf.size(); i++) {
            if (listdf.at(i).startsWith("/dev/mmcblk0p1")) {
                isExist =  true;
                break;
            }
        }
    }
    cmddf.deleteLater();
    return isExist;
}

bool SqlImport::readSdcard()
{
    QProcess cmddu;
    cmddu.start("du -s /mnt/nandflash/record.db");
    cmddu.waitForFinished();
    QByteArray du = cmddu.readAll();
    QString sf = du.mid(0, du.indexOf("\t"));
    return (sf.toInt() > SD_SIZE);
}

bool SqlImport::initBackup()
{
    tmpMap.insert(AddrEnum, Qt::Key_Copy);
    tmpMap.insert(AddrText, "copy");
    emit sendAppMsg(tmpMap);
    tmpMap.clear();
    QProcess cmddu;
    QString name = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    QString p = "cp /mnt/nandflash/record.db /mnt/sdcard/";
    p.append(name + ".db");
    cmddu.start(p);
    cmddu.waitForFinished();
    return true;
}

bool SqlImport::initRecord()
{
    QSqlDatabase::database("record").close();
    QSqlDatabase::database("record").open();
    QSqlQuery query(QSqlDatabase::database("record"));
    if (!query.exec("drop table if exists aip_record"))
        qDebug() << query.lastError();
    if (!query.exec("vacuum"))
        qDebug() << query.lastError();
    QString cmd;
    cmd = "create table if not exists aip_record (";
    cmd += "R_UUID bigint, R_ITEM integer,R_TEXT text,primary key (R_UUID,R_ITEM))";
    if (!query.exec(cmd))
        qWarning() << "aip_record:" << query.lastError();
    query.clear();
    tmpMap.insert(AddrEnum, Qt::Key_Copy);
    tmpMap.insert(AddrText, "over");
    emit sendAppMsg(tmpMap);
    tmpMap.clear();
    return true;
}

void SqlImport::saveSqlite(QTmpMap msg)
{
    if (tmpQuan.isEmpty()) {
        initData();
    }
    qint64 uuid = QDate::currentDate().toJulianDay();
    QSqlDatabase::database("record").transaction();
    QSqlQuery query(QSqlDatabase::database("record"));
    if (1) {
        int test = msg[AddrModel].toInt();
        tmpQuan[test] = tmpQuan[test].toInt() + 1;
        if (msg.value(DataOKNG).toInt() == 0) {
            tmpOKNG[test] = tmpOKNG[test].toInt() + 1;
        }
        query.prepare("replace into aip_sqlite values(?,?,?,?)");
        query.addBindValue(uuid);
        query.addBindValue(test);
        query.addBindValue(tmpQuan[test].toInt());
        query.addBindValue(tmpOKNG[test].toInt());
        query.exec();
    }
    for (int t=0; t < 3; t++) {
        int test = msg[AddrDCRS1 + t].toInt();  // 是否测试
        if (msg[test].toInt() == 1) {
            int addr = msg[AddrDCRR1 + t].toInt();  // 电阻结果地址 版本,温度,状态,判定
            tmpQuan[test] = tmpQuan[test].toInt() + 1;
            if (msg.value(addr + AddrDataJ).toInt() == 0) {
                tmpOKNG[test] = tmpOKNG[test].toInt() + 1;
            }
            query.prepare("replace into aip_sqlite values(?,?,?,?)");
            query.addBindValue(uuid);
            query.addBindValue(test);
            query.addBindValue(tmpQuan[test].toInt());
            query.addBindValue(tmpOKNG[test].toInt());
            query.exec();
        }
    }
    for (int t=0; t < 4; t++) {
        int test = msg[AddrACWS1 + t].toInt();  // 是否测试
        if (msg[test].toInt() == 1) {
            int addr = msg[AddrACWR1 + t].toInt();  // 高压结果地址 版本,温度,状态,判定
            tmpQuan[test] = tmpQuan[test].toInt() + 1;
            if (msg.value(addr + AddrDataJ).toInt() == 0) {
                tmpOKNG[test] = tmpOKNG[test].toInt() + 1;
            }
            query.prepare("replace into aip_sqlite values(?,?,?,?)");
            query.addBindValue(uuid);
            query.addBindValue(test);
            query.addBindValue(tmpQuan[test].toInt());
            query.addBindValue(tmpOKNG[test].toInt());
            query.exec();
        }
    }
    if (1) {
        int test = msg[AddrIMPS1].toInt();
        if (msg[test].toInt() == 1) {
            int addr = msg[AddrIMPR1].toInt();  // 匝间结果地址 版本,温度,状态,判定
            tmpQuan[test] = tmpQuan[test].toInt() + 1;
            if (msg.value(addr + AddrDataJ).toInt() == 0) {
                tmpOKNG[test] = tmpOKNG[test].toInt() + 1;
            }
            query.prepare("replace into aip_sqlite values(?,?,?,?)");
            query.addBindValue(uuid);
            query.addBindValue(test);
            query.addBindValue(tmpQuan[test].toInt());
            query.addBindValue(tmpOKNG[test].toInt());
            query.exec();
        }
    }
    QSqlDatabase::database("record").commit();
}

void SqlImport::saveRecord(QTmpMap msg)
{
    QSqlDatabase::database("record").transaction();
    QSqlQuery query(QSqlDatabase::database("record"));
    SqlSnowId snow;
    quint64 uuid = snow.getId();
    int c = msg[AddrModel].toInt();
    int weld = msg[c + 1].toInt();
    for (int t=0; t < 3; t++) {
        int test = msg[AddrDCRS1 + t].toInt();
        int addr = msg[AddrDCRR1 + t].toInt() + 4;  // 电阻结果地址 档位,结果,小数,判定
        int quan = (t == 2) ? 2 : 4;
        if (msg[test].toInt() == 1) {
            for (int i=0; i < weld*quan; i++) {
                query.prepare("insert into aip_record values(?,?,?)");
                query.addBindValue(uuid);
                query.addBindValue(addr + i);
                query.addBindValue(msg.value(addr + i).toInt());
                query.exec();
            }
        }
    }
    for (int t=0; t < 4; t++) {
        int test = msg[AddrACWS1 + t].toInt();
        if (msg[test].toInt() == 1) {
            int addr = msg[AddrACWR1 + t].toInt() + 4;  // 高压结果地址 电压,结果,小数,判定
            for (int i=0; i < 4; i++) {
                query.prepare("insert into aip_record values(?,?,?)");
                query.addBindValue(uuid);
                query.addBindValue(addr + i);
                query.addBindValue(msg.value(addr + i).toInt());
                query.exec();
            }
        }
    }
    if (1) {
        int test = msg[AddrIMPS1].toInt();
        int time = msg[AddrIMPSA].toInt();
        int addr = msg[AddrIMPR1].toInt() + 4;  // 匝间结果地址 电压,结果,频率,判定
        int quan = (time*2 == weld) ? 2 : 4;
        if (msg[test].toInt() == 1) {
            for (int i=0; i < weld*quan; i++) {
                query.prepare("insert into aip_record values(?,?,?)");
                query.addBindValue(uuid);
                query.addBindValue(addr + i);
                query.addBindValue(msg.value(addr + i).toInt());
                query.exec();
            }
        }
    }
    QSqlDatabase::database("record").commit();
}

void SqlImport::recvAppMsg(QTmpMap msg)
{
    int c = msg.value(AddrEnum).toInt();
    switch (c) {
    case Qt::Key_Save:
        if (msg.value(AddrText).toString() == "aip_record") {
            saveSqlite(msg);
            saveRecord(msg);
        }
        break;
    case Qt::Key_Game:
        QTimer::singleShot(1000, this, SLOT(initFlash()));
        break;
    default:
        break;
    }
}

