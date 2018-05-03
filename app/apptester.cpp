/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       测试界面
*******************************************************************************/
#include "apptester.h"

AppTester::AppTester(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void AppTester::initUI()
{
    initSkin();
    initLayout();
    initStatus();
    initWorker();
    initHistogram();
    drawHistogram();
    initResult();
    initQChart();
    initButton();
    initDcrWeld();
    initDcrChip();
    initDcrDiag();
    initDcrWave();
    drawDcrWave();
    initInrTextCG();
    initAcwTextAC();
    initAcwTextAL();
    initAcwTextLC();
    initImpText();
    initImpWave();
    drawImpWave();
}

void AppTester::initSkin()
{
    this->setStyleSheet(".QGroupBox{border:none;}");
}

void AppTester::initLayout()
{
    view = new QTableWidget(this);
    view->horizontalHeader()->setVisible(false);
    view->verticalHeader()->setVisible(false);
    view->setRowCount(7);
    view->setColumnCount(4);
    for (int i=1; i < 7; i++) {
        view->setSpan(i, 0, 1, 2);
        view->setSpan(i, 2, 1, 2);
    }
    for (int i=1; i < 5; i+=2) {
        view->setSpan(i, 2, 2, 2);
    }

    mLayout = new QVBoxLayout(this);
    mLayout->setContentsMargins(0, 3, 0, 3);
    mLayout->addWidget(view);
}

void AppTester::initStatus()
{
    status = new QTableWidget(this);
    status->setRowCount(2);
    status->setColumnCount(4);
    status->setFixedHeight(60);
    status->setColumnWidth(0, 36);
    status->setColumnWidth(1, 48);
    status->setColumnWidth(2, 36);
#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    status->horizontalHeader()->setResizeMode(3, QHeaderView::Stretch);
    status->verticalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    status->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    status->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif
    status->horizontalHeader()->setVisible(false);
    status->verticalHeader()->setVisible(false);

    status->horizontalHeader()->setStretchLastSection(true);
    status->setStyleSheet("border:none;font:11pt;");
    status->setEnabled(false);

    QStringList strs;
    strs << tr("用户:") << "____" << tr("型号:") << "0123456789ABC"
         << tr("温度:") << "____" << tr("编码:") << "____";

    for (int i=0; i < strs.size(); i++) {
        status->setItem(i/4, i%4, new QTableWidgetItem(strs.at(i)));
    }

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(1);
    layout->addWidget(status);
    layout->addStretch();

    vLayout = new QHBoxLayout;
    vLayout->setMargin(0);
    layout->addLayout(vLayout);

    QFrame *box = new QFrame(this);
    box->setLayout(layout);
    view->setCellWidget(0, 0, box);
}

void AppTester::initWorker()
{  // 工位显示区
    textWorker = new QLabel(this);
    vLayout->addWidget(textWorker);
    textWorker->setText(judgeON.arg("左"));
}

void AppTester::initResult()
{  // 总结果显示区
    textResult = new QLabel(this);
    vLayout->addWidget(textResult);
    textResult->setText(judgeOK.arg("OK"));
}

void AppTester::initQChart()
{
    pieChart = new BoxQChart(this);
    connect(pieChart, SIGNAL(sendClick()), this, SLOT(pieResize()));
    view->setCellWidget(0, 2, pieChart);
}

void AppTester::initButton()
{  // 按钮显示区
    QVBoxLayout *btnLayout = new QVBoxLayout;
    btnLayout->setMargin(3);

    QPushButton *btnHome = new QPushButton("开机主页", this);
    btnLayout->addWidget(btnHome);
    btnHome->setObjectName("author");
    btnHome->setMinimumSize(97, 44);
    connect(btnHome, SIGNAL(clicked(bool)), this, SLOT(clickButton()));

    QPushButton *btnConf = new QPushButton("型号管理", this);
    btnLayout->addWidget(btnConf);
    btnConf->setObjectName("config");
    btnConf->setMinimumSize(97, 44);
    connect(btnConf, SIGNAL(clicked(bool)), this, SLOT(clickButton()));

    btnPlay = new QPushButton("启动测试", this);
    btnLayout->addWidget(btnPlay);
    btnPlay->setMinimumSize(97, 44);
    connect(btnPlay, SIGNAL(clicked(bool)), this, SLOT(clickStart()));
#ifdef __arm__
    btnPlay->hide();
#endif

    btnLayout->addStretch();

    QFrame *frame = new QFrame(this);
    frame->setLayout(btnLayout);

    view->setCellWidget(0, 3, frame);
}

void AppTester::initDcrWeld()
{
    QString tt = titleNG + "%1&nbsp;&nbsp;%2%3%&nbsp;&nbsp;%4:%5~%6Ω</p>";
    tt = tt.arg("片间电阻").arg("离散:±");
    tt = tt.arg(QString::number(12, 'f', 3)).arg("均值");
    tt = tt.arg(QString::number(0.500, 'f', 3)).arg(QString::number(1.500, 'f', 3));

    QLabel *title = new QLabel(this);
    title->setText(tt);

    textDcrMD = new QTextBrowser(this);
    textDcrMD->setEnabled(false);
    textDcrMD->setStyleSheet("border:none");
    for (int i=0; i < NUMS; i++) {
        if (i !=0 && i%12 == 0)
            textDcrMD->insertHtml("<br></br>");
        if (i == 12)
            textDcrMD->insertHtml(SmallNG.arg(QString::number(1.400, 'f', 3)));
        else
            textDcrMD->insertHtml(SmallOK.arg(QString::number(1.400, 'f', 3)));
    }
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(title);
    layout->addWidget(textDcrMD);

    QFrame *frame = new QFrame(this);
    frame->setLayout(layout);
    view->setCellWidget(1, 2, frame);
}

void AppTester::initDcrChip()
{
    QString tt = titleOK + "%1&nbsp;&nbsp;&lt;%2mΩ</p>";
    tt = tt.arg("焊接电阻").arg(QString::number(0.100, 'f', 3));

    QLabel *title = new QLabel(this);
    title->setText(tt);

    textDcrCP = new QTextBrowser(this);
    textDcrCP->setEnabled(false);
    textDcrCP->setStyleSheet("border:none");
    for (int i=0; i < NUMS; i++) {
        if (i !=0 && i%12 == 0)
            textDcrCP->insertHtml("<br></br>");
        textDcrCP->insertHtml(SmallOK.arg(QString::number(0.0500, 'f', 3)));
    }
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(title);
    layout->addWidget(textDcrCP);
    layout->setStretch(1, 1);

    QFrame *frame = new QFrame(this);
    frame->setLayout(layout);
    view->setCellWidget(3, 2, frame);
}

void AppTester::initDcrDiag()
{
    QString tt = titleOK + "%1&nbsp;&nbsp;%2%3%&nbsp;&nbsp;%4:%5~%6Ω</p>";
    tt = tt.arg("跨间电阻").arg("离散:±");
    tt = tt.arg(QString::number(12, 'f', 3)).arg("均值");
    tt = tt.arg(QString::number(0.500, 'f', 3)).arg(QString::number(1.500, 'f', 3));

    QLabel *title = new QLabel(this);
    title->setText(tt);

    QStringList tmp;
    tmp << tr("挂钩顺序") << tr("顺时针") << "OK";
    QHBoxLayout *largeLayout = new QHBoxLayout;
    largeLayout->setMargin(0);
    for (int i=0; i < 3; i++) {
        QLabel *text = new QLabel(largeOK.arg(tmp.at(i)), this);
        //        inrTexts.append(text);
        largeLayout->addWidget(text);
    }

    QTextBrowser *text = new QTextBrowser(this);
    text->setStyleSheet("border:none;");

    for (int i=0; i < NUMS/2; i++) {
        if (i !=0 && i%12 == 0)
            text->insertHtml("<br></br>");
        text->insertHtml(SmallOK.arg(QString::number(0.0500, 'f', 3)));
    }

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(title);
    layout->addLayout(largeLayout);
    layout->addWidget(text);
    layout->setStretch(1, 1);

    QFrame *frame = new QFrame(this);
    frame->setLayout(layout);
    view->setCellWidget(5, 2, frame);
}

void AppTester::initDcrWave()
{
    dcrView = new QCustomPlot(this);
    connect(dcrView, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(dcrResize()));
    dcrView->setBackground(QBrush(QColor(25, 25, 25))); //设置背景色
    dcrView->xAxis->grid()->setPen(Qt::NoPen);
    dcrView->yAxis->grid()->setPen(QPen(Qt::darkGreen, 1, Qt::DotLine));
    dcrView->xAxis->setTicks(false);
    dcrView->xAxis->setTickLabels(false);
    dcrView->yAxis->setTicks(false);
    dcrView->yAxis->setTickLabels(false);
    dcrView->axisRect()->setMinimumMargins(QMargins(0, 0, 0, 0));
    dcrView->axisRect()->setMargins(QMargins(0, 0, 0, 0));
    dcrView->xAxis->setBasePen(QPen(Qt::darkGreen, 1, Qt::DotLine));
    dcrView->yAxis->setBasePen(Qt::NoPen);
    dcrView->xAxis2->setBasePen(Qt::NoPen);
    dcrView->yAxis2->setBasePen(Qt::NoPen);
    dcrView->xAxis->setRange(0, 24);
    dcrView->yAxis->setRange(0, 103);

    dcrView->xAxis->setTickLabelColor(Qt::white);
    dcrView->xAxis->setLabelColor(Qt::white);
    dcrView->xAxis->setTickLabelColor(Qt::white);

    view->setCellWidget(6, 2, dcrView);
}

void AppTester::initInrTextCG()
{
    QString tt = titleOK + "%1&nbsp;&nbsp;DC:%2kV&nbsp;&nbsp;IR:%3MΩ</p>";
    tt = tt.arg("绝缘电阻");
    tt = tt.arg(QString::number(0.5, 'f', 3)).arg(QString::number(500, 'f', 0));

    QLabel *title = new QLabel(this);
    title->setText(tt);
    acwTitles.append(title);

    QStringList tmp;
    tmp << "1.500kV" << ">500MΩ" << "OK";
    QHBoxLayout *datLayout = new QHBoxLayout;
    datLayout->setMargin(0);
    for (int i=0; i < 3; i++) {
        QLabel *text = new QLabel(largeOK.arg(tmp.at(i)), this);
        acwLabels.append(text);
        datLayout->addWidget(text);
    }

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(title);
    layout->addLayout(datLayout);
    layout->setStretch(1, 1);

    QFrame *frame = new QFrame(this);
    frame->setLayout(layout);
    view->setCellWidget(1, 0, frame);
}

void AppTester::initAcwTextAC()
{
    QString tt = titleOK + "%1&nbsp;&nbsp;AC:%2kV&nbsp;&nbsp;I:%3~%4mA</p>";
    tt = tt.arg("轴铁耐压").arg(QString::number(1.5, 'f', 3));
    tt = tt.arg(QString::number(0.01, 'f', 2)).arg(QString::number(0.5, 'f', 2));

    QLabel *title = new QLabel(this);
    title->setText(tt);
    acwTitles.append(title);

    QStringList tmp;
    tmp << "1.500kV" << "0.300mA" << "OK";
    QHBoxLayout *datLayout = new QHBoxLayout;
    datLayout->setMargin(0);
    for (int i=0; i < 3; i++) {
        QLabel *text = new QLabel(largeOK.arg(tmp.at(i)), this);
        acwLabels.append(text);
        datLayout->addWidget(text);
    }

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(title);
    layout->addLayout(datLayout);
    layout->setStretch(1, 1);

    QFrame *frame = new QFrame(this);
    frame->setLayout(layout);
    view->setCellWidget(2, 0, frame);
}

void AppTester::initAcwTextAL()
{
    QString tt = titleOK + "%1&nbsp;&nbsp;AC:%2kV&nbsp;&nbsp;I:%3~%4mA</p>";
    tt = tt.arg("轴线耐压").arg(QString::number(2.5, 'f', 3));
    tt = tt.arg(QString::number(0.01, 'f', 2)).arg(QString::number(0.5, 'f', 2));

    QLabel *title = new QLabel(this);
    title->setText(tt);
    acwTitles.append(title);

    QStringList tmp;
    tmp << "2.500kV" << "0.400mA" << "OK";
    QHBoxLayout *datLayout = new QHBoxLayout;
    datLayout->setMargin(0);
    for (int i=0; i < 3; i++) {
        QLabel *text = new QLabel(largeOK.arg(tmp.at(i)), this);
        acwLabels.append(text);
        datLayout->addWidget(text);
    }

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(title);
    layout->addLayout(datLayout);
    layout->setStretch(1, 1);

    QFrame *frame = new QFrame(this);
    frame->setLayout(layout);
    view->setCellWidget(3, 0, frame);
}

void AppTester::initAcwTextLC()
{
    QString tt = titleOK + "%1&nbsp;&nbsp;AC:%2kV&nbsp;&nbsp;I:%3~%4mA</p>";
    tt = tt.arg("线铁耐压").arg(QString::number(4.5, 'f', 3));
    tt = tt.arg(QString::number(0.01, 'f', 2)).arg(QString::number(0.5, 'f', 2));

    QLabel *title = new QLabel(this);
    title->setText(tt);
    acwTitles.append(title);

    QStringList tmp;
    tmp << "4.500kV" << "0.400mA" << "OK";
    QHBoxLayout *datLayout = new QHBoxLayout;
    datLayout->setMargin(0);
    for (int i=0; i < 3; i++) {
        QLabel *text = new QLabel(largeOK.arg(tmp.at(i)), this);
        acwLabels.append(text);
        datLayout->addWidget(text);
    }

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(title);
    layout->addLayout(datLayout);
    layout->setStretch(1, 1);

    QFrame *frame = new QFrame(this);
    frame->setLayout(layout);
    view->setCellWidget(4, 0, frame);
}

void AppTester::initImpText()
{
    QString tt = titleOK + "%1&nbsp;&nbsp;AC:%2kV&nbsp;&nbsp;&lt;%3%</p>";
    tt = tt.arg("匝间测试").arg(QString::number(1.0, 'f', 3));
    tt = tt.arg(QString::number(12, 'f', 2));

    QLabel *title = new QLabel(this);
    title->setText(tt);

    QStringList tmp;
    tmp << tr("1.005kV") << tr("2.5030%") << "OK";
    QHBoxLayout *largeLayout = new QHBoxLayout;
    largeLayout->setMargin(0);
    for (int i=0; i < 3; i++) {
        QLabel *text = new QLabel(largeOK.arg(tmp.at(i)), this);
        //        inrTexts.append(text);
        largeLayout->addWidget(text);
    }

    QTextBrowser *text = new QTextBrowser(this);
    text->setStyleSheet("border:none");
    for (int i=0; i < NUMS/2; i++) {
        if (i !=0 && i%12 == 0)
            text->insertHtml("<br></br>");
        text->insertHtml(SmallOK.arg(QString::number(0.0500, 'f', 3)));
    }

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(title);
    layout->addLayout(largeLayout);
    layout->addWidget(text);
    layout->setStretch(1, 1);

    QFrame *frame = new QFrame(this);
    frame->setLayout(layout);
    view->setCellWidget(5, 0, frame);
}

void AppTester::initImpWave()
{
    impView = new QCustomPlot(this);
    connect(impView, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(impResize()));
    impView->setBackground(QBrush(QColor(25, 25, 25))); //设置背景色
    impView->xAxis->grid()->setPen(Qt::NoPen);
    impView->yAxis->grid()->setPen(QPen(Qt::darkGreen, 1, Qt::DotLine));
    impView->xAxis->setTicks(false);
    impView->xAxis->setTickLabels(false);
    impView->yAxis->setTicks(false);
    impView->yAxis->setTickLabels(false);
    impView->axisRect()->setMinimumMargins(QMargins(0, 0, 0, 0));
    impView->axisRect()->setMargins(QMargins(0, 0, 0, 0));
    impView->xAxis->setBasePen(QPen(Qt::darkGreen, 1, Qt::DotLine));
    impView->yAxis->setBasePen(Qt::NoPen);
    impView->xAxis2->setBasePen(Qt::NoPen);
    impView->yAxis2->setBasePen(Qt::NoPen);
    impView->xAxis->setRange(0, 100);
    impView->yAxis->setRange(0, 103);

    impView->xAxis->setTickLabelColor(Qt::white);
    impView->xAxis->setLabelColor(Qt::white);
    impView->xAxis->setTickLabelColor(Qt::white);

    view->setCellWidget(6, 0, impView);
}

void AppTester::drawImpWave()
{
    QCPGraph *graph = impView->addGraph();
    graph->setPen(QPen(Qt::green, 2));
    QVector<double> x(100), y(100);

    for (int i=0; i < 100; i++) {
        x[i] = i;
        y[i] = 50+50*sin(PI*i/24)*(100-i)/100;
    }
    graph->setData(x, y);
    impView->replot();
}

void AppTester::setViewSize()
{
    for (int i=1; i < 5; i++) {
        view->setRowHeight(i, 52);
    }
    if (NUMS > 36) {
        for (int i=1; i < 5; i++) {
            view->setRowHeight(i, 58);
        }
    }
    view->setRowHeight(5, 96);
    view->setRowHeight(0, 160);
    view->setColumnWidth(0, this->width()*270/800);
    view->setColumnWidth(1, this->width()*130/800);
    view->setColumnWidth(2, this->width()*270/800);
    view->horizontalHeader()->setStretchLastSection(true);
    view->verticalHeader()->setStretchLastSection(true);
}

void AppTester::drawDcrWave()
{
    QCPGraph *graph1 = dcrView->addGraph();
    graph1->setPen(QPen(Qt::green, 2));
    QCPGraph *graph2 = dcrView->addGraph();
    graph2->setPen(QPen(Qt::white, 1));
    QCPGraph *graph3 = dcrView->addGraph();
    graph3->setPen(QPen(Qt::white, 1));
    QVector<double> x1(24), y1(24);
    QVector<double> x2(24), y2(24);
    QVector<double> x3(24), y3(24);

    for (int i=0; i < 24; i++) {
        x1[i] = i;
        if (i % 2 == 0) {
            y1[i] = 50-i;
            y2[i] = 64-i;
            y3[i] = 36-i;
        } else {
            y1[i] = 75-i;
            y2[i] = 89-i;
            y3[i] = 61-i;
        }
    }
    graph1->setData(x1, y1);
    graph2->setData(x1, y2);
    graph3->setData(x1, y3);
    dcrView->replot();
}

void AppTester::initHistogram()
{
    histogram = new QCustomPlot(this);
    QStringList colors;
    colors << "blue" << "green" << "red";
    for (int i=0; i < 3; i++) {
        bars.append(new QCPBars(histogram->xAxis, histogram->yAxis));
        bars.at(i)->setWidth(0.9);
        bars.at(i)->setPen(Qt::NoPen);
        bars.at(i)->setBrush(QColor(colors.at(i)));
    }

    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, QColor(80, 80, 80));
    plotGradient.setColorAt(1, QColor(50, 50, 50));
    histogram->setBackground(plotGradient);

    QVector<double> ticks;
    QVector<QString> labels;
    ticks << 1 << 2 << 3;
    labels << "ALL\n0" << "OK\n0" << "NG\n0";
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);
    histogram->xAxis->setTicker(textTicker);
    histogram->xAxis->setTickLength(0, 1);  // x轴标尺步长

    histogram->xAxis->setBasePen(Qt::NoPen);
    histogram->yAxis->setBasePen(Qt::NoPen);
    histogram->xAxis2->setBasePen(Qt::NoPen);
    histogram->yAxis2->setBasePen(Qt::NoPen);
    histogram->xAxis->grid()->setVisible(false);
    histogram->yAxis->grid()->setVisible(false);
    histogram->xAxis->setTickLabelColor(Qt::white);
    histogram->yAxis->setTicks(false);
    histogram->xAxis->setLabelColor(Qt::white);
    histogram->setBackground(QBrush(QColor(25, 25, 25))); //设置背景色
    histogram->axisRect()->setMinimumMargins(QMargins(3, 9, 3, 0));  // 左上右下
    histogram->rescaleAxes();
    histogram->xAxis->setRange(0.5, 3.5);  // x轴范围
    histogram->yAxis->setRange(0, 101);    // y轴范围
    histogram->xAxis->setPadding(1);
    histogram->yAxis->setPadding(1);
    histogram->xAxis->setTickLabelPadding(1);

    view->setCellWidget(0, 1, histogram);
}

