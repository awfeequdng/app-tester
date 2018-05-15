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
    status->setRowCount(3);
    status->setColumnCount(4);

#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    status->setColumnWidth(0, 36);
    status->setColumnWidth(1, 56);
    status->setColumnWidth(2, 36);
    status->horizontalHeader()->setResizeMode(3, QHeaderView::Stretch);
    status->verticalHeader()->setResizeMode(2, QHeaderView::Stretch);
#else
    status->setColumnWidth(0, 42);
    status->setColumnWidth(1, 56);
    status->setColumnWidth(2, 42);
    status->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    status->verticalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
#endif
    status->setSpan(2, 0, 1, 3);
    status->horizontalHeader()->setVisible(false);
    status->verticalHeader()->setVisible(false);

    status->horizontalHeader()->setStretchLastSection(true);
    status->setStyleSheet("border:none;font:11pt;background:#121922");
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

    QFrame *box = new QFrame(this);
    box->setLayout(layout);
    view->setCellWidget(0, 0, box);
}

void AppTester::initWorker()
{  // 工位显示区
    textWorker = new QLabel(this);
    status->setCellWidget(2, 0, textWorker);
    textWorker->setText(judgeON.arg("左"));
}

void AppTester::initResult()
{  // 总结果显示区
    textResult = new QLabel(this);
    status->setCellWidget(2, 3, textResult);
    textResult->setText(judgeOK);
}

void AppTester::initQChart()
{
    boxChart = new BoxQChart(this);
    connect(boxChart, SIGNAL(sendClick()), this, SLOT(boxResize()));
    view->setCellWidget(0, 2, boxChart);
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
    btnPlay->setMinimumSize(97, 44);
    connect(btnPlay, SIGNAL(clicked(bool)), this, SLOT(clickStart()));

    QLabel *btnLogo = new QLabel(this);
    btnLogo->setPixmap(QPixmap(":/icon_aip.png"));
    btnLogo->setScaledContents(true);

#ifdef __arm__
    btnPlay->hide();
    btnLayout->addWidget(btnLogo);
#else
    btnLayout->addWidget(btnPlay);
    btnLogo->hide();
#endif

    btnLayout->addStretch();

    QFrame *frame = new QFrame(this);
    frame->setLayout(btnLayout);

    view->setCellWidget(0, 3, frame);
}

void AppTester::initDcrWeld()
{
    QLabel *title = new QLabel(this);
    dcrTitles.append(title);

    textWeld = new QTextBrowser(this);
    textWeld->setEnabled(false);
    textWeld->setStyleSheet("border:none");
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(title);
    layout->addWidget(textWeld);

    QFrame *frame = new QFrame(this);
    frame->setLayout(layout);
    view->setCellWidget(1, 2, frame);
}

void AppTester::initDcrChip()
{
    QLabel *title = new QLabel(this);
    dcrTitles.append(title);

    textChip = new QTextBrowser(this);
    textChip->setEnabled(false);
    textChip->setStyleSheet("border:none");
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(title);
    layout->addWidget(textChip);
    layout->setStretch(1, 1);

    QFrame *frame = new QFrame(this);
    frame->setLayout(layout);
    view->setCellWidget(3, 2, frame);
}

void AppTester::initDcrDiag()
{
    QLabel *title = new QLabel(this);
    dcrTitles.append(title);

    QHBoxLayout *largeLayout = new QHBoxLayout;
    largeLayout->setMargin(0);
    for (int i=0; i < 3; i++) {
        QLabel *text = new QLabel(this);
        dcrLabels.append(text);
        largeLayout->addWidget(text);
    }

    textDiag = new QTextBrowser(this);
    textDiag->setStyleSheet("border:none;");

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(title);
    layout->addLayout(largeLayout);
    layout->addWidget(textDiag);
    layout->setStretch(1, 1);

    QFrame *frame = new QFrame(this);
    frame->setLayout(layout);
    view->setCellWidget(5, 2, frame);
}

