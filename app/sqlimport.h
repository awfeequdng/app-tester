/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       数据导入
*******************************************************************************/
#ifndef SQLIMPORT_H
#define SQLIMPORT_H

#include <QTimer>
#include <QObject>
#include <QProcess>
#include <QSqlQuery>
#include <QSqlError>
#include <QElapsedTimer>

#include "sqlsnowid.h"
#include "main.h"
#include "typsetimp.h"

#define DATAOK  0x00
#define DATANG  0x01

#define TEMPDATE 0x01  // 测试日期
#define TEMPPLAY 0x02  // 开始时间
#define TEMPSTOP 0x03  // 完成时间
#define TEMPWORK 0x04  // 当前工位
#define TEMPCODE 0x05  // 条码状态
#define TEMPTEMP 0x06  // 当前温度
#define TEMPISOK 0x07  // 测试判定

#define SD_SIZE 80000

class SqlImport : public QObject
{
    Q_OBJECT
public:
    explicit SqlImport(QObject *parent = 0);

signals:
    void sendAppMsg(QTmpMap msg);
private slots:
    void initData();
    void initFlash();
    bool readRecord();
    bool readSdcard();
    bool initBackup();
    bool initRecord();
    void saveSqlite(QTmpMap msg);
    void saveRecord(QTmpMap msg);
    void recvAppMsg(QTmpMap msg);
private:
    QTmpMap tmpQuan;
    QTmpMap tmpOKNG;
    QElapsedTimer t;
    QTmpMap tmpMap;
    QTmpMap tmpSet;
};

#endif // SQLIMPORT_H