void AppTester::drawHistogram()
{
    countAll = 100;
    countOk = 98;
    QVector<double> x1(1), y1(1);
    QVector<double> x2(1), y2(1);
    QVector<double> x3(1), y3(1);

    x1[0] = 1;
    x2[0] = 2;
    x3[0] = 3;
    y1[0] = 100;
    y2[0] = (100*countOk)/countAll;
    y3[0] = (100*(countAll - countOk))/countAll;

    bars.at(0)->setData(x1, y1);
    bars.at(1)->setData(x2, y2);
    bars.at(2)->setData(x3, y3);

    QVector<double> ticks;
    QVector<QString> labels;
    ticks << 1 << 2 << 3;
    labels << (QString("ALL\n%1").arg(countAll))
           << (QString("OK\n%1").arg(countOk))
           << (QString("NG\n%1").arg(countAll - countOk));
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);
    histogram->xAxis->setTicker(textTicker);

    histogram->replot();
}

void AppTester::pieResize()
{
    if (view->isRowHidden(1)) {
        for (int i=0; i < 7; i++) {
            view->showRow(i);
        }
        for (int i=0; i < 4; i++) {
            view->showColumn(i);
        }
        setViewSize();
    } else {
        for (int i=0; i < 7; i++) {
            if (i == 0)
                continue;
            view->hideRow(i);
        }
        for (int i=0; i < 4; i++) {
            if (i == 2) {
                continue;
            }
            view->hideColumn(i);
        }
    }
}

