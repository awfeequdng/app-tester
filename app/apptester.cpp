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
    btnLayout->setSpacing(6);

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

    QLabel *btnLogo = new QLabel(this);
    btnLogo->setFixedHeight(49);
    btnLogo->setPixmap(QPixmap(":/icon_aip.png"));
    btnLogo->setScaledContents(true);
    btnLayout->addWidget(btnLogo);

    btnPlay = new QPushButton("启动测试", this);
    btnPlay->setMinimumSize(97, 44);
    connect(btnPlay, SIGNAL(clicked(bool)), this, SLOT(clickStart()));
    btnLayout->addWidget(btnPlay);

#ifdef __arm__
    btnPlay->hide();
#else
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

    graph1 = dcrView->addGraph();
    graph1->setPen(QPen(Qt::white, 1));
    graph2 = dcrView->addGraph();
    graph2->setPen(QPen(Qt::white, 1));
    graph3 = dcrView->addGraph();
    graph3->setPen(QPen(Qt::green, 2));
}

void AppTester::initInrTextCG()
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

void AppTester::initAcwTextAC()
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

void AppTester::initAcwTextAL()
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

void AppTester::initAcwTextLC()
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
    impStdd = impView->addGraph();

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

        QVector<double> x(1), y(1);
        x[0] = -1;
        y[0] = -1;
        graph1->setData(x, y);
        graph2->setData(x, y);
        graph3->setData(x, y);
        dcrView->replot();
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

void AppTester::initViewText()
{
    QVector<double> x(1), y(1);
    x[0] = -1;
    y[0] = -1;
    for (int i=0; i < 4; i++) {
        int parm = tmpSet[AddrACWS1 + i].toInt();  // 高压配置地址
        if (tmpSet[parm].toInt() == 1) {  // 项目测试
            acwLabels.at(i*3 + 0)->setText(largeOK.arg("-.---kV"));
            if (i == 0) {
                acwLabels.at(3*i + 1)->setText(largeOK.arg("---.-MΩ"));
            } else {
                acwLabels.at(3*i + 1)->setText(largeOK.arg("-.---mA"));
            }
            acwLabels.at(3*i + 2)->setText(largeOK.arg("--"));
        }
    }
    for (int i=0; i < 3; i++) {
        int parm = tmpSet[AddrDCRS1 + i].toInt();  // 配置地址
        if (tmpSet[parm].toInt() == 1) {  // 项目测试
            if (i == 0) {
                textWeld->clear();
                graph1->setData(x, y);
                graph2->setData(x, y);
                graph3->setData(x, y);
                dcrView->replot();
            }
            if (i == 1) {
                textChip->clear();
            }
            if (i == 2) {
                textDiag->clear();
            }
        }
    }
    if (1) {
        int parm = tmpSet[AddrIMPS1].toInt();  // 匝间配置地址
        if (tmpSet[parm].toInt() == 1) {
            impLabels.at(0)->setText(largeOK.arg("-.---kV"));
            impLabels.at(1)->setText(largeOK.arg("-.---%"));
            impLabels.at(2)->setText(largeOK.arg("--"));
            tmpWave.clear();
            textIMPR->clear();
            //            impLine->setData(x, y);
            //            impStdd->setData(x, y);
            //            impView->replot();
        }
    }
}

