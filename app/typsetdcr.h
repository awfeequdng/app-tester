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

#include "appdefine.h"
#include "typconfig.h"

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
    void saveSettings();
    void clickButtons();
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
    QComboBox *grade;
    QTmpMap tmpSet;
    QTmpMap tmpMsg;
};

#endif // TYPSETDCR_H