void AppTester::dcrResize()
{
    if (view->isRowHidden(1)) {
        for (int i=0; i < 7; i++) {
            view->showRow(i);
        }
        for (int i=0; i < 4; i++) {
            view->showColumn(i);
        }
        setViewSize();
    } else {
        for (int i=0; i < 7; i++) {
            if (i == 6)
                continue;
            view->hideRow(i);
        }
        for (int i=0; i < 4; i++) {
            if (i == 2) {
                continue;
            }
            view->hideColumn(i);
        }
    }
}

void AppTester::impResize()
{
    if (view->isRowHidden(1)) {
        for (int i=0; i < 7; i++) {
            view->showRow(i);
        }
        for (int i=0; i < 4; i++) {
            view->showColumn(i);
        }
        setViewSize();
    } else {
        for (int i=0; i < 7; i++) {
            if (i == 6)
                continue;
            view->hideRow(i);
        }
        for (int i=0; i < 4; i++) {
            if (i == 0) {
                continue;
            }
            view->hideColumn(i);
        }
    }
}

void AppTester::clickStart()
{
    if (btnPlay->text() == tr("启动测试")) {
        btnPlay->setText(tr("停止测试"));
        tmpMap.insert("enum", Qt::Key_Play);

    } else {
        btnPlay->setText(tr("启动测试"));
        tmpMap.insert("enum", Qt::Key_Stop);
    }
    emit sendAppMap(tmpMap);
    tmpMap.clear();
}

