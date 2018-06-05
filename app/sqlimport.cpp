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

void SqlImport::recvAppDat(QTmpMap msg)
{
    QSqlDatabase::database("record").transaction();
    QSqlQuery query(QSqlDatabase::database("record"));
    SqlSnowId snow;
    quint64 guid = snow.getId();
    int c = msg[AddrModel].toInt();
    int weld = msg[c + 1].toInt();
    for (int t=0; t < 3; t++) {
        int test = msg[AddrDCRS1 + t].toInt();
        int addr = msg[AddrDCRR1 + t].toInt();
        if (msg[test].toInt() == 1) {
            for (int i=0; i < weld; i++) {
                query.prepare("insert into aip_record values(?,?,?,?)");
                query.addBindValue(snow.getId());
                query.addBindValue(guid);
                query.addBindValue(addr + i);
                query.addBindValue(msg.value(addr + i).toInt());
                query.exec();
            }
        }
    }
    for (int t=0; t < 4; t++) {
        int test = msg[AddrACWS1 + t].toInt();
        if (msg[test].toInt() == 1) {
            int addr = msg[AddrACWR1 + t].toInt();  // 高压结果地址
            for (int i=0; i < 4; i++) {
                query.prepare("insert into aip_record values(?,?,?,?)");
                query.addBindValue(snow.getId());
                query.addBindValue(guid);
                query.addBindValue(addr + i);
                query.addBindValue(msg.value(addr + i).toInt());
                query.exec();
            }
        }
    }
    if (1) {
        int test = msg[AddrIMPS1].toInt();
        int addr = msg[AddrIMPR1].toInt();
        if (msg[test].toInt() == 1) {
            for (int i=0; i < weld; i++) {
                query.prepare("insert into aip_record values(?,?,?,?)");
                query.addBindValue(snow.getId());
                query.addBindValue(guid);
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
    case Qt::Key_Book:
        recvAppDat(msg);
        break;
    default:
        break;
    }
}

