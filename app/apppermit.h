/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       权限管理
*******************************************************************************/
#ifndef APPPERMIT_H
#define APPPERMIT_H

#include <QDebug>
#include <QLabel>
#include <QLayout>
#include <QWidget>
#include <QVariant>
#include <QLineEdit>
#include <QGroupBox>
#include <QShowEvent>
#include <QHeaderView>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>

#include "appsignin.h"
#include "boxqitems.h"
#include "main.h"

const int nAuthor = 0x00;
const int nTester = 0x01;

const int nSignin = 0x00;
const int nSystem = 0x01;
const int nOnline = 0x02;
const int nMaster = 0x03;
const int nPermit = 0x04;
const int nBackup = 0x05;
const int nLogger = 0x06;
const int nIoctrl = 0x07;

const int nConfig = 0x00;
const int nSetDCR = 0x01;
const int nSetMAG = 0x02;
const int nSetINR = 0x03;
const int nSetACW = 0x04;
const int nSetDCW = 0x05;
const int nSetIMP = 0x06;
const int nSetPWR = 0x07;
const int nSetIND = 0x08;
const int nSetLCK = 0x09;
const int nSetLVS = 0x0A;
const int nSetHAL = 0x0B;
const int nSetLOD = 0x0C;
const int nSetNLD = 0x0D;
const int nSetEMF = 0x0E;

const int nRecord = 0x00;
const int nUpload = 0x01;
const int nSdcard = 0x02;

class AppPermit : public QWidget
{
    Q_OBJECT
public:
    explicit AppPermit(QWidget *parent = 0);
signals:
    void sendAppMsg(QTmpMap msg);
    void sendAppMap(QVariantMap msg);
private slots:
    void initUI();
    void initLayout();
    void initViewBar();
    void initButtonBar();
    void initItemDelegate();
    void initSettings();
    void saveSettings();
    void autoChange();
    void change();
    void recvAppMsg(QTmpMap msg);
    virtual void showEvent(QShowEvent *e);
    virtual void hideEvent(QHideEvent *e);
private:
    QVBoxLayout *layout;
    QTableWidget *view;
    QStringList gRole;
    QTmpMap tmpSet;
    QTmpMap tmpMsg;
    QTmpMap screen;
    bool isInit;
};

#endif // APPPERMIT_H