void AppTester::clickButton()
{
    tmpMap.insert("enum", Qt::Key_Display);
    tmpMap.insert("text", sender()->objectName());
    emit sendAppMap(tmpMap);
    tmpMap.clear();
}

void AppTester::clearView()
{
    QStringList tmp;
    tmp << tr("绝缘电阻") << tr("轴铁耐压") << tr("轴线耐压") << tr("线铁耐压");
    int addr;
    QString tt;
    for (int i=0; i < tmp.size(); i++) {
        addr = AddrAG + i*0x10;
        double v = config[QString::number(addr + AddrHV)].toDouble()/1000;
        double h = config[QString::number(addr + AddrHH)].toDouble();
        double l = config[QString::number(addr + AddrHL)].toDouble();
        if (i == 0) {
            tt = titleOK + "%1&nbsp;&nbsp;DC:%2kV&nbsp;&nbsp;IR:%3MΩ</p>";
            tt = tt.arg(tmp.at(i)).arg(QString::number(v, 'f', 3));
            tt = tt.arg(l);
        } else {
            tt = titleOK + "%1&nbsp;&nbsp;AC:%2kV&nbsp;&nbsp;I:%3~%4mA</p>";
            tt = tt.arg(tmp.at(i)).arg(QString::number(v, 'f', 3));
            h /= 100;
            l /= 100;
            tt = tt.arg(QString::number(l, 'f', 2)).arg(QString::number(h, 'f', 2));
        }
        acwTitles.at(i)->setText(tt);
        acwLabels.at(i*3 + 0)->setText(largeOK.arg("-.---kV"));
        if (i == 0) {
            acwLabels.at(3*i + 1)->setText(largeOK.arg("---.-MΩ"));
        } else {
            acwLabels.at(3*i + 1)->setText(largeOK.arg("-.---mA"));
        }
        acwLabels.at(3*i + 2)->setText(largeOK.arg("--"));
    }
}

