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
    tmpMap.insert(Qt::Key_0, Qt::Key_Copy);
    tmpMap.insert(Qt::Key_1, "copy");
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
        qWarning() << query.lastError();
    if (!query.exec("vacuum"))
        qWarning() << query.lastError();
    QString cmd;
    cmd = "create table if not exists aip_record (";
    cmd += "R_UUID bigint, R_ITEM integer,R_TEXT text,primary key (R_UUID,R_ITEM))";
    if (!query.exec(cmd))
        qWarning() << "aip_record:" << query.lastError();
    query.clear();
    tmpMap.insert(Qt::Key_0, Qt::Key_Copy);
    tmpMap.insert(Qt::Key_1, "over");
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
        int test = msg[(4000 + Qt::Key_0)].toInt();
        tmpQuan[test] = tmpQuan[test].toInt() + 1;
        if (msg.value(tmpSet.value((3000 + Qt::Key_0)).toInt() + TEMPISOK).toInt() == 0) {
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
        int test = msg[(4000 + Qt::Key_1) + t].toInt();  // 是否测试
        if (msg[test].toInt() == 1) {
            int addr = msg[(3000 + Qt::Key_1) + t].toInt();  // 电阻结果地址 版本,温度,状态,判定
            tmpQuan[test] = tmpQuan[test].toInt() + 1;
            if (msg.value(addr + OKNGDCRA).toInt() == 0) {
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
        int test = msg[(4000 + Qt::Key_4) + t].toInt();  // 是否测试
        if (msg[test].toInt() == 1) {
            int addr = msg[(3000 + Qt::Key_4) + t].toInt();  // 高压结果地址 版本,温度,状态,判定
            tmpQuan[test] = tmpQuan[test].toInt() + 1;
            if (msg.value(addr + OKNGACWA).toInt() == 0) {
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
        int test = msg[(4000 + Qt::Key_8)].toInt();
        if (msg[test].toInt() == 1) {
            int addr = msg[(3000 + Qt::Key_8)].toInt();  // 匝间结果地址 版本,温度,状态,判定
            tmpQuan[test] = tmpQuan[test].toInt() + 1;
            if (msg.value(addr + OKNGIMPA).toInt() == 0) {
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
    int type = msg[DataFile].toInt();

    int addr = tmpSet.value((3000 + Qt::Key_0)).toInt();

    query.prepare("insert into aip_record values(?,?,?,?)");  // 测试日期
    query.addBindValue(uuid);
    query.addBindValue(addr + TEMPDATE);
    query.addBindValue(type);
    query.addBindValue(QDate::currentDate().toString("yyyy-MM-dd"));
    query.exec();

    query.prepare("insert into aip_record values(?,?,?,?)");  // 开始时间
    query.addBindValue(uuid);
    query.addBindValue(addr + TEMPPLAY);
    query.addBindValue(type);
    query.addBindValue(msg.value(addr + TEMPPLAY).toTime().toString("hh:mm:ss"));
    query.exec();

    query.prepare("insert into aip_record values(?,?,?,?)");  // 完成时间
    query.addBindValue(uuid);
    query.addBindValue(addr + TEMPSTOP);
    query.addBindValue(type);
    query.addBindValue(msg.value(addr + TEMPSTOP).toTime().toString("hh:mm:ss"));
    query.exec();

    query.prepare("insert into aip_record values(?,?,?,?)");  // 当前用户
    query.addBindValue(uuid);
    query.addBindValue(DataUser);
    query.addBindValue(type);
    query.addBindValue(msg.value(DataUser).toInt() - msg.value((2000 + Qt::Key_5)).toInt() + 1);
    query.exec();

    query.prepare("insert into aip_record values(?,?,?,?)");  // 当前温度
    query.addBindValue(uuid);
    query.addBindValue(addr + TEMPTEMP);
    query.addBindValue(type);
    query.addBindValue(msg.value(addr + TEMPTEMP).toString());
    query.exec();

    query.prepare("insert into aip_record values(?,?,?,?)");  // 当前工位
    query.addBindValue(uuid);
    query.addBindValue(addr + TEMPWORK);
    query.addBindValue(type);
    query.addBindValue(msg.value(addr + TEMPWORK).toInt() == 0x11 ? 1 : 2);
    query.exec();

    query.prepare("insert into aip_record values(?,?,?,?)");  // 当前编码
    query.addBindValue(uuid);
    query.addBindValue(addr + TEMPCODE);
    query.addBindValue(type);
    query.addBindValue(msg.value(addr + TEMPCODE).toString());
    query.exec();

    query.prepare("insert into aip_record values(?,?,?,?)");  // 总判定
    query.addBindValue(uuid);
    query.addBindValue(addr + TEMPISOK);
    query.addBindValue(type);
    query.addBindValue(msg.value(addr + TEMPISOK).toInt());
    query.exec();

    int c = msg[(4000 + Qt::Key_0)].toInt();
    int weld = msg[c].toInt();
    for (int t=0; t < 3; t++) {
        int test = msg[(4000 + Qt::Key_1) + t].toInt();
        int addr = msg[(3000 + Qt::Key_1) + t].toInt() + CACHEDCR;
        int quan = (t == 2) ? weld/2 : weld;
        if (msg[test].toInt() == 1) {
            for (int i=0; i < quan; i++) {
                double r = msg.value(addr + i*4 + DATADCRR).toDouble();
                double p = msg.value(addr + i*4 + GEARDCRR).toDouble();
                p = (p > 3) ? p - 3 : p;
                QString real = QString::number(r * qPow(10, -p), 'f', p);
                query.prepare("insert into aip_record values(?,?,?,?)");
                query.addBindValue(uuid);
                query.addBindValue(addr + i);
                query.addBindValue(type);
                query.addBindValue(real);
                query.exec();
            }
        }
    }
    for (int t=0; t < 4; t++) {
        int test = msg[(4000 + Qt::Key_4) + t].toInt();
        int addr = msg[(3000 + Qt::Key_4) + t].toInt() + CACHEACW;
        if (msg[test].toInt() == 1) {
            double r = msg[addr + DATAACWR].toDouble();
            double p = msg[addr + GEARACWR].toDouble();
            r *= qPow(10, -p);
            QString real = QString::number(r, 'f', 3);
            if (t == NUMBINRS) {
                real = (r > 500) ? "500" : (QString::number(r, 'f', 1));
            }
            query.prepare("insert into aip_record values(?,?,?,?)");
            query.addBindValue(uuid);
            query.addBindValue(addr);
            query.addBindValue(type);
            query.addBindValue(real);
            query.exec();
        }
    }
    if (1) {
        int test = msg[(4000 + Qt::Key_8)].toInt();
        int time = msg[test + AddrIMPSA].toInt();
        int addr = msg[(3000 + Qt::Key_8)].toInt() + CACHEIMP;  // 匝间结果地址 电压,结果,频率,判定
        int quan = (time*2 == weld) ? weld/2 : weld;
        if (msg[test + AddrIMPSC].toInt() == 1) {
            for (int i=0; i < quan; i++) {
                double r = msg[addr + i*4 + DATAIMPR].toDouble();
                QString real = QString::number(r/1000, 'f', r >= 10000 ? 2 : 3);
                query.prepare("insert into aip_record values(?,?,?,?)");
                query.addBindValue(uuid);
                query.addBindValue(addr + i);
                query.addBindValue(type);
                query.addBindValue(real);
                query.exec();
            }
        }
    }
    query.prepare("insert into aip_record values(?,?,?,?)");
    query.addBindValue(uuid);
    query.addBindValue(0xFFFF);
    query.addBindValue(type);
    query.addBindValue(weld);
    query.exec();
    QSqlDatabase::database("record").commit();
}

void SqlImport::recvAppMsg(QTmpMap msg)
{
    switch (msg.value(Qt::Key_0).toInt()) {
    case Qt::Key_Copy:
        tmpSet = msg;
        break;
    case Qt::Key_Save:
        if (msg.value(Qt::Key_1).toString() == "aip_record") {
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