void AppTester::initDcrWave()
{
    dcrView = new QCustomPlot(this);
    connect(dcrView, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(dcrResize()));
    dcrView->setBackground(QBrush(QColor("#121922"))); //设置背景色
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
    dcrView->yAxis->setRange(0, 105);

    dcrView->xAxis->setTickLabelColor(Qt::white);
    dcrView->xAxis->setLabelColor(Qt::white);
    dcrView->xAxis->setTickLabelColor(Qt::white);

    view->setCellWidget(6, 2, dcrView);
}

void AppTester::initInrTextCG()
{
    QFrame *frame = new QFrame(this);
    view->setCellWidget(1, 0, frame);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    frame->setLayout(layout);

    QLabel *title = new QLabel(this);
    layout->addWidget(title);
    acwTitles.append(title);

    QHBoxLayout *large = new QHBoxLayout;
    layout->addLayout(large);
    layout->setStretch(1, 1);
    large->setMargin(0);
    for (int i=0; i < 3; i++) {
        QLabel *text = new QLabel(this);
        acwLabels.append(text);
        large->addWidget(text);
    }
}

void AppTester::initAcwTextAC()
{
    QFrame *frame = new QFrame(this);
    view->setCellWidget(2, 0, frame);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    frame->setLayout(layout);

    QLabel *title = new QLabel(this);
    layout->addWidget(title);
    acwTitles.append(title);

    QHBoxLayout *large = new QHBoxLayout;
    layout->addLayout(large);
    layout->setStretch(1, 1);
    large->setMargin(0);
    for (int i=0; i < 3; i++) {
        QLabel *text = new QLabel(this);
        acwLabels.append(text);
        large->addWidget(text);
    }
}

void AppTester::initAcwTextAL()
{
    QFrame *frame = new QFrame(this);
    view->setCellWidget(3, 0, frame);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    frame->setLayout(layout);

    QLabel *title = new QLabel(this);
    layout->addWidget(title);
    acwTitles.append(title);

    QHBoxLayout *large = new QHBoxLayout;
    layout->addLayout(large);
    layout->setStretch(1, 1);
    large->setMargin(0);
    for (int i=0; i < 3; i++) {
        QLabel *text = new QLabel(this);
        acwLabels.append(text);
        large->addWidget(text);
    }
}

void AppTester::initAcwTextLC()
{
    QFrame *frame = new QFrame(this);
    view->setCellWidget(4, 0, frame);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    frame->setLayout(layout);

    QLabel *title = new QLabel(this);
    layout->addWidget(title);
    acwTitles.append(title);

    QHBoxLayout *large = new QHBoxLayout;
    layout->addLayout(large);
    layout->setStretch(1, 1);
    large->setMargin(0);
    for (int i=0; i < 3; i++) {
        QLabel *text = new QLabel(this);
        acwLabels.append(text);
        large->addWidget(text);
    }
}

void AppTester::initImpText()
{
    QString tt = titleOK + "%1&nbsp;&nbsp;AC:%2kV&nbsp;&nbsp;&lt;%3%</p>";
    tt = tt.arg("匝间测试").arg(QString::number(1.0, 'f', 3));
    tt = tt.arg(QString::number(12, 'f', 2));

    QLabel *title = new QLabel(this);
    impTitles.append(title);
    title->setText(tt);

    QStringList tmp;
    tmp << tr("1.005kV") << tr("2.5030%") << "OK";
    QHBoxLayout *largeLayout = new QHBoxLayout;
    largeLayout->setMargin(0);
    for (int i=0; i < 3; i++) {
        QLabel *text = new QLabel(largeOK.arg(tmp.at(i)), this);
        impLabels.append(text);
        largeLayout->addWidget(text);
    }

    textIMPR = new QTextBrowser(this);
    textIMPR->setStyleSheet("border:none");

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(title);
    layout->addLayout(largeLayout);
    layout->addWidget(textIMPR);
    layout->setStretch(1, 1);

    QFrame *frame = new QFrame(this);
    frame->setLayout(layout);
    view->setCellWidget(5, 0, frame);
}

