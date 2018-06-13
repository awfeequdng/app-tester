/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       介电强度配置界面
*******************************************************************************/
#ifndef TYPSETACW_H
#define TYPSETACW_H

#include <QLabel>
#include <QDebug>
#include <QLayout>
#include <QWidget>
#include <QVariant>
#include <QCheckBox>
#include <QGroupBox>
#include <QComboBox>
#include <QLineEdit>
#include <QTableView>
#include <QHeaderView>
#include <QShowEvent>
#include <QMessageBox>
#include <QPushButton>
#include <QDoubleSpinBox>

#include "boxqmodel.h"
#include "boxqitems.h"
#include "boxdouble.h"
#include "main.h"
#include "typconfig.h"

const int AddrACWSC = 0x00;  // 测试
const int AddrACWSN = 0x01;  // 名称
const int AddrACWSV = 0x02;  // 电压
const int AddrACWST = 0x03;  // 时间
const int AddrACWSH = 0x04;  // 上限
const int AddrACWSL = 0x05;  // 下限
const int AddrACWSO = 0x06;  // 补偿
const int AddrACWSA = 0x07;  // 电弧

#define LL 97

class TypSetAcw : public QWidget
{
    Q_OBJECT
public:
    explicit TypSetAcw(QWidget *parent = 0);
signals:
    void sendAppMsg(QTmpMap msg);
private slots:
    void initUI();
    void initLayout();
    void initInrView();
    void initAcwView();
    void initButtons();
    void initDelegate();
    void initSettings();
    void saveSettings();
    void recvAppMsg(QTmpMap msg);
    virtual void showEvent(QShowEvent *e);
private:
    QVBoxLayout *boxLayout;
    QTableView *inrView;
    BoxQModel *mInrView;
    QTableView *acwView;
    BoxQModel *mAcwView;
    QTmpMap tmpSet;
};

#endif // TYPSETACW_H
