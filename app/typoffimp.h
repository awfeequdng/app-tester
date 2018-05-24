/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       匝间后台配置界面
*******************************************************************************/
#ifndef TYPOFFIMP_H
#define TYPOFFIMP_H

#include <QLabel>
#include <QLayout>
#include <QWidget>
#include <QVariant>
#include <QGroupBox>
#include <QLineEdit>
#include <QShowEvent>
#include <QMessageBox>
#include <QPushButton>

#include "appdefine.h"

class TypOffImp : public QWidget
{
    Q_OBJECT
public:
    explicit TypOffImp(QWidget *parent = 0);
signals:
    void sendAppMsg(QTmpMap msg);
private slots:
    void initUI();
    void initLayout();
    void initBoxINR();
    void initSettings();
    void zeroINR();
    void calcINR();
    void recvAppMsg(QTmpMap msg);
    virtual void showEvent(QShowEvent *e);
private:
    QGroupBox *inr;
    QGroupBox *acw;
    QList<QPushButton*> inrs;
    QList<QPushButton*> acws;
    QList<QLineEdit*> inrboxs;
    QList<QLineEdit*> acwboxs;
    QTmpMap tmpSet;
    QTmpMap tmpMsg;
};

#endif // TYPOFFIMP_H