void AppTester::initImpWave()
{
    impView = new QCustomPlot(this);
    connect(impView, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(impResize()));
    impView->setBackground(QBrush(QColor("#121922"))); //设置背景色
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
    impView->xAxis->setRange(0, 400);
    impView->yAxis->setRange(0, 105);

    impView->xAxis->setTickLabelColor(Qt::white);
    impView->xAxis->setLabelColor(Qt::white);
    impView->xAxis->setTickLabelColor(Qt::white);

    impLine = impView->addGraph();

    view->setCellWidget(6, 0, impView);
}

void AppTester::initSettings()
{
    int r = 0;
    boxChart->setNum(tmpSet[tmpSet[AddrModel].toInt()].toInt());
    if (1) {
        if (tmpSet[DataSign].toInt() == 0) {
            status->item(0, 1)->setText(tr("未登录"));
        } else {
            r = tmpSet[DataUser].toInt();
            status->item(0, 1)->setText(tmpSet[r].toString());
        }
    }
    if (1) {
        r = tmpSet[DataFile].toInt();
        status->item(0, 3)->setText(tmpSet[r].toString());
    }

    QString tt;
    for (int i=0; i < 4; i++) {
        r = tmpSet[AddrACWS1 + i].toInt();  // 高压配置地址
        tt = tmpSet[r+0].toDouble() == 1 ? titleOK : titleNG;
        double c = tmpSet[r + AddrACWSC].toDouble();
        double v = tmpSet[r + AddrACWSV].toDouble()/1000;
        double h = tmpSet[r + AddrACWSH].toDouble();
        double l = tmpSet[r + AddrACWSL].toDouble();
        QString n = tmpSet[r + AddrACWSN].toString();
        h = (i == 0) ? h : h/100;
        l = (i == 0) ? l : l/100;
        if (i == 0) {
            tt += "&nbsp;&nbsp;%1&nbsp;&nbsp;DC:%2kV&nbsp;&nbsp;IR:%3MΩ</p>";
            tt = tt.arg(n).arg(QString::number(v, 'f', 3));
            tt = tt.arg(l);
        } else {
            tt += "&nbsp;&nbsp;%1&nbsp;&nbsp;AC:%2kV&nbsp;&nbsp;I:%3~%4mA</p>";
            tt = tt.arg(n).arg(QString::number(v, 'f', 3));
            tt = tt.arg(QString::number(l, 'f', 2)).arg(QString::number(h, 'f', 2));
        }
        acwTitles.at(i)->setText(tt);
        QString color = (c == 1) ? largeOK : largeON;
        acwLabels.at(i*3 + 0)->setText(color.arg("-.---kV"));
        if (i == 0) {
            acwLabels.at(3*i + 1)->setText(color.arg("---.-MΩ"));
        } else {
            acwLabels.at(3*i + 1)->setText(color.arg("-.---mA"));
        }
        acwLabels.at(3*i + 2)->setText(color.arg("--"));
    }
    if (1) {
        int r = tmpSet[AddrIMPS1].toInt();  // 匝间配置地址
        tt = tmpSet[r+0].toDouble() == 1 ? titleOK : titleNG;
        double c = tmpSet[r + AddrACWSC].toDouble();
        double v = tmpSet[r + AddrACWSV].toDouble()/1000;
        double h = tmpSet[r + AddrACWSH].toDouble();
        QString n = tmpSet[r + AddrACWSN].toString();

        tt += "&nbsp;&nbsp;%1&nbsp;&nbsp;AC:%2kV&nbsp;&nbsp;&lt;%3%</p>";
        tt = tt.arg(n).arg(QString::number(v, 'f', 3));
        tt = tt.arg(QString::number(h, 'f', 2));
        impTitles.at(0)->setText(tt);
        QString color = (c == 1) ? largeOK : largeON;
        impLabels.at(0)->setText(color.arg("-.---kV"));
        impLabels.at(1)->setText(color.arg("-.---%"));
        impLabels.at(2)->setText(color.arg("--"));

        tmpWave.clear();
        textIMPR->clear();
    }
    if (1) {
        int r = tmpSet[AddrDCRS1].toInt();  // 配置地址
        QString tt = tmpSet[r+0].toDouble() == 1 ? titleOK : titleNG;
        tt += "&nbsp;&nbsp;%1&nbsp;&nbsp;%2%3%</p>";
        tt = tt.arg("片间电阻").arg("离散:±");
        tt = tt.arg(QString::number(tmpSet[r+1].toDouble()/1000, 'f', 3));
        dcrTitles.at(0)->setText(tt);
        textWeld->clear();
    }
    if (2) {
        int r = tmpSet[AddrDCRS2].toInt();  // 配置地址
        QString tt = tmpSet[r+0].toDouble() == 1 ? titleOK : titleNG;
        tt += "&nbsp;&nbsp;%1&nbsp;&nbsp;&lt; %2mΩ</p>";
        tt = tt.arg("焊接电阻").arg(QString::number(tmpSet[r+1].toDouble()/1000, 'f', 3));
        dcrTitles.at(1)->setText(tt);
        textChip->clear();
    }
    if (3) {
        int r = tmpSet[AddrDCRS3].toInt();  // 配置地址
        int c = tmpSet[r+0].toInt();
        double l = tmpSet[r + 1].toDouble()/1000;
        double h = tmpSet[r + 2].toDouble()/1000;
        QString tt = c == 1 ? titleOK : titleNG;
        tt += "&nbsp;&nbsp;%1&nbsp;&nbsp;%2:%3~%4Ω</p>";
        tt = tt.arg("跨间电阻").arg("均值");
        tt = tt.arg(QString::number(l, 'f', 3)).arg(QString::number(h, 'f', 3));
        dcrTitles.at(2)->setText(tt);

        QStringList tmp;
        tmp << tr("挂钩顺序") << tr("顺时针") << "OK";
        if (c == 1) {
            dcrLabels.at(0)->setText(largeOK.arg("挂钩顺序"));
            dcrLabels.at(1)->setText(largeOK.arg("------"));
            dcrLabels.at(2)->setText(largeOK.arg("--"));
        }
        textDiag->clear();
    }
}

