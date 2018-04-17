/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       电枢测试仪主页
*******************************************************************************/
#ifndef APPAUTHOR_H
#define APPAUTHOR_H

#include <QLabel>
#include <QLayout>
#include <QWidget>
#include <QVariant>
#include <QShowEvent>
#include <QMessageBox>
#include <QPushButton>

class AppAuthor : public QWidget
{
    Q_OBJECT
public:
    explicit AppAuthor(QWidget *parent = 0);
signals:
    void sendAppMap(QVariantMap msg);
private slots:
    void initUI();
    void initSkin();
    void initLayout();
    void initButton();
    void clickButton();
    virtual void showEvent(QShowEvent *e);
private:
    QVariantMap tmpMap;
    QHBoxLayout *btnLayout;
};

#endif // APPAUTHOR_H
