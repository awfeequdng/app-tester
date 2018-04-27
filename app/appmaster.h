/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       用户管理
*******************************************************************************/
#ifndef APPMASTER_H
#define APPMASTER_H

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

#define ADDR_MASTER 0x0100

#define M_ROW 15
#define M_COL 4

class AppMaster : public QWidget
{
    Q_OBJECT
public:
    explicit AppMaster(QWidget *parent = 0);
signals:
    void sendAppMap(QVariantMap msg);
private slots:
    void initUI();
    void initView();
    void initLayout();
    void initDelegate();
    void initSettings();
    void saveSettings();
    void clickButtons();
    void autoInput(int r, int c);
    void recvAppMap(QVariantMap msg);
    virtual void showEvent(QShowEvent *e);
private:
    QTableWidget *view;
    QVariantMap tmpMap;
    QVariantMap config;

    QLineEdit *page;
    QStringList groups;


public slots:
};

#endif // APPMASTER_H
