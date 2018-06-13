/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       介电强度后台配置界面
*******************************************************************************/
#ifndef TYPOFFACW_H
#define TYPOFFACW_H

#include <QLabel>
#include <QDebug>
#include <QLayout>
#include <QWidget>
#include <QVariant>
#include <QGroupBox>
#include <QLineEdit>
#include <QShowEvent>
#include <QMessageBox>
#include <QPushButton>

#include "main.h"

class TypOffAcw : public QWidget
{
    Q_OBJECT
public:
    explicit TypOffAcw(QWidget *parent = 0);
signals:
    void sendAppMsg(QTmpMap msg);
private slots:
    void initUI();
    void initLayout();
    void initBoxINR();
    void initBoxACW();
    void initSettings();
    void zeroINR();
    void calcINR();
    void zeroACW();
    void calcACW();
    void recvAppMsg(QTmpMap msg);
    virtual void showEvent(QShowEvent *e);
private:
    QGroupBox *inr;
    QGroupBox *acw;
    QTmpMap tmpSet;
    QTmpMap tmpMsg;
    QList<QPushButton*> inrs;
    QList<QPushButton*> acws;
    QList<QLineEdit*> inrboxs;
    QList<QLineEdit*> acwboxs;
};

#endif // TYPOFFACW_H
