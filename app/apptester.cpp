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
    initQuRate();
    initResult();
    initQChart();
    initButton();
    initTextDCR1();
    initTextDCR2();
    initTextDCR3();
    initWaveDCRW();
    initTextACW4();
    initTextACW1();
    initTextACW2();
    initTextACW3();
    initTextIMP1();
    initWaveIMPW();
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
        view->setSpan(i, 0, 2, 2);
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

void AppTester::initQuRate()
{  // 合格率显示区
    testQu = 0;
    testOK = 0;
    QVector<double> y(3);
    y[0] = testQu;
    y[1] = testOK;
    y[2] = testQu - testOK;
    allView = new BoxQLabel(this);
    allView->setData(y, 2);
    view->setCellWidget(0, 1, allView);
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

    btnHome = new QPushButton("开机主页", this);
    btnLayout->addWidget(btnHome);
    btnHome->setObjectName("author");
    btnHome->setMinimumSize(97, 44);
    connect(btnHome, SIGNAL(clicked(bool)), this, SLOT(clickButton()));

    btnConf = new QPushButton("型号管理", this);
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

void AppTester::initTextDCR1()
{
    QLabel *title = new QLabel(this);
    dcrTitles.append(title);

    textDCR1 = new QTextBrowser(this);
    textDCR1->setStyleSheet("border:none");

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(title);
    layout->addWidget(textDCR1);

    QFrame *frame = new QFrame(this);
    frame->setLayout(layout);
    view->setCellWidget(1, 0, frame);
}

void AppTester::initTextDCR2()
{
    QLabel *title = new QLabel(this);
    dcrTitles.append(title);

    textDCR2 = new QTextBrowser(this);
    textDCR2->setStyleSheet("border:none");

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(title);
    layout->addWidget(textDCR2);
    layout->setStretch(1, 1);

    QFrame *frame = new QFrame(this);
    frame->setLayout(layout);
    view->setCellWidget(3, 0, frame);
}

void AppTester::initTextDCR3()
{
    QLabel *title = new QLabel(this);
    dcrTitles.append(title);

    textDCR3 = new QTextBrowser(this);
    textDCR3->setStyleSheet("border:none;");

    QHBoxLayout *largeLayout = new QHBoxLayout;
    largeLayout->setMargin(0);
    for (int i=0; i < 3; i++) {
        QLabel *text = new QLabel(this);
        dcrLabels.append(text);
        largeLayout->addWidget(text);
    }

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(title);
    layout->addWidget(textDCR3);
    layout->addLayout(largeLayout);
    layout->setStretch(1, 1);

    QFrame *frame = new QFrame(this);
    frame->setLayout(layout);
    view->setCellWidget(5, 0, frame);
}

void AppTester::initWaveDCRW()
{
    dcrView = new BoxQLabel(this);
    connect(dcrView, SIGNAL(clicked()), this, SLOT(dcrResize()));
    view->setCellWidget(6, 0, dcrView);
}

void AppTester::initTextACW4()
{
    QLabel *title = new QLabel(this);
    acwTitles.append(title);

    QHBoxLayout *large = new QHBoxLayout;
    large->setMargin(0);
    for (int i=0; i < 3; i++) {
        QLabel *text = new QLabel(this);
        acwLabels.append(text);
        large->addWidget(text);
    }
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(title);
    layout->addLayout(large);
    layout->setStretch(1, 1);

    QFrame *frame = new QFrame(this);
    frame->setLayout(layout);
    view->setCellWidget(4, 2, frame);
}

void AppTester::initTextACW1()
{
    QLabel *title = new QLabel(this);
    acwTitles.append(title);

    QHBoxLayout *large = new QHBoxLayout;
    large->setMargin(0);
    for (int i=0; i < 3; i++) {
        QLabel *text = new QLabel(this);
        acwLabels.append(text);
        large->addWidget(text);
    }
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(title);
    layout->addLayout(large);
    layout->setStretch(1, 1);

    QFrame *frame = new QFrame(this);
    frame->setLayout(layout);
    view->setCellWidget(1, 2, frame);
}

void AppTester::initTextACW2()
{
    QLabel *title = new QLabel(this);
    acwTitles.append(title);

    QHBoxLayout *large = new QHBoxLayout;
    large->setMargin(0);
    for (int i=0; i < 3; i++) {
        QLabel *text = new QLabel(this);
        acwLabels.append(text);
        large->addWidget(text);
    }
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(title);
    layout->addLayout(large);
    layout->setStretch(1, 1);

    QFrame *frame = new QFrame(this);
    frame->setLayout(layout);
    view->setCellWidget(2, 2, frame);
}

void AppTester::initTextACW3()
{
    QLabel *title = new QLabel(this);
    acwTitles.append(title);

    QHBoxLayout *large = new QHBoxLayout;
    large->setMargin(0);
    for (int i=0; i < 3; i++) {
        QLabel *text = new QLabel(this);
        acwLabels.append(text);
        large->addWidget(text);
    }
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(title);
    layout->addLayout(large);
    layout->setStretch(1, 1);

    QFrame *frame = new QFrame(this);
    frame->setLayout(layout);
    view->setCellWidget(3, 2, frame);
}

void AppTester::initTextIMP1()
{
    QLabel *title = new QLabel(this);
    impTitles.append(title);

    QHBoxLayout *large = new QHBoxLayout;
    large->setMargin(0);
    for (int i=0; i < 3; i++) {
        QLabel *text = new QLabel(this);
        impLabels.append(text);
        large->addWidget(text);
    }

    textIMPR = new QTextBrowser(this);
    textIMPR->setStyleSheet("border:none");

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(title);
    layout->addWidget(textIMPR);
    layout->addLayout(large);
    layout->setStretch(1, 1);

    QFrame *frame = new QFrame(this);
    frame->setLayout(layout);
    view->setCellWidget(5, 2, frame);
}

void AppTester::initWaveIMPW()
{
    impView = new BoxQLabel(this);
    connect(impView, SIGNAL(clicked()), this, SLOT(impResize()));
    view->setCellWidget(6, 2, impView);
}

void AppTester::initSettings()
{
    boxChart->setNum(tmpSet[tmpSet[AddrModel].toInt()].toInt());
    int r = 0;
    if (1) {  // 初始化用户显示
        if (tmpSet[DataSign].toInt() == 0) {
            status->item(0, 1)->setText(tr("未登录"));
        } else {
            r = tmpSet[DataUser].toInt();
            status->item(0, 1)->setText(tmpSet[r].toString());
        }
    }
    if (1) {  // 初始化型号显示
        r = tmpSet[DataFile].toInt();
        status->item(0, 3)->setText(tmpSet[r].toString());
    }

    QString tt;
    for (int i=0; i < 4; i++) {  // 初始化介电强度
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
        QString cc = (c == 1) ? largeOK : largeEN;
        acwLabels.at(3*i + 0)->setText(cc.arg("-.---kV"));
        acwLabels.at(3*i + 1)->setText(cc.arg((i == 0) ? "---.-MΩ" : "-.---mA"));
        acwLabels.at(3*i + 2)->setText(cc.arg("--"));
    }
    if (1) {  // 初始化匝间显示
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
        QString color = (c == 1) ? largeOK : largeEN;
        impLabels.at(0)->setText(color.arg("-.---kV"));
        impLabels.at(1)->setText(color.arg("-.---%"));
        impLabels.at(2)->setText(color.arg("--"));

        tmpWave.clear();
        textIMPR->clear();
        impView->setZero();
        impView->update();
    }
    if (1) {  // 初始化片间电阻
        int r = tmpSet[AddrDCRS1].toInt();  // 配置地址
        int c = tmpSet[r+0].toInt();
        QString tt = tmpSet[r+0].toDouble() == 1 ? titleOK : titleNG;
        QString gg = tmpSet[r+5].toDouble() >= 4 ? "mΩ" : "Ω";
        tt += "&nbsp;&nbsp;%1&nbsp;&nbsp;%2%3%&nbsp;%4%5</p>";
        tt = tt.arg("片间电阻").arg("离散:±");
        tt = tt.arg(QString::number(tmpSet[r+1].toDouble()/1000, 'f', 3));
        tt = tt.arg(tr("单位:")).arg(gg);
        dcrTitles.at(0)->setText(tt);
        textDCR1->clear();
        dcrView->setZero();
        dcrView->update();

        QString ww = (c == 1) ? largeOK : largeEN;
        dcrLabels.at(0)->setText(ww.arg("挂钩顺序"));
        dcrLabels.at(1)->setText(ww.arg("------"));
        dcrLabels.at(2)->setText(ww.arg("--"));
    }
    if (2) {  // 初始化焊接电阻
        int r = tmpSet[AddrDCRS2].toInt();  // 配置地址

        QString tt = tmpSet[r+0].toDouble() == 1 ? titleOK : titleNG;
        tt += "&nbsp;&nbsp;%1&nbsp;&nbsp;&lt; %2mΩ</p>";
        tt = tt.arg("焊接电阻").arg(QString::number(tmpSet[r+1].toDouble()/1000, 'f', 3));
        dcrTitles.at(1)->setText(tt);
        textDCR2->clear();
    }
    if (3) {  // 初始化跨间电阻
        int r = tmpSet[AddrDCRS3].toInt();  // 配置地址
        int c = tmpSet[r+0].toInt();
        double l = tmpSet[r + 1].toDouble();
        double h = tmpSet[r + 2].toDouble();
        double g = tmpSet[tmpSet[AddrDCRS3].toInt()+3].toDouble();
        h = g < 4 ? h/1000 : h;
        l = g < 4 ? l/1000 : l;
        QString tt = c == 1 ? titleOK : titleNG;
        QString gg = g < 4 ? "Ω" : "mΩ";
        g = (g > 3) ? g-3 : g;

        tt += "&nbsp;&nbsp;%1&nbsp;&nbsp;%2:%3~%4%5</p>";
        tt = tt.arg("跨间电阻").arg("均值");
        tt = tt.arg(QString::number(l, 'f', 3)).arg(QString::number(h, 'f', g));
        tt = tt.arg(gg);
        dcrTitles.at(2)->setText(tt);
        textDCR3->clear();
    }
}

void AppTester::initTextView()
{
    boxChart->setNum(tmpSet[tmpSet[AddrModel].toInt()].toInt());
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
                textDCR1->clear();
                dcrView->setZero();
                dcrView->update();
            }
            if (i == 1) {
                textDCR2->clear();
            }
            if (i == 2) {
                textDCR3->clear();
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
            impView->setZero();
            impView->update();
        }
        dcrLabels.at(0)->setText(largeOK.arg(tr("挂钩顺序")));
        dcrLabels.at(1)->setText(largeOK.arg("----"));
        dcrLabels.at(2)->setText(largeOK.arg("--"));
    }
}

