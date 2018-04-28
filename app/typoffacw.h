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
#include <QLayout>
#include <QWidget>
#include <QVariant>
#include <QShowEvent>
#include <QMessageBox>
#include <QPushButton>

class TypOffAcw : public QWidget
{
    Q_OBJECT
public:
    explicit TypOffAcw(QWidget *parent = 0);
signals:
    void sendAppMap(QVariantMap msg);
};

#endif // TYPOFFACW_H
