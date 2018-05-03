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
#include "appdefine.h"

#define LL 97

class TypSetAcw : public QWidget
{
    Q_OBJECT
public:
    explicit TypSetAcw(QWidget *parent = 0);
signals:
    void sendAppMap(QVariantMap msg);
private slots:
    void initUI();
    void initLayout();
    void initInrView();
    void initAcwView();
    void initButtons();
    void initDelegate();
    void initSettings();
    void saveSettings();
    void recvAppMap(QVariantMap msg);
    virtual void showEvent(QShowEvent *e);
private:
    QVBoxLayout *boxLayout;
    QVariantMap config;

    QTableView *inrView;
    BoxQModel *mInrView;
    QTableView *acwView;
    BoxQModel *mAcwView;
};

#endif // TYPSETACW_H