void AppTester::drawWaveDCRW()
{
    int addr = tmpSet[AddrDCRSW].toInt();  // 标准波形地址
    int real = tmpSet[AddrDCRR1].toInt();  // 实际波形地址
    int stdd = tmpSet[tmpSet[AddrDCRS1].toInt() + 1].toInt();
    int from = tmpSet[AddrModel].toInt();
    int quan = tmpSet[from + AddrDCRSC].toInt();
    int tool = tmpSet[from + AddrDEVSC].toInt() * quan;
    QVector<double> xx(tool), yh(tool), yl(tool), yr(tool);

    double max = 0;
    double min = 0xffff;
    for (int i=0; i < tool; i++) {
        xx[i] = i;
        double ts = tmpSet[addr + i*2 + 0].toDouble();
        double ps = tmpSet[addr + i*2 + 1].toDouble();
        if (ts == 0)
            return;
        ts = ts * qPow(10, -ps);
        double t1 = ts + ts * stdd / 100000;
        double t2 = ts - ts * stdd / 100000;
        double rr = tmpSet[real + (i+1)*4 + AddrDataR].toInt();
        double pp = tmpSet[real + (i+1)*4 + AddrDataS].toInt();
        rr = rr * qPow(10, -pp);
        max = qMax(max, t1);
        min = qMin(min, t2);
    }
    double sss = max - min;
    for (int i=0; i < tool; i++) {
        double ts = tmpSet[addr + i*2 + 0].toDouble();
        double ps = tmpSet[addr + i*2 + 1].toDouble();
        ts = ts * qPow(10, -ps);
        double t1 = ts + ts * stdd / 100000;
        double t2 = ts - ts * stdd / 100000;
        double rr = tmpSet[real + (i+1)*4 + AddrDataR].toInt();
        double pp = tmpSet[real + (i+1)*4 + AddrDataS].toInt();
        rr = rr * qPow(10, -pp);
        yh[i] = (t1 - min) * 80 / sss + 10;
        yl[i] = (t2 - min) * 80 / sss + 10;
        yr[i] = (rr - min) * 80 / sss + 10;
    }
    dcrView->setWave(yr, 0);
    dcrView->setWave(yh, 1);
    dcrView->setWave(yl, 2);
    dcrView->update();
    if (tmpSet[real + 4 + AddrDataV].toInt() == 0) {
        dcrLabels.at(1)->setText(largeOK.arg(tr("顺时针")));
        dcrLabels.at(2)->setText(largeOK.arg("OK"));
        boxChart->setTurn(0);
    } else {
        dcrLabels.at(1)->setText(largeOK.arg(tr("逆时针")));
        dcrLabels.at(2)->setText(largeOK.arg("OK"));
        boxChart->setTurn(1);
    }
}

