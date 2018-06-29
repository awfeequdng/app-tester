/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       电阻配置界面
*******************************************************************************/
#ifndef TYPSETDCR_H
#define TYPSETDCR_H

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
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QListView>

#include "main.h"
#include "typconfig.h"

#define ISCKDCR1 0x00  // 片间测试
#define SMAXDCR1 0x01  // 片间上限
#define ISTMDCR1 0x02  // 片间补偿
#define TEMPDCR1 0x03  // 片间温度
#define TIMEDCR1 0x04  // 片间时间
#define GEARDCR1 0x05  // 片间档位
#define TURNDCR1 0x06  // 片间顺序

#define ISCHDCR2 0x00  // 焊接测试
#define SMAXDCR2 0x01  // 焊接上限

#define ISCHDCR3 0x00  // 跨间测试
#define SMINDCR3 0x01  // 跨间下限
#define SMAXDCR3 0x02  // 跨间上限

#define CACHEDCR 0x10  // 离散数据长度

#define HARDDCRA 0x00  // 版本
#define TEMPDCRA 0x01  // 温度
#define STATDCRA 0x02  // 状态
#define OKNGDCRA 0x03  // 判定
#define TURNDCRA 0x04  // 顺序
#define FROMDCRA 0x05  // 起点
#define TURNISOK 0x06  // 顺序判定

#define NUMBDCRR 0x00  // 电阻编号
#define DATADCRR 0x01  // 电阻结果
#define GEARDCRR 0x02  // 电阻档位
#define OKNGDCRR 0x03  // 电阻判定

class TypSetDcr : public QWidget
{
    Q_OBJECT
public:
    explicit TypSetDcr(QWidget *parent = 0);
signals:
    void sendAppMsg(QTmpMap msg);
private slots:
    void initUI();
    void initLayout();
    void initCompBar();
    void initWeldBar();
    void initChipBar();
    void initDiagBar();
    void initViewBar();
    void initButtons();
    void initSettings();
    void initViewData();
    void saveSettings();
    void sample();
    void recvUpdate(QTmpMap msg);
    void recvAppMsg(QTmpMap msg);
    virtual void showEvent(QShowEvent *e);
private:
    QVBoxLayout *boxLayout;
    QTableWidget *view;
    QCheckBox *boxTemp;
    QCheckBox *boxWeld;
    QCheckBox *boxChip;
    QCheckBox *boxDiag;
    QComboBox *boxTurn;
    QDoubleSpinBox *boxTime;
    QDoubleSpinBox *maxTemp;
    QDoubleSpinBox *maxWeld;
    QDoubleSpinBox *maxChip;
    QDoubleSpinBox *maxDiag;
    QDoubleSpinBox *minDiag;
    QTmpMap tmpSet;
    QTmpMap tmpMsg;
    QPushButton *btnSave;
    QPushButton *btnWeld;
};

#endif // TYPSETDCR_H
