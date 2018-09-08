/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       系统设置
*******************************************************************************/
#ifndef APPSYSTEM_H
#define APPSYSTEM_H

#include <QLabel>
#include <QTimer>
#include <QDebug>
#include <QLayout>
#include <QWidget>
#include <QVariant>

#include <QListView>
#include <QComboBox>
#include <QGroupBox>
#include <QLineEdit>
#include <QSettings>
#include <QTableView>
#include <QShowEvent>
#include <QHeaderView>
#include <QMessageBox>
#include <QPushButton>
#include <QDateTimeEdit>

#include "main.h"

#include "boxqitems.h"
#include "boxqmodel.h"
#include "boxdouble.h"

const int SystArea = 0x00;
const int SystTest = 0x01;
const int SystMode = 0x02;
const int SystRate = 0x03;
const int SystBeep = 0x04;
const int SystCode = 0x05;
const int SystSize = 0x06;
const int SystTime = 0x07;
const int SystWarn = 0x08;
const int SystWait = 0x09;

class AppSystem : public QWidget
{
    Q_OBJECT
public:
    explicit AppSystem(QWidget *parent = 0);
signals:
    void sendAppMsg(QTmpMap msg);
private slots:
    void initUI();
    void initLayout();
    void initSystem();
    void initDevice();
    void initButton();
    void initDelegate();
    void initSettings();
    void saveSettings();
    void setLocalTime();
    void autoChange();
    void change();
    void beep();
    void stop();
    void recvAppMsg(QTmpMap msg);
    virtual void showEvent(QShowEvent *e);
private:
    QHBoxLayout *layout;
    QVBoxLayout *boxLayout;
    QTableView *view;
    BoxQModel *mView;
    QList<QComboBox*> texts;
    QList<QLineEdit*> ctrls;
    QDateTimeEdit *time;
    QTmpMap tmpSet;
    QTmpMap tmpMsg;
    bool isInit;
    QStringList areas;
    QStringList modes;
    QStringList tests;
};

#endif // APPSYSTEM_H
