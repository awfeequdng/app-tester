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
#include "devsetcan.h"

const int AddrTypeC = 2000;  // 当前配置地址

const int AddrModel = 1050;  // 综合配置地址
const int AddrDCRS1 = 1051;  // 片机配置地址
const int AddrDCRS2 = 1052;  // 焊接配置地址
const int AddrDCRS3 = 1053;  // 跨间配置地址
const int AddrACWS1 = 1054;  // 绝缘配置地址
const int AddrACWS2 = 1055;  // 轴铁配置地址
const int AddrACWS3 = 1056;  // 轴线配置地址
const int AddrACWS4 = 1057;  // 铁线配置地址
const int AddrIMPS1 = 1058;  // 匝间配置地址
const int AddrIMPSW = 1059;  // 标准波形地址

const int AddrDCRSC = 0x00;
const int AddrDEVSC = 0x01;

#define C_ROW 16
#define C_COL 2

class TypConfig : public QWidget
{
    Q_OBJECT
public:
    explicit TypConfig(QWidget *parent = 0);
signals:
    void sendAppMap(QVariantMap msg);
    void sendAppMsg(QTmpMap msg);
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
    void recvAppMsg(QTmpMap msg);
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

    QTmpMap tmpSet;
};

#endif // TYPCONFIG_H
