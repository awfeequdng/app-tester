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

#include "lib/qcustomplot.h"
#include "boxqchart.h"

#include "appdefine.h"

#include "devsetcan.h"
#include "typconfig.h"
#include "typsetdcr.h"
#include "typsetacw.h"
#include "typsetimp.h"

#define NUMS 72

const QString StyleOK = "<p style='font-size:10px;color:#FFFFFF;line-height:14px;'align='left'>";
const QString StyleNG = "<p style='font-size:10px;color:#FF0000;line-height:14px;'align='left'>";
const QString SmallOK = StyleOK + "<b>%1&nbsp;</b></p>";
const QString SmallNG = StyleNG + "<b>%1&nbsp;</b></p>";

const QString titleOK = "<p style='font-size:11pt;color:#FFFFFF;' align='left'>";
const QString titleNG = "<p style='font-size:11pt;color:#666666;' align='left'>";
const QString largeOK = "<p style='font-size:15pt;color:#00FF00;' align='center'>%1</p>";
const QString largeNG = "<p style='font-size:15pt;color:#FF0000;' align='center'>%1</p>";
const QString largeON = "<p style='font-size:15pt;color:#666666;' align='center'>%1</p>";

const QString judgeOK = "<p style='font-size:64pt;color:#00FF00;'align='center'><b>OK</b></p>";
const QString judgeNG = "<p style='font-size:64pt;color:#FF0000;'align='center'><b>NG</b></p>";
const QString judgeON = "<p style='font-size:64pt;color:#FFFF00;'align='center'><b>%1</b></p>";

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
    void initQChart();
    void initButton();
    void initDcrWeld();
    void initDcrChip();
    void initDcrDiag();
    void initDcrWave();
    void drawDcrWave();
    void initInrTextCG();
    void initAcwTextAC();
    void initAcwTextAL();
    void initAcwTextLC();
    void initImpText();
    void initImpWave();
    void initSettings();
    void drawImpWave();

    void setViewSize();
    void initHistogram();
    void drawHistogram();
    void boxResize();
    void dcrResize();
    void impResize();
    void clickStart();
    void clickButton();
    void recvLedMsg(QTmpMap msg);
    void recvUpdate(QTmpMap msg);
    void recvAppMsg(QTmpMap msg);
    virtual void showEvent(QShowEvent *e);
private:
    QTableWidget *status;
    QVBoxLayout *mLayout;
    QCustomPlot *dcrView;
    QCustomPlot *impView;
    QTableWidget *view;
    QLabel *textResult;
    QLabel *textWorker;
    QCustomPlot *histogram;
    QList<QCPBars*> bars;
    quint32 countOk;
    quint32 countAll;

    QList<QLabel*> dcrTitles;
    QList<QLabel*> dcrLabels;
    QList<QLabel*> acwLabels;
    QList<QLabel*> acwTitles;
    QList<QLabel*> impLabels;
    QList<QLabel*> impTitles;

    QTextBrowser *textWeld;
    QTextBrowser *textChip;
    QTextBrowser *textIMPR;

    QPushButton *btnPlay;

    BoxQChart *boxChart;

    QList<int> tmpWave;
    QList<int> impWave;
    QList<double> diff;
    double prev;
    double min;
    int minb;
    QCPGraph *impLine;
    QElapsedTimer t;
    QTmpMap tmpSet;
    QTmpMap tmpMsg;
};

#endif // APPTESTER_H