void AppTester::drawImpWave()
{
    impLine->setPen(QPen(Qt::green, 2));
    if (tmpWave.isEmpty()) {
        QVector<double> x(1), y(1);
        x[0] = -1;
        y[0] = -1;
        impLine->setData(x, y);
    } else {
        QVector<double> x(400), y(400);
        for (int i=0; i < 400; i++) {
            x[i] = i;
            y[i] = tmpWave.at(i)*100/0x0400;
        }
        impLine->setData(x, y);
    }
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
    view->setColumnWidth(0, this->width()*280/800);
    view->setColumnWidth(1, this->width()*120/800);
    view->setColumnWidth(2, this->width()*270/800);
    view->horizontalHeader()->setStretchLastSection(true);
    view->verticalHeader()->setStretchLastSection(true);
}

void AppTester::drawDcrWave()
{
    //    dcrView->clearGraphs();
    //    QCPGraph *graph1 = dcrView->addGraph();
    //    graph1->setPen(QPen(Qt::green, 2));
    //    QCPGraph *graph2 = dcrView->addGraph();
    //    graph2->setPen(QPen(Qt::white, 1));
    //    QCPGraph *graph3 = dcrView->addGraph();
    //    graph3->setPen(QPen(Qt::white, 1));
    //    int c = config[QString::number(AddrModel + AddrDCRSC)].toInt();
    //    QVector<double> x1(c), y1(c), y2(c), y3(c);

    //    int t = qMin(4, qMax(1, c/12));

    //    for (int i=0; i < c; i++) {
    //        x1[i] = i;
    //        if (i % 2 == 0) {
    //            y1[i] = 50-i/t;
    //            y2[i] = 54-i/t;
    //            y3[i] = 46-i/t;
    //        } else {
    //            y1[i] = 55-i/t;
    //            y2[i] = 59-i/t;
    //            y3[i] = 51-i/t;
    //        }
    //    }
    //    graph1->setData(x1, y1);
    //    graph2->setData(x1, y2);
    //    graph3->setData(x1, y3);
    //    dcrView->xAxis->setRange(0, c-1);
    //    dcrView->replot();
}