void AppTester::recvUpdate(QVariantMap msg)
{
    int addr = msg.value("text").toInt();
    int t = (addr%AddrAG)/0x10;
    double j = msg[QString::number(addr + AddrHJ)].toInt();
    QString color = (j == 0) ? largeNG : largeOK;
    QString judge = (j == 0) ? "NG" : "OK";
    double v = msg[QString::number(addr + AddrHU)].toInt();
    double r = msg[QString::number(addr + AddrHR)].toInt();
    double d = msg[QString::number(addr + AddrHD)].toInt();
    r *= qPow(10, -d);
    QString volt = QString::number(v/1000, 'f', 3) + "kV";
    QString real = QString::number(r, 'f', 3) + "mA";
    if (t == 0) {
        real = (r > 500) ? ">500MΩ" : (QString::number(r, 'f', 1) + "MΩ");
    }
    acwLabels.at(3*t + 0)->setText(color.arg(volt));
    acwLabels.at(3*t + 1)->setText(color.arg(real));
    acwLabels.at(3*t + 2)->setText(color.arg(judge));

    QStringList keys = msg.keys();
    keys.removeOne("enum");
    keys.removeOne("text");
    for (int i=0; i < keys.size(); i++) {
        config[keys.at(i)] = msg[keys.at(i)];
    }
}

void AppTester::recvAppMap(QVariantMap msg)
{
    switch (msg.value("enum").toInt()) {
    case Qt::Key_Option:
        config = msg;
        pieChart->setNum(msg[QString::number(AddrSC + AddrTC)].toInt());
        break;
    case Qt::Key_Refresh:
        recvUpdate(msg);
        break;
    case Qt::Key_WakeUp:
        clearView();
        break;
    case Qt::Key_Stop:
        break;
    case Qt::Key_Sleep:
        btnPlay->setText(tr("启动测试"));
        break;
    default:
        break;
    }
}

void AppTester::showEvent(QShowEvent *e)
{
    this->setFocus();
    setViewSize();
    clearView();
    e->accept();
}
