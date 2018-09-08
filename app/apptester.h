/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       测试界面
*******************************************************************************/
#ifndef APPTESTER_H
#define APPTESTER_H

#include <qmath.h>
#include <QLabel>
#include <QWidget>
#include <QLayout>
#include <QString>
#include <QSplitter>
#include <QGroupBox>
#include <QTableView>
#include <QLCDNumber>
#include <QMessageBox>
#include <QPushButton>
#include <QHeaderView>
#include <QTextBrowser>
#include <QTableWidget>
#include <QApplication>
#include <QGraphicsView>
#include <QElapsedTimer>
#include <QGraphicsScene>
#include <QStandardItemModel>

#include "boxqchart.h"
#include "boxqlabel.h"

#include "main.h"

#include "appsignin.h"
#include "devsetcan.h"
#include "typconfig.h"
#include "typsetdcr.h"
#include "typsetacw.h"
#include "typsetimp.h"
#include "sqlimport.h"

#include "boxqimage.h"

#define NUMS 72

const QString StyleOK = "<p style='font-size:10px;color:#FFFFFF;line-height:14px;'align='left'>";
const QString StyleNG = "<p style='font-size:10px;color:#FF0000;line-height:14px;'align='left'>";
const QString SmallOK = StyleOK + "<b>%1&nbsp;</b></p>";
const QString SmallNG = StyleNG + "<b>%1&nbsp;</b></p>";

const QString titleOK = "<p style='font-size:11pt;color:#FFFFFF;' align='left'>";
const QString titleNG = "<p style='font-size:11pt;color:#666666;' align='left'>";
const QString largeOK = "<p style='font-size:15pt;color:#00FF00;' align='center'>%1</p>";
const QString largeNG = "<p style='font-size:15pt;color:#FF0000;' align='center'>%1</p>";
const QString largeEN = "<p style='font-size:15pt;color:#666666;' align='center'>%1</p>";

class AppTester : public QWidget
{
    Q_OBJECT
public:
    explicit AppTester(QWidget *parent = 0);
signals:
    void sendAppMsg(QTmpMap msg);
private slots:
    void initUI();
    void initSkin();
    void initLayout();
    void initStatus();
    void initWorker();
    void initResult();
    void initQuRate();
    void initQChart();
    void initButton();
    void initTextAll();
    void initTextDCR1();
    void initTextDCR2();
    void initTextDCR3();
    void initWaveDCRW();
    void initTextACW4();
    void initTextACW1();
    void initTextACW2();
    void initTextACW3();
    void initTextIMP1();
    void initWaveIMPW();
    void initSettings();
    void initTextView();
    void drawWaveDCRW(QTmpMap msg);
    void drawWaveIMPW(int numb);
    void showViewSize();
    void drawAllRate();
    void boxResize();
    void dcrResize();
    void impResize();
    void clickText();
    void updateText();
    void clickStart();
    void clickButton();

    void recvTmpMsg(QTmpMap msg);
    void recvLedMsg(QTmpMap msg);
    void recvDCRMsg(QTmpMap msg);
    void recvACWMsg(QTmpMap msg);
    void recvIMPMsg(QTmpMap msg);
    void recvUpdate(QTmpMap msg);
    void recvAppMsg(QTmpMap msg);
    virtual void showEvent(QShowEvent *e);
private:
    QTableWidget *status;
    QVBoxLayout *mLayout;
    QTableWidget *view;
    QLabel *textResult;
    QLabel *textWorker;
    quint32 testOK;
    quint32 testQu;

    QList<QLabel*> dcrTitles;
    QList<QLabel*> dcrLabels;
    QList<QLabel*> acwLabels;
    QList<QLabel*> acwTitles;
    QList<QLabel*> impLabels;
    QList<QLabel*> impTitles;

    BoxQImage *textRA;
    BoxQImage *textDCR1;
    BoxQImage *textDCR2;
    BoxQImage *textDCR3;
    BoxQImage *textIMPR;

    QPushButton *btnHome;
    QPushButton *btnConf;
    QPushButton *btnPlay;

    BoxQChart *boxChart;

    QList<int> tmpWave;
    QList<int> impWave;
    QList<double> diff;
    double impPrev;
    double min;
    int minb;
    QElapsedTimer t;
    QTmpMap tmpSet;
    QTmpMap tmpMsg;

    BoxQLabel *impView;
    BoxQLabel *dcrView;
    BoxQLabel *allView;
    QList<int> tmpDcr;
    QList<int> tmpPow;
    QStringList tmpImp;
    QVariantMap tmpMap;
};

#endif // APPTESTER_H