void AppTester::drawWaveIMPW(int numb)
{
    QVector<double> stds(400), real(400);
    int addr = tmpSet[AddrIMPSW].toInt();
    for (int i=0; i < 400; i++) {
        real[i] = tmpWave.at(i)*100/0x0400;
        stds[i] = tmpSet[addr + numb*400 + i].toInt()*100/0x0400;
    }
    impView->setWave(real, 0);
    impView->setWave(stds, 1);
    impView->update();
}

void AppTester::showViewSize()
{
    for (int i=0; i < 7; i++) {
        view->showRow(i);
    }
    for (int i=0; i < 4; i++) {
        view->showColumn(i);
    }
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

void AppTester::drawAllRate()
{
    if (testQu == 0)
        return;
    QVector<double> y(3);
    y[0] = testQu;
    y[1] = testOK;
    y[2] = testQu - testOK;

    allView->setData(y, 2);
    allView->update();
}

void AppTester::boxResize()
{
    if (view->isRowHidden(1)) {
        showViewSize();
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
        showViewSize();
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

void AppTester::impResize()
{
    if (view->isRowHidden(1)) {
        showViewSize();
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
    //    tmp = (tmp == "LED1") ? "LEDR" : tmp;
    if (msg.value(AddrBeep).isNull() && tmp == "LEDY") {
        initTextView();
        btnHome->setEnabled(false);
        btnConf->setEnabled(false);
        btnPlay->setText(tr("停止测试"));
        textResult->setText(judgeON.arg("ON"));
        boxChart->setRun(1);
    } else if (msg.value(AddrBeep).toInt() >= 0) {
        boxChart->setRun(0);
        btnHome->setEnabled(true);
        btnConf->setEnabled(true);
        btnPlay->setText(tr("启动测试"));
        if (tmp == "LEDG") {
            testQu++;
            testOK++;
            textResult->setText(judgeOK);
        }
        if (tmp == "LEDR") {
            testQu++;
            textResult->setText(judgeNG);
        }
        drawAllRate();
    }
}

void AppTester::recvDCRMsg(QTmpMap msg)
{
    int from = tmpSet[AddrModel].toInt();
    int quan = tmpSet[from].toInt();
    int curr = msg.value(AddrText).toInt();
    if (curr == AddrDCRS1) {
        textDCR1->clear();
        int addr = tmpSet.value(AddrDCRR1).toInt(); // 电阻结果
        for (int i=0; i < quan; i++) {
            if (i%12 == 0) {
                if (i != 0)
                    textDCR1->insertHtml("<br></br>");
                textDCR1->insertHtml("&nbsp;&nbsp;");
            }
            double r = msg.value(addr + (i+1)*4 + AddrDataR).toInt();
            double p = msg.value(addr + (i+1)*4 + AddrDataS).toInt();
            double j = msg.value(addr + (i+1)*4 + AddrDataJ).toInt();
            tmpSet[addr + (i+1)*4 + AddrDataR] = r;
            tmpSet[addr + (i+1)*4 + AddrDataS] = p;
            p = (p > 3) ? p - 3 : p;
            r = r * qPow(10, -p);
            QString str = (j == DataOK) ? SmallOK : SmallNG;
            textDCR1->insertHtml(str.arg(tr("%1").arg(QString::number(r, 'f', p))));
            if (str == SmallNG)
                boxChart->setRow(i);
        }
        tmpSet[addr + 4 + AddrDataV] = msg.value(addr + 4 + AddrDataV).toInt();
        drawWaveDCRW();
    }
    if (curr == AddrDCRS2) {
        textDCR2->clear();
        int addr = tmpSet.value(AddrDCRR2).toInt(); // 电阻结果
        for (int i=0; i < quan; i++) {
            if (i%12 == 0) {
                if (i != 0)
                    textDCR2->insertHtml("<br></br>");
                textDCR2->insertHtml("&nbsp;&nbsp;");
            }
            double r = msg.value(addr + (i+1)*4 + AddrDataR).toInt();
            double p = msg.value(addr + (i+1)*4 + AddrDataS).toInt();
            double j = msg.value(addr + (i+1)*4 + AddrDataJ).toInt();
//            p = (p > 3) ? p - 3 : p;
            QString str = (j == DataOK) ? SmallOK : SmallNG;
            r = r * qPow(10, 3-p);  // 焊接电阻
            if (r < 10.0)
                textDCR2->insertHtml(str.arg(QString::number(r, 'f', p-3)));
            else
                textDCR2->insertHtml(str.arg(QString(">10.0")));
            if (str == SmallNG)
                boxChart->setPie(i);
        }
    }
    if (curr == AddrDCRS3) {
        textDCR3->clear();
        int addr = tmpSet.value(AddrDCRR3).toInt(); // 电阻结果
        for (int i=0; i < quan/2; i++) {
            if (i%12 == 0) {
                if (i != 0)
                    textDCR3->insertHtml("<br></br>");
                textDCR3->insertHtml("&nbsp;&nbsp;");
            }
            double r = msg.value(addr + (i+1)*4 + AddrDataR).toInt();
            double p = msg.value(addr + (i+1)*4 + AddrDataS).toInt();
            double j = msg.value(addr + (i+1)*4 + AddrDataJ).toInt();
            p = (p > 3) ? p - 3 : p;
            r = r * qPow(10, -p);
            QString str = (j == DataOK) ? SmallOK : SmallNG;
            textDCR3->insertHtml(str.arg(QString::number(r, 'f', p)));
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
        QString tmp = QString::number(real/1000, 'f', real >= 10000 ? 2 : 3) + "%";
        impLabels.at(0)->setText(okng.arg(QString::number(volt/1000, 'f', 3) + "kV"));
        impLabels.at(1)->setText(okng.arg(tmp));
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
    drawWaveIMPW((stdn == 0) ? numb-1 : stdn);
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
        if (!msg.value(DataCode).isNull()) {
            status->item(1, 3)->setText(msg.value(DataCode).toString());
        }
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
    showViewSize();
    e->accept();
}
