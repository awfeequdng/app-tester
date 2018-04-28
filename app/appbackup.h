/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       后台管理
*******************************************************************************/
#ifndef APPBACKUP_H
#define APPBACKUP_H

#include <QLabel>
#include <QLayout>
#include <QWidget>
#include <QVariant>
#include <QGroupBox>
#include <QLineEdit>
#include <QShowEvent>
#include <QPushButton>
#include <QMessageBox>

#include "appdefine.h"

class AppBackup : public QWidget
{
    Q_OBJECT
public:
    explicit AppBackup(QWidget *parent = 0);
signals:
    void sendAppMap(QVariantMap msg);
private slots:
    void initUI();
    void initLayout();
    void initBoxText();
    void initSettings();
    void saveSettings();
    void recvAppMap(QVariantMap msg);
    virtual void showEvent(QShowEvent *e);
private:
    QHBoxLayout *boxLayout;
    QList<QLineEdit*> texts;
    QVariantMap config;
};

#endif // APPBACKUP_H
