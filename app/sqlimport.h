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

#define SD_SIZE 800

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
};

#endif // SQLIMPORT_H
