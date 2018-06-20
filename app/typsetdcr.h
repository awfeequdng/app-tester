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

const int ISCKDCR1 = 0x00;
const int SMAXDCR1 = 0x01;
const int ISTMDCR1 = 0x02;
const int TEMPDCR1 = 0x03;
const int TIMEDCR1 = 0x04;
const int GEARDCR1 = 0x05;

const int ISCHDCR2 = 0x01;
const int SMAXDCR2 = 0x02;

const int ISCHDCR3 = 0x00;
const int SMINDCR3 = 0x01;
const int SMAXDCR3 = 0x02;
const int GEARDCR3 = 0x03;
const int RMINDCR3 = 0x04;
const int GMINDCR3 = 0x05;
const int RMAXDCR3 = 0x06;
const int GMAXDCR3 = 0x07;

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
    QDoubleSpinBox *boxTime;
    QDoubleSpinBox *maxTemp;
    QDoubleSpinBox *maxWeld;
    QDoubleSpinBox *maxChip;
    QDoubleSpinBox *maxDiag;
    QDoubleSpinBox *minDiag;
    QTmpMap tmpSet;
    QTmpMap tmpMsg;
    int currItem;
    QPushButton *btnSave;
    QPushButton *btnWeld;
    QPushButton *btnDiag;
};

#endif // TYPSETDCR_H
