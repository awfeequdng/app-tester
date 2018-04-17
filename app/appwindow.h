/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       电枢测试仪主界面
*******************************************************************************/
#ifndef APPWINDOW_H
#define APPWINDOW_H

#include <QFile>
#include <QLayout>
#include <QDateTime>
#include <QMainWindow>
#include <QApplication>
#include <QStackedWidget>

#include "appauthor.h"

class AppWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit AppWindow(QWidget *parent = 0);
private slots:
    void initUI();
    void initSkin();
    void initTitle();
    void initLayout();
    void initAuthor();
    void recvAppMap(QVariantMap msg);
private:
    QStackedWidget *stack;
    QVBoxLayout *btnLayout;
};

#endif // APPWINDOW_H