void AppTester::initHistogram()
{
    countAll = 0;
    countOk = 0;
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
    histogram->setBackground(QBrush(QColor("#121922"))); //设置背景色
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
    if (countAll == 0)
        return;
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

void AppTester::boxResize()
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
        tmpMsg.insert(AddrEnum, Qt::Key_Play);
    } else {
        btnPlay->setText(tr("启动测试"));
        tmpMsg.insert(AddrEnum, Qt::Key_Stop);
    }
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
}

void AppTester::clickButton()
{
    tmpMsg.insert(AddrEnum, Qt::Key_View);
    tmpMsg.insert(AddrText, sender()->objectName());
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
}

void AppTester::recvTmpMsg(QTmpMap msg)
{
    if (!msg.value(DataTemp).isNull()) {
        double temp = msg.value(DataTemp).toInt();
        if (temp > 0x8000) {
            temp = 0x8000 - temp;
        }
        status->item(1, 1)->setText(QString::number(temp/10, 'f', 1));
    }
}

void AppTester::recvLedMsg(QTmpMap msg)
{
    QString tmp = msg.value(AddrText).toString();
    if (tmp == "LEDY") {
        initSettings();
        btnPlay->setText(tr("停止测试"));
        textResult->setText(judgeON.arg("ON"));
        boxChart->setRun(1);
#ifdef __arm__
        view->setEnabled(false);
#endif
    } else {
        boxChart->setRun(0);
        view->setEnabled(true);
        btnPlay->setText(tr("启动测试"));
    }
    if (tmp == "LEDG") {
        if (msg.value(AddrBeep).toInt() > 0) {
            countAll++;
            countOk++;
            textResult->setText(judgeOK);
            drawHistogram();
        }
    }
    if (tmp == "LEDR") {
        if (msg.value(AddrBeep).toInt() > 0) {
            countAll++;
            textResult->setText(judgeNG);
            drawHistogram();
        }
    }
}

