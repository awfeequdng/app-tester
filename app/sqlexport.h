/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       数据导出
*******************************************************************************/
#ifndef SQLEXPORT_H
#define SQLEXPORT_H

#include <QFile>
#include <QDate>
#include <QTimer>
#include <QDebug>
#include <QWidget>
#include <QObject>
#include <QProcess>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlDatabase>
#include <QElapsedTimer>
#include <QSqlQueryModel>
#include <QTextCodec>
#include <QSqlTableModel>
#include <QMessageBox>

#include "main.h"
#include "typsetdcr.h"

#ifdef __arm__
#define LENTH 10000
#else
#define LENTH 10000
#endif


class SqlExport : public QObject
{
    Q_OBJECT
public:
    explicit SqlExport(QObject *parent = 0);

signals:
    void sendAppMsg(QTmpMap msg);
private slots:
    int exportFile(QTmpMap msg);
    int createFile(QTmpMap msg);
    int selectNumb(QTmpMap msg);
    int selectItem(QTmpMap msg);
    int exportHead(QTmpMap msg);
    int exportData(QTmpMap msg);
    QByteArray ToGbk(const QString &inStr);
    void showText(QString err);
    void recvAppMsg(QTmpMap msg);
private:
    QSqlDatabase db;
    int quan;
    QList<int> numbs;
    QFile *file;
    QTmpMap tmpSet;
    QTmpMap tmpMsg;
};

#endif // SQLEXPORT_H
