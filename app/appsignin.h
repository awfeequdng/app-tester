/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       登录界面
*******************************************************************************/
#ifndef APPSIGNIN_H
#define APPSIGNIN_H

#include <QDebug>
#include <QLabel>
#include <QLayout>
#include <QDialog>
#include <QVariant>
#include <QDateTime>
#include <QGroupBox>
#include <QCheckBox>
#include <QListView>
#include <QComboBox>
#include <QLineEdit>
#include <QSettings>
#include <QShowEvent>
#include <QMessageBox>
#include <QPushButton>
#include <QToolButton>
#include <QStackedWidget>

#include "main.h"

#include "appmaster.h"
#include "apppermit.h"

#define TEMPSIGN  0x00  // 登录状态

class AppSignin : public QDialog
{
    Q_OBJECT
public:
    explicit AppSignin(QWidget *parent = 0);
signals:
    void sendAppMsg(QTmpMap msg);
    void sendAppMap(QVariantMap msg);
private slots:
    void initUI();
    void initLayout();
    void initTitleBar();
    void initLoginBar();
    void initInputBar();
    void initAboutBar();
    void initApplyBar();
    void initSettings();
    void saveSettings();
    void checkSignin();
    void clickLink(QString msg);
    void recvAppMsg(QTmpMap msg);
    virtual void showEvent(QShowEvent *e);
private:
    QFrame *title;
    QStackedWidget *stack;
    QComboBox *username;
    QLineEdit *password;
    QCheckBox *autosave;
    QCheckBox *autosign;
    QList<QLineEdit*> inputs;

    bool isOk;
    bool isAuto;
    QTmpMap tmpSet;
    QTmpMap tmpMsg;
};

#endif // APPSIGNIN_H
