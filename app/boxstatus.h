/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       提示框
*******************************************************************************/
#ifndef BOXSTATUS_H
#define BOXSTATUS_H

#include <QFile>
#include <QDebug>
#include <QLabel>
#include <QDialog>
#include <QLayout>
#include <QPushButton>
#include <QShowEvent>
#include <QHideEvent>
#include <QProgressBar>
#include <QApplication>
#include <QPropertyAnimation>

class BoxStatus : public QDialog
{
    Q_OBJECT
public:
    explicit BoxStatus(QWidget *parent = 0);
public slots:
    void setLabelText(QString text);
private slots:
    void initUI();
private:
    QLabel *logoLabel;
    QLabel *textLabel;
};

#endif // BOXSTATUS_H
