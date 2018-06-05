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



const int AddrName = 0x00;
const int AddrPass = 0x01;
const int AddrRole = 0x02;
const int AddrLast = 0x03;
const int AddrSave = 0x04;

#define M_ROW 15

class AppMaster : public QWidget
{
    Q_OBJECT
public:
    explicit AppMaster(QWidget *parent = 0);
signals:
    void sendAppMsg(QTmpMap msg);
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
    void appendMaster();
    void removeMaster();
    void recvAppMsg(QTmpMap msg);
    virtual void showEvent(QShowEvent *e);
private:
    QHBoxLayout *boxLayout;
    QVBoxLayout *btnLayout;
    QTableWidget *view;
    QLineEdit *lineNumb;
    QLineEdit *lineName;
    QLineEdit *linePass;
    QComboBox *boxGroup;
    QStringList roles;
    QTmpMap tmpSet;
};

#endif // APPMASTER_H
