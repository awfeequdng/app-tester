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

#include <QLayout>
#include <QDateTime>
#include <QMainWindow>
#include <QStackedWidget>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

#include "devsetkey.h"

#include "appauthor.h"
#include "appsignin.h"
#include "appsystem.h"
#include "appbackup.h"
#include "applogger.h"
#include "appmaster.h"
#include "appaction.h"
#include "typconfig.h"
#include "typsetall.h"
#include "typsetdcr.h"
#include "typsetacw.h"
#include "typsetimp.h"
#include "typoffdcr.h"
#include "typoffacw.h"
#include "typoffimp.h"
#include "sqlcreate.h"
#include "sqlrecord.h"
#include "sqlupload.h"
#include "sqlsdcard.h"
#include "sqlunqual.h"
#include "apptester.h"

#include "boxdialog.h"

#define WIDTH 150

class AppWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit AppWindow(QWidget *parent = 0);
signals:
    void sendAppMap(QVariantMap msg);
private slots:
    void initUI();
    void initSql();
    void initTitle();
    void initLayout();
    void initAuthor();
    void initDevice();
    void initScreen();
    void initSignin();
    void initSystem();
    void initBackup();
    void initLogger();
    void initMaster();
    void initAction();
    void initConfig();
    void initSetAll();
    void initSetDcr();
    void initSetAcw();
    void initSetImp();
    void initOffDcr();
    void initOffAcw();
    void initOffImp();
    void initRecord();
    void initUpload();
    void initSdcard();
    void initUnqual();
    void initTester();
    void initButton(QString title, QString name);
    void showBoxPop(QString text, int t);
    void readSettings();
    void readModelSet();
    void initSettings();
    void sendSettings();
    void saveSettings();
    void saveModelSet();
    void clickButtons();
    void showTester();
    bool checkAction(QString msg);
    void screensShow(QString msg);
    void recvAppShow(QString msg);
    virtual void cloudAntimation();
    void recvAppMap(QVariantMap msg);
    void wait(int ms);
private:
    QStackedWidget *stack;
    QVBoxLayout *btnLayout;
    QFrame *btnFrame;
    QList<QPushButton*> buttons;
    QVariantMap config;
    QVariantMap tmpMap;

    QStringList sources;
    QStringList masters;
    QList<int> sgroups;
    QList<int> saction;
    QList<int> maction;

    QString verNumb;
    BoxDialog *boxpop;
    bool isSignin;

    int preindex;
};

#endif // APPWINDOW_H
