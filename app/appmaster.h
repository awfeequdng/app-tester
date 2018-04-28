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
#include <QLabel>
#include <QLayout>
#include <QWidget>
#include <QVariant>
#include <QLineEdit>
#include <QGroupBox>
#include <QDateTime>
#include <QShowEvent>
#include <QHeaderView>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>

#include "boxqcombo.h"
#include "boxqitems.h"

#include "appdefine.h"

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
    void initLayout();
    void initViewBar();
    void initLineBar();
    void initButtons();
    void initDelegate();
    void initSettings();
    void saveSettings();
    void clickViewBar();
    void clickButtons();
    void appendMaster();
    void removeMaster();
    void recvAppMap(QVariantMap msg);
    virtual void showEvent(QShowEvent *e);
private:
    QVBoxLayout *boxLayout;
    QTableWidget *view;
    QVariantMap tmpMap;
    QVariantMap config;

    QLineEdit *page;
    QStringList groups;

    QLineEdit *lineNumb;
    QLineEdit *lineName;
    QLineEdit *linePass;
    QComboBox *boxGroup;


public slots:
};

#endif // APPMASTER_H
