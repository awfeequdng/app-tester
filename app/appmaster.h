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
#include <QListView>
#include <QLineEdit>
#include <QGroupBox>
#include <QComboBox>
#include <QShowEvent>
#include <QHeaderView>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>

#include "boxqitems.h"
#include "main.h"

const int mName = 0x00;
const int mPass = 0x01;
const int mRole = 0x02;
const int mLast = 0x03;
const int mSave = 0x04;

class AppMaster : public QWidget
{
    Q_OBJECT
public:
    explicit AppMaster(QWidget *parent = 0);
signals:
    void sendAppMsg(QTmpMap msg);
    void sendAppMap(QVariantMap msg);
private slots:
    void initUI();
    void initLayout();
    void initViewBar();
    void initLineBar();
    void initDelegate();
    void initSettings();
    void saveSettings();
    void clickViewBar();
    void recvAppMsg(QTmpMap msg);
    virtual void showEvent(QShowEvent *e);
private:
    QVBoxLayout *layout;
    QTableWidget *view;
    QLineEdit *iName;
    QLineEdit *iPass;
    QComboBox *iRole;
    QStringList gRole;
    QTmpMap tmpSet;
    QTmpMap tmpMsg;
};

#endif // APPMASTER_H