void AppTester::recvUpdate(QTmpMap msg)
{
    if (this->isHidden())
        return;
    int c = tmpSet[tmpSet[AddrModel].toInt()].toInt();
    int addr = msg.value(AddrText).toInt();
    if (addr >= AddrACWS1 && addr <= AddrACWS4) {
        int t = addr % AddrACWS1;
        int s = tmpSet[AddrINRA + t].toInt();
        double j = msg[s + AddrACWJ].toInt();
        double v = msg[s + AddrACWU].toInt();
        double r = msg[s + AddrACWR].toInt();
        double p = msg[s + AddrACWP].toInt();
        QString color = (j == DataNG) ? largeNG : largeOK;
        QString judge = (j == DataNG) ? "NG" : "OK";
        r *= qPow(10, -p);
        QString volt = QString::number(v/1000, 'f', 3) + "kV";
        QString real = QString::number(r, 'f', 3) + "mA";
        if (t == 0) {
            real = (r > 500) ? ">500MΩ" : (QString::number(r, 'f', 1) + "MΩ");
        }
        acwLabels.at(3*t + 0)->setText(color.arg(volt));
        acwLabels.at(3*t + 1)->setText(color.arg(real));
        acwLabels.at(3*t + 2)->setText(color.arg(judge));
    }
    if (addr == AddrIMPS1) {
        QString real;
        if (msg[AddrData].toInt() == 0) {  // 显示结果
            int r = tmpSet.value(AddrIMPA).toInt();  // 匝间结果地址
            for (int i=0; i < c; i++) {
                if (i%12 == 0) {
                    if (i != 0)
                        textIMPR->insertHtml("<br></br>");
                    textIMPR->insertHtml("&nbsp;&nbsp;");
                }
                double tmp = msg.value(r+i).toDouble()/1000;
                textIMPR->insertHtml(SmallOK.arg(QString::number(tmp, 'f', tmp >= 10 ? 2 : 3)));
            }
        } else {
            tmpWave.clear();
            int t = tmpSet[AddrIMPW].toInt();  // 匝间波形地址
            for (int i=0; i < 400; i++) {
                tmpWave.append(msg[t + i].toInt());
            }
            drawImpWave();

            int s = tmpSet[AddrIMPA].toInt() + msg[AddrData].toInt() - 1;
            double r = msg[s].toDouble()/1000;
            real = QString::number(r, 'f', 3) + "%";
            double j = msg[DataIMPJ].toInt();  // 匝间判定
            double v = msg[DataIMPU].toInt();  // 匝间电压

            QString color = (j == DataNG) ? largeNG : largeOK;
            QString judge = (j == DataNG) ? "NG" : "OK";
            QString volt = QString::number(v/1000, 'f', 3) + "kV";
            impLabels.at(0)->setText(color.arg(volt));
            impLabels.at(1)->setText(color.arg(real));
            impLabels.at(2)->setText(color.arg(judge));
        }
    }
    if (addr == AddrDCRS1) {
        textWeld->clear();
        int r = tmpSet.value(AddrWeld).toInt();
        int s = tmpSet.value(AddrDCRS1).toInt();
        int w = tmpSet.value(AddrDCRSW).toInt();
        int h = tmpSet[s + 1].toInt();
        for (int i=0; i < c; i++) {
            if (i%12 == 0) {
                if (i != 0)
                    textWeld->insertHtml("<br></br>");
                textWeld->insertHtml("&nbsp;&nbsp;");
            }
            double std = tmpSet[w + i].toInt();
            double tmp = msg.value(r+i).toDouble();
            QString str = (abs(tmp-std)/std*1000 < h) ? SmallOK : SmallNG;
            textWeld->insertHtml(str.arg(QString::number(tmp, 'f', tmp >= 10 ? 2 : 3)));
            if (str == SmallNG)
                boxChart->setClr(i);
        }
    }
    if (addr == AddrDCRS2) {
        textChip->clear();
        int r = tmpSet.value(AddrChip).toInt();
        int s = tmpSet.value(AddrDCRS2).toInt();
        for (int i=0; i < c; i++) {
            if (i%12 == 0) {
                if (i != 0)
                    textChip->insertHtml("<br></br>");
                textChip->insertHtml("&nbsp;&nbsp;");
            }
            double h = msg.value(s).toDouble()/1000;  // 单位uΩ换算为mΩ
            double tmp = msg.value(r+i).toDouble();  // 单位为mΩ
            QString str = (tmp > h) ? SmallNG : SmallOK;
            textChip->insertHtml(str.arg(QString::number(tmp, 'f', tmp >= 10 ? 2 : 3)));
            if (str == SmallNG)
                boxChart->setClr(i);
        }
    }
    if (addr == AddrDCRS3) {
        textDiag->clear();
        int r = tmpSet.value(AddrDiag).toInt();
        for (int i=0; i < c; i++) {
            if (i%12 == 0) {
                if (i != 0)
                    textDiag->insertHtml("<br></br>");
                textDiag->insertHtml("&nbsp;&nbsp;");
            }
            int s = tmpSet[AddrDCRS3].toInt();  // 配置地址
            double l = tmpSet[s + 1].toDouble()/1000;
            double h = tmpSet[s + 2].toDouble()/1000;
            double tmp = msg.value(r+i).toDouble()/1000;
            QString str = (tmp > l && tmp < h) ? SmallOK : SmallNG;
            textDiag->insertHtml(str.arg(QString::number(tmp, 'f', tmp >= 10 ? 2 : 3)));
            if (str == SmallNG)
                boxChart->setClr(i);
        }
    }
}

void AppTester::recvAppMsg(QTmpMap msg)
{
    int c = msg.value(0).toInt();
    switch (c) {
    case Qt::Key_Copy:
        tmpSet = msg;
        break;
    case Qt::Key_News:
        recvUpdate(msg);
        break;
    case Qt::Key_Call:
        recvLedMsg(msg);
        break;
    case Qt::Key_Shop:
        recvTmpMsg(msg);
        break;
    default:
        break;
    }
}

void AppTester::showEvent(QShowEvent *e)
{
    this->setFocus();
    initSettings();
    drawDcrWave();
    setViewSize();
    e->accept();
}
