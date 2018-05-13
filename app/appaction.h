/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       权限管理
*******************************************************************************/
#ifndef APPACTION_H
#define APPACTION_H

#include <QDebug>
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

#include "boxqcombo.h"
#include "boxqitems.h"

#include "appdefine.h"
#include "appmaster.h"

const int AddrShow = 1020;

//const int AddrName = 0x00;
const int AddrMark = 0x01;
//const int AddrRole = 0x02;
const int AddrForm = 0x03;

#define A_ROW 25
#define A_COL 4

class AppAction : public QWidget
{
    Q_OBJECT
public:
    explicit AppAction(QWidget *parent = 0);
signals:
    void sendAppMap(QVariantMap msg);
    void sendAppMsg(QTmpMap msg);
private slots:
    void initUI();
    void initView();
    void initLayout();
    void initDelegate();
    void initSettings();
    void saveSettings();
    void recvAppMsg(QTmpMap msg);
    virtual void showEvent(QShowEvent *e);
private:
    QTableWidget *view;
    QStringList roles;
    QTmpMap tmpSet;
};

#endif // APPACTION_H
