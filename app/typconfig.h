/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       型号管理界面
*******************************************************************************/
#ifndef TYPCONFIG_H
#define TYPCONFIG_H

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
#include <QSplitter>
#include <QComboBox>

#include <QSqlQuery>
#include <QSqlError>
#include <QStringList>
#include <QSqlDatabase>

#include "boxqitems.h"

#include "appdefine.h"

#define C_ROW 16
#define C_COL 2

class TypConfig : public QWidget
{
    Q_OBJECT
public:
    explicit TypConfig(QWidget *parent = 0);
signals:
    void sendAppMap(QVariantMap msg);
private slots:
    void initUI();
    void initLayout();
    void initViewBar();
    void initConfigBar();
    void initButtonBar();
    void initSettings();
    void saveSettings();
    void appendModelType();
    void selectModelType();
    void deleteModelType();
    void clickButtons();
    void clickViewBar();
    void recvAppMap(QVariantMap msg);
    virtual void showEvent(QShowEvent *e);
private:
    QTableWidget *view;
    QVariantMap tmpMap;
    QVariantMap config;
    QLineEdit *page;
    QSplitter *splitter;
    QVBoxLayout *boxLayout;
    QFrame *setFrame;
    int isShow;
    QStringList names;
    QTableWidget *settings;
};

#endif // TYPCONFIG_H
