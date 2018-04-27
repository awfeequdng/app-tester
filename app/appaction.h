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

#define ADDR_ACTION 0x0200
#define A_ROW 16
#define A_COL 4

class AppAction : public QWidget
{
    Q_OBJECT
public:
    explicit AppAction(QWidget *parent = 0);
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
    void recvAppMap(QVariantMap msg);
    virtual void showEvent(QShowEvent *e);
private:
    QTableWidget *view;
    QVariantMap tmpMap;
    QVariantMap config;
    QLineEdit *page;
    QStringList groups;
};

#endif // APPACTION_H