void AppTester::drawImpWave(int numb)
{
    impLine->setPen(QPen(Qt::green, 2));
    impStdd->setPen(QPen(Qt::white, 2));
    QVector<double> x(400), y(400);
    for (int i=0; i < 400; i++) {
        x[i] = i;
        y[i] = tmpWave.at(i)*100/0x0400;
    }
    impLine->setData(x, y);
    int addr = tmpSet[AddrIMPSW].toInt();
    for (int i=0; i < 400; i++) {
        x[i] = i;
        y[i] =  tmpSet[addr + numb*400 + i].toInt()*100/0x0400;
    }
    impStdd->setData(x, y);
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
    int addr = tmpSet[AddrDCRSW].toInt();
    int real = tmpSet[AddrDCRR1].toInt();
    int stdd = tmpSet[tmpSet[AddrDCRS1].toInt() + 1].toInt();
    int c = tmpSet[tmpSet[AddrModel].toInt()].toInt();
    QVector<double> x1(c), y1(c), y2(c), y3(c);

    double max = 0;
    double min = 0xffff;
    for (int i=0; i < c-1; i++) {
        x1[i] = i;
        double ts = tmpSet[addr + i*2 + 0].toDouble();
        double ps = tmpSet[addr + i*2 + 1].toDouble();
        ts = ts * qPow(10, -ps);
        double t1 = ts + ts * stdd / 100000;
        double t2 = ts - ts * stdd / 100000;
        double rr = tmpSet[real + (i+1)*4 + AddrDataR].toInt();
        double pp = tmpSet[real + (i+1)*4 + AddrDataS].toInt();
        rr = rr * qPow(10, -pp);
        max = qMax(max, t1);
        min = qMin(min, t2);
        if (ts == 0)
            return;
        y1[i] = t1;
        y2[i] = t2;
        y3[i] = rr;
    }
    double sss = max - min;
    for (int i=0; i < c-1; i++) {
        double ts = tmpSet[addr + i*2 + 0].toDouble();
        double ps = tmpSet[addr + i*2 + 1].toDouble();
        ts = ts * qPow(10, -ps);
        double t1 = ts + ts * stdd / 100000;
        double t2 = ts - ts * stdd / 100000;
        double rr = tmpSet[real + (i+1)*4 + AddrDataR].toInt();
        double pp = tmpSet[real + (i+1)*4 + AddrDataS].toInt();
        rr = rr * qPow(10, -pp);
        y1[i] = (t1 - min) * 80 / sss + 10;
        y2[i] = (t2 - min) * 80 / sss + 10;
        y3[i] = (rr - min) * 80 / sss + 10;
    }
    graph1->setData(x1, y1);
    graph2->setData(x1, y2);
    graph3->setData(x1, y3);
    dcrView->xAxis->setRange(1.1, c-2);
    dcrView->replot();
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
        initViewText();
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

void AppTester::recvDCRMsg(QTmpMap msg)
{
    int c = tmpSet[tmpSet[AddrModel].toInt()].toInt();
    int curr = msg.value(AddrText).toInt();
    if (curr == AddrDCRS1) {
        textWeld->clear();
        int addr = tmpSet.value(AddrDCRR1).toInt(); // 电阻结果
        for (int i=0; i < c; i++) {
            if (i%12 == 0) {
                if (i != 0)
                    textWeld->insertHtml("<br></br>");
                textWeld->insertHtml("&nbsp;&nbsp;");
            }
            double r = msg.value(addr + (i+1)*4 + AddrDataR).toInt();
            double p = msg.value(addr + (i+1)*4 + AddrDataS).toInt();
            double j = msg.value(addr + (i+1)*4 + AddrDataJ).toInt();
            QString str = (j == DataOK) ? SmallOK : SmallNG;
            textWeld->insertHtml(str.arg(tr("%1").arg(QString::number(r * qPow(10, p-3), 'g', 5))));
            if (str == SmallNG)
                boxChart->setClr(i);
            tmpSet[addr + (i+1)*4 + AddrDataR] = r;
            tmpSet[addr + (i+1)*4 + AddrDataS] = p;
        }
        drawDcrWave();
    }
    if (curr == AddrDCRS2) {
        textChip->clear();
        int addr = tmpSet.value(AddrDCRR2).toInt(); // 电阻结果
        for (int i=0; i < c; i++) {
            if (i%12 == 0) {
                if (i != 0)
                    textChip->insertHtml("<br></br>");
                textChip->insertHtml("&nbsp;&nbsp;");
            }
            double r = msg.value(addr + (i+1)*4 + AddrDataR).toInt();
            double p = msg.value(addr + (i+1)*4 + AddrDataS).toInt();
            double j = msg.value(addr + (i+1)*4 + AddrDataJ).toInt();
            QString str = (j == DataOK) ? SmallOK : SmallNG;
            r = r * qPow(10, -p);  // 焊接电阻
            if (r < 10.0)
                textChip->insertHtml(str.arg(QString::number(r, 'f', p)));
            else
                textChip->insertHtml(str.arg(QString(">10.0")));
            if (str == SmallNG)
                boxChart->setClr(i);
        }
    }
    if (curr == AddrDCRS3) {
        textDiag->clear();
        int addr = tmpSet.value(AddrDCRR3).toInt(); // 电阻结果
        for (int i=0; i < c/2; i++) {
            if (i%12 == 0) {
                if (i != 0)
                    textDiag->insertHtml("<br></br>");
                textDiag->insertHtml("&nbsp;&nbsp;");
            }
            double r = msg.value(addr + (i+1)*4 + AddrDataR).toInt();
            double p = msg.value(addr + (i+1)*4 + AddrDataS).toInt();
            double j = msg.value(addr + (i+1)*4 + AddrDataJ).toInt();
            r = r * qPow(10, -p);  // 跨间电阻
            QString str = (j == DataOK) ? SmallOK : SmallNG;

            textDiag->insertHtml(str.arg(QString::number(r, 'f', p)));
            if (str == SmallNG)
                boxChart->setClr(i);
        }
    }
}

void AppTester::recvACWMsg(QTmpMap msg)
{
    int curr = msg.value(AddrText).toInt();
    int temp = 0x04;
    int t = curr - AddrACWS1;
    int s = tmpSet[AddrACWR1 + t].toInt();
    double j = msg[s + temp + AddrDataJ].toInt();
    double v = msg[s + temp + AddrDataV].toInt();
    double r = msg[s + temp + AddrDataR].toInt();
    double p = msg[s + temp + AddrDataS].toInt();
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

void AppTester::recvIMPMsg(QTmpMap msg)
{
    int parm = tmpSet[AddrIMPR1].toInt();  // 测试结果地址
    int addr = tmpSet[AddrIMPW1].toInt();  // 测试波形地址
    int imps = msg.value(parm + AddrDataS).toInt();  // 状态
    int numb = msg.value(parm + AddrDataR).toInt();  // 编号
    int stdn = tmpSet[tmpSet[AddrIMPS1].toInt() + AddrIMPSL].toInt();  // 采样点
    if (imps == DataTest) {  // 测试状态更新波形
        tmpWave.clear();
        for (int i=0; i < 400; i++) {
            tmpWave.append(msg[addr + i].toInt());
        }
        double volt = msg[parm + 4*numb + AddrDataV].toDouble();
        double real = msg[parm + 4*numb + AddrDataR].toDouble();
        double isok = msg[parm + 4*numb + AddrDataJ].toDouble();
        QString okng = (isok == DataNG) ? largeNG : largeOK;
        impLabels.at(0)->setText(okng.arg(QString::number(volt/1000, 'f', 3) + "kV"));
        impLabels.at(1)->setText(okng.arg(QString::number(real/1000, 'f', real >= 10000 ? 2 : 3) + "%"));
        impLabels.at(2)->setText(okng.arg((isok == DataNG) ? "NG" : "OK"));
        if (numb == 1) {
            prev = 1;
            impWave = tmpWave;
        } else {
            if (real > msg[parm + 4*prev + AddrDataR].toDouble()) {
                prev = numb;
                impWave = tmpWave;
            }
        }
    } else {  // 测试完成更新结果
        int time = tmpSet[tmpSet[AddrModel].toInt()].toInt();
        int impt = tmpSet[AddrIMPS1].toInt() + AddrIMPSA;
        time = (time%2 == 0 && time/2  == tmpSet[impt].toInt()) ? time/2 : time;
        for (int i=0; i < time; i++) {
            if (i%12 == 0) {
                if (i != 0)
                    textIMPR->insertHtml("<br></br>");
                textIMPR->insertHtml("&nbsp;&nbsp;");
            }
            double real = msg.value(parm + (i+1)*4 + AddrDataR).toDouble()/1000;
            double isok = msg.value(parm + (i+1)*4 + AddrDataJ).toInt();
            QString str = (isok == DataOK) ? SmallOK : SmallNG;
            textIMPR->insertHtml(str.arg(tr("%1").arg(real, 0, 'f', real >= 10 ? 2 : 3)));
        }
        double v = msg[parm + 4*prev + AddrDataV].toDouble();
        double r = msg[parm + 4*prev + AddrDataR].toDouble();
        double j = msg[parm + 4*prev + AddrDataJ].toDouble();
        QString okng = (j == DataNG) ? largeNG : largeOK;
        impLabels.at(0)->setText(okng.arg(QString::number(v/1000, 'f', 3) + "kV"));
        impLabels.at(1)->setText(okng.arg(QString::number(r/1000, 'f', r >= 10000 ? 2 : 3) + "%"));
        impLabels.at(2)->setText(okng.arg((j == DataNG) ? "NG" : "OK"));
        tmpWave = impWave;
        numb = prev;
    }
    drawImpWave((stdn == 0) ? numb-1 : stdn);
}

void AppTester::recvUpdate(QTmpMap msg)
{
    if (this->isHidden())
        return;
    int curr = msg.value(AddrText).toInt();
    switch (curr) {
    case AddrDCRS1:
    case AddrDCRS2:
    case AddrDCRS3:
        recvDCRMsg(msg);
        break;
    case AddrACWS1:
    case AddrACWS2:
    case AddrACWS3:
    case AddrACWS4:
        recvACWMsg(msg);
        break;
    case AddrIMPS1:
        recvIMPMsg(msg);
        break;
    default:
        break;
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
