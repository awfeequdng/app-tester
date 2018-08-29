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
    initTextACW1();
    initTextACW2();
    initTextACW3();
    initTextACW4();
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
    if (Qt::Key_0) {  // 初始化状态栏显示
        int temp = tmpSet.value((3000 + Qt::Key_0)).toInt();  // 时临数据地址
        int sign = tmpSet.value(temp + TEMPSIGN).toInt();
        int user = tmpSet.value(DataUser).toInt();
        int file = tmpSet.value(DataFile).toInt();
        status->item(0, 1)->setText((sign == 0) ? tr("未登录") : tmpSet.value(user).toString());
        status->item(0, 3)->setText(tmpSet.value(file).toString());

        int addr = tmpSet.value((4000 + Qt::Key_0)).toInt();  // 综合配置地址
        boxChart->setNum(tmpSet.value(addr + AddrDCRSC).toInt()); // 电枢片数
    }
    if (Qt::Key_1) {  // 初始化片间电阻
        int addr = tmpSet.value((4000 + Qt::Key_1)).toInt();  // 片间配置地址
        int test = tmpSet.value(addr + ISCKDCR1).toInt();  // 是否测试
        int gear = tmpSet.value(addr + GEARDCR1).toInt();  // 测试档位
        QString tt = test == 1 ? titleOK : titleNG;
        QString gg = gear >= 4 ? "mΩ" : "Ω";
        tt += "&nbsp;&nbsp;%1&nbsp;&nbsp;%2%3%&nbsp;%4%5</p>";
        tt = tt.arg("片间电阻").arg("离散:±");
        tt = tt.arg(QString::number(tmpSet[addr+1].toDouble()/1000, 'f', 2));
        tt = tt.arg(tr("单位:")).arg(gg);
        dcrTitles.at(0)->setText(tt);
        textDCR1->clear();
        dcrView->setZero();
        dcrView->update();

        QString ww = (test == 1) ? largeOK : largeEN;
        dcrLabels.at(0)->setText(ww.arg("挂钩顺序"));
        dcrLabels.at(1)->setText(ww.arg("----"));
        dcrLabels.at(2)->setText(ww.arg("--"));
    }
    if (Qt::Key_2) {  // 初始化焊接电阻
        int addr = tmpSet.value((4000 + Qt::Key_2)).toInt();  // 焊接配置地址
        int test = tmpSet.value(addr + ISCHDCR2).toInt();  // 是否测试
        QString tt = test == 1 ? titleOK : titleNG;
        tt += "&nbsp;&nbsp;%1&nbsp;&nbsp;&lt; %2mΩ</p>";
        tt = tt.arg("焊接电阻").arg(QString::number(tmpSet[addr+1].toDouble()/1000, 'f', 2));
        dcrTitles.at(1)->setText(tt);
        textDCR2->clear();
    }

    if (Qt::Key_3) {  // 初始化跨间电阻
        double addr = tmpSet.value((4000 + Qt::Key_3)).toInt();  // 跨间配置地址
        double test = tmpSet.value(addr + ISCHDCR3).toInt();
        double smin = tmpSet.value(addr + SMINDCR3).toDouble();
        double smax = tmpSet.value(addr + SMAXDCR3).toDouble();
        QString tt = test == 1 ? titleOK : titleNG;
        tt += "&nbsp;&nbsp;%1&nbsp;&nbsp;%2:%3~%4Ω</p>";
        tt = tt.arg("跨间电阻").arg("均值");
        tt = tt.arg(QString::number(smin/1000, 'f', 3)).arg(QString::number(smax/1000, 'f', 3));
        dcrTitles.at(2)->setText(tt);
        textDCR3->clear();
    }
    for (int i=0; i < 4; i++) {  // 初始化介电强度
        double addr = tmpSet.value((4000 + Qt::Key_4) + i).toInt();  // 介电强度配置地址
        double test = tmpSet.value(addr + AddrACWSC).toInt();
        double volt = tmpSet.value(addr + AddrACWSV).toDouble()/1000;
        double smax = tmpSet.value(addr + AddrACWSH).toDouble();
        double smin = tmpSet.value(addr + AddrACWSL).toDouble();
        smax = (i == NUMBINRS) ? smax : smax/100;
        smin = (i == NUMBINRS) ? smin : smin/100;
        QString n = tmpSet.value(addr + AddrACWSN).toString();
        QString tt = test == 1 ? titleOK : titleNG;
        if (i == NUMBINRS) {
            tt += "&nbsp;&nbsp;%1&nbsp;&nbsp;DC:%2kV&nbsp;&nbsp;IR:%3MΩ</p>";
            tt = tt.arg(n).arg(QString::number(volt, 'f', 3));
            tt = tt.arg(smin);
        } else {
            tt += "&nbsp;&nbsp;%1&nbsp;&nbsp;AC:%2kV&nbsp;&nbsp;I:%3~%4mA</p>";
            tt = tt.arg(n).arg(QString::number(volt, 'f', 3));
            tt = tt.arg(QString::number(smin, 'f', 2)).arg(QString::number(smax, 'f', 2));
        }
        acwTitles.at(i)->setText(tt);
        QString cc = (test == 1) ? largeOK : largeEN;
        acwLabels.at(3*i + 0)->setText(cc.arg("-.---kV"));
        acwLabels.at(3*i + 1)->setText(cc.arg((i == NUMBINRS) ? "---.-MΩ" : "-.---mA"));
        acwLabels.at(3*i + 2)->setText(cc.arg("--"));
    }
    if (Qt::Key_8) {  // 初始化匝间显示
        double addr = tmpSet.value((4000 + Qt::Key_8)).toInt();  // 匝间配置地址
        double test = tmpSet.value(addr + AddrIMPSC).toInt();
        double volt = tmpSet.value(addr + AddrIMPSV).toDouble()/1000;
        double smax = tmpSet.value(addr + AddrIMPSH).toDouble();
        QString n = tmpSet.value(addr + AddrACWSN).toString();
        QString tt = test == 1 ? titleOK : titleNG;
        tt += "&nbsp;&nbsp;%1&nbsp;&nbsp;AC:%2kV&nbsp;&nbsp;&lt;%3%</p>";
        tt = tt.arg(n).arg(QString::number(volt, 'f', 3));
        tt = tt.arg(QString::number(smax, 'f', 2));
        impTitles.at(0)->setText(tt);
        QString color = (test == 1) ? largeOK : largeEN;
        impLabels.at(0)->setText(color.arg("-.---kV"));
        impLabels.at(1)->setText(color.arg("-.---%"));
        impLabels.at(2)->setText(color.arg("--"));

        tmpWave.clear();
        textIMPR->clear();
        impView->setZero();
        impView->update();
    }
}

void AppTester::initTextView()
{
    for (int i=0; i < 4; i++) {
        int parm = tmpSet[(4000 + Qt::Key_4) + i].toInt();  // 高压配置地址
        if (tmpSet[parm].toInt() == 1) {  // 项目测试
            acwLabels.at(i*3 + 0)->setText(largeOK.arg("-.---kV"));
            if (i == NUMBINRS) {
                acwLabels.at(3*i + 1)->setText(largeOK.arg("---.-MΩ"));
            } else {
                acwLabels.at(3*i + 1)->setText(largeOK.arg("-.---mA"));
            }
            acwLabels.at(3*i + 2)->setText(largeOK.arg("--"));
        }
    }
    for (int i=0; i < 3; i++) {
        int parm = tmpSet[(4000 + Qt::Key_1) + i].toInt();  // 配置地址
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
        int parm = tmpSet[(4000 + Qt::Key_8)].toInt();  // 匝间配置地址
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

void AppTester::drawWaveDCRW(QTmpMap msg)
{
    int stdw = tmpSet.value(4000 + Qt::Key_9).toInt();  // 标准波形地址
    int addr = tmpSet.value(3000 + Qt::Key_1).toInt();  // 实际波形地址
    int parm = tmpSet.value(4000 + Qt::Key_1).toInt();  // 片间配置地址
    int smax = tmpSet.value(parm + SMAXDCR1).toInt();  // 片间上限
    int from = tmpSet[(4000 + Qt::Key_0)].toInt();
    int quan = tmpSet[from + AddrDCRSC].toInt();
    int tool = tmpSet[from + AddrDEVSC].toInt() * quan;
    QVector<double> xx(tool), yh(tool), yl(tool), yr(tool);

    double max = 0;
    double min = 0xffff;
    for (int i=0; i < tool; i++) {
        xx[i] = i;
        double ts = tmpSet.value(stdw + i*2 + 0).toDouble();
        double ps = tmpSet.value(stdw + i*2 + 1).toDouble();
        if (ts == 0)
            return;
        ts = ts * qPow(10, -ps);
        double t1 = ts + ts * smax / 100000;
        double t2 = ts - ts * smax / 100000;
        double rr = msg.value(addr + CACHEDCR + i*4 + DATADCRR).toInt();
        double pp = msg.value(addr + CACHEDCR + i*4 + GEARDCRR).toInt();
        rr = rr * qPow(10, -pp);
        max = qMax(max, t1);
        min = qMin(min, t2);
    }
    double sss = max - min;
    for (int i=0; i < tool; i++) {
        double ts = tmpSet.value(stdw + i*2 + 0).toDouble();
        double ps = tmpSet.value(stdw + i*2 + 1).toDouble();
        ts = ts * qPow(10, -ps);
        double t1 = ts + ts * smax / 100000;
        double t2 = ts - ts * smax / 100000;
        double rr = msg.value(addr + CACHEDCR + i*4 + DATADCRR).toInt();
        double pp = msg.value(addr + CACHEDCR + i*4 + GEARDCRR).toInt();
        rr = rr * qPow(10, -pp);
        yh[i] = (t1 - min) * 80 / sss + 10;
        yl[i] = (t2 - min) * 80 / sss + 10;
        yr[i] = (rr - min) * 80 / sss + 10;
    }
    dcrView->setWave(yr, 0);
    dcrView->setWave(yh, 1);
    dcrView->setWave(yl, 2);
    dcrView->update();
    int turn = msg.value(addr + TURNDCRA).toInt();
    int isok = msg.value(addr + TURNISOK).toInt();
    QString str = (isok == DATAOK) ? largeOK : largeNG;
    dcrLabels.at(0)->setText(str.arg(tr("挂钩顺序")));
    dcrLabels.at(1)->setText(str.arg((turn == 0) ? tr("顺时针") : tr("逆时针")));
    dcrLabels.at(2)->setText(str.arg((isok == DATAOK) ? "OK" : "NG"));
    boxChart->setTurn((isok == DATAOK) ? turn : turn + 2);
}

void AppTester::drawWaveIMPW(int numb)
{
    QVector<double> stds(IMP_SIZE), real(IMP_SIZE);
    int addr = tmpSet[(4000 + Qt::Key_A)].toInt();
    for (int i=0; i < IMP_SIZE; i++) {
        real[i] = tmpWave.at(i)*100/0x0400;
        stds[i] = tmpSet[addr + numb*IMP_SIZE + i].toInt()*100/0x0400;
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
        tmpMsg.insert(Qt::Key_0, Qt::Key_Play);
    } else {
        btnPlay->setText(tr("启动测试"));
        tmpMsg.insert(Qt::Key_0, Qt::Key_Stop);
    }
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
}

void AppTester::clickButton()
{
    tmpMsg.insert(Qt::Key_0, Qt::Key_View);
    tmpMsg.insert(Qt::Key_1, sender()->objectName());
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
}

void AppTester::recvTmpMsg(QTmpMap msg)
{
    int addr = tmpSet.value((3000 + Qt::Key_0)).toInt();
    if (!msg.value(addr + TEMPTEMP).isNull()) {
        double temp = msg.value(addr + TEMPTEMP).toInt();
        if (temp > 0x8000) {
            temp = 0x8000 - temp;
        }
        status->item(1, 1)->setText(QString::number(temp/10, 'f', 1));
    }
}

void AppTester::recvLedMsg(QTmpMap msg)
{
    int stat = msg.value(Qt::Key_2).toInt();
    int beep = msg.value(Qt::Key_3).toInt();
    int work = msg.value(Qt::Key_4).toInt();
    QString isok = judgeNG;
    switch (stat) {
    case DATADC:
        if (beep > 0) {
            boxChart->setRun(0);
            btnHome->setEnabled(true);
            btnConf->setEnabled(true);
            btnPlay->setText(tr("启动测试"));
            textResult->setText(judgeDC);
        }
        break;
    case DATAOK:
        if (beep > 0) {
            testOK++;
            isok = judgeOK;
        }
        //        break;
    case DATANG:
        if (beep > 0) {
            boxChart->setRun(0);
            btnHome->setEnabled(true);
            btnConf->setEnabled(true);
            btnPlay->setText(tr("启动测试"));
            testQu++;
            textResult->setText(isok);
            drawAllRate();
        }
        break;

    case DATAON:  // 启动测试,清理界面
        t.restart();
        initTextView();
        btnHome->setEnabled(false);
        btnConf->setEnabled(false);
        btnPlay->setText(tr("停止测试"));
        textResult->setText(judgeON.arg("ON"));
        boxChart->setRun(1);
        textWorker->setText(judgeON.arg((work == 0x11 ? tr("左") : tr("右"))));
        break;
    case DATADD:
        break;
    default:
        break;
    }
}

void AppTester::recvDCRMsg(QTmpMap msg)
{
    int conf = tmpSet.value(4000 + Qt::Key_0).toInt();
    int quan = tmpSet.value(conf + AddrDCRSC).toInt();
    int curr = msg.value(Qt::Key_1).toInt();
    int addr = tmpSet.value(curr + 3000).toInt() + CACHEDCR;;
    QTextBrowser *textR = textDCR1;
    if (curr == Qt::Key_1) {
        textR = textDCR1;
    }
    if (curr == Qt::Key_2) {
        textR = textDCR2;
    }
    if (curr == Qt::Key_3) {
        textR = textDCR3;
        quan = quan / 2;
    }
    textR->clear();
    for (int i=0; i < quan; i++) {
        double numb = msg.value(addr + i*4 + NUMBDCRR).toInt();
        double real = msg.value(addr + i*4 + DATADCRR).toInt();
        double gear = msg.value(addr + i*4 + GEARDCRR).toInt();
        double isok = msg.value(addr + i*4 + OKNGDCRR).toInt();
        gear = (gear > 3) ? gear - 3 : gear;
        real = real * qPow(10, -gear);
        QString str = (isok == DATAOK) ? SmallOK : SmallNG;
        numb = (curr == Qt::Key_1) ? numb : i;
        tmpMsg.insert(numb, str.arg(tr("%1").arg(QString::number(real, 'f', gear))));
        if (str == SmallNG && curr == Qt::Key_1)
            boxChart->setRow(numb);
        if (str == SmallNG && curr == Qt::Key_2)
            boxChart->setPie(i);
    }
    for (int i=0; i < quan; i++) {
        textR->insertHtml((i%12 == 0 && i != 0) ? "<br></br>" : "");
        textR->insertHtml((i%12 == 0) ? "&nbsp;&nbsp;" : "");
        textR->insertHtml(tmpMsg.value(i).toString());
    }
    tmpMsg.clear();
    if (curr == Qt::Key_1)
        drawWaveDCRW(msg);
}

void AppTester::recvACWMsg(QTmpMap msg)
{
    double curr = msg.value(Qt::Key_1).toInt();  // 当前测试项目
    double numb = curr - Qt::Key_4;  // 第几个测试项
    double addr = tmpSet.value(curr + 3000).toInt();  // 测试结果地址
    double parm = tmpSet.value(curr + 4000).toInt();  // 测试参数地址
    double isok = msg.value(addr + CACHEACW + OKNGACWR).toInt();
    double volt = msg.value(addr + CACHEACW + VOLTACWR).toInt();
    double real = msg.value(addr + CACHEACW + DATAACWR).toInt();
    double gear = msg.value(addr + CACHEACW + GEARACWR).toInt();
    real *= qPow(10, -gear);
    volt = tmpSet.value(parm + AddrACWSV).toInt();
    QString strs = QString::number(real, 'f', 3) + "mA";
    if (numb == NUMBINRS) {
        strs = (real > 500) ? ">500MΩ" : (QString::number(real, 'f', 1) + "MΩ");
    }
    QString color = (isok == DATANG) ? largeNG : largeOK;
    acwLabels.at(3*numb + 0)->setText(color.arg(QString::number(volt/1000, 'f', 3) + "kV"));
    acwLabels.at(3*numb + 1)->setText(color.arg(strs));
    acwLabels.at(3*numb + 2)->setText(color.arg((isok == DATANG) ? "NG" : "OK"));
}

void AppTester::recvIMPMsg(QTmpMap msg)
{
    volatile int parm = tmpSet.value(4000 + Qt::Key_8).toInt();  // 测试参数地址
    volatile int addr = tmpSet.value(3000 + Qt::Key_8).toInt();  // 测试结果地址
    volatile int tmpw = tmpSet.value(3000 + Qt::Key_A).toInt();  // 测试波形地址
    volatile int tmps = msg.value(addr + STATIMPA).toInt();  // 状态
    volatile int numb = msg.value(addr + NUMBIMPA).toInt();  // 编号
    volatile int stdn = tmpSet.value(parm + AddrIMPSL).toInt();  // 采样点
    if (tmps >= DataTest) {  // 测试状态更新波形
        tmpWave.clear();
        for (int i=0; i < IMP_SIZE; i++) {
            tmpWave.append(msg.value(tmpw + i).toInt());
        }
        double volt = msg.value(addr + CACHEIMP + 4*(numb-1) + VOLTIMPR).toDouble();
        double real = msg.value(addr + CACHEIMP + 4*(numb-1) + DATAIMPR).toDouble();
        double isok = msg.value(addr + CACHEIMP + 4*(numb-1) + OKNGIMPR).toDouble();
        double gear = real >= 10000 ? 2 : 3;
        QString okng = (isok == DATANG) ? largeNG : largeOK;
        QString strs = (isok == DATANG) ? SmallNG : SmallOK;
        QString tmp = QString::number(real/1000, 'f', gear);
        tmpImp.append(strs.arg(tmp));
        volt = tmpSet.value(parm + AddrIMPSV).toInt();
        impLabels.at(0)->setText(okng.arg(QString::number(volt/1000, 'f', 3) + "kV"));
        impLabels.at(1)->setText(okng.arg(tmp + "%"));
        impLabels.at(2)->setText(okng.arg((isok == DATANG) ? "NG" : "OK"));

        double last = msg.value(addr + CACHEIMP + 4*(impPrev-1) + DATAIMPR).toDouble();
        last = (impPrev == 0) ? 0 : last;
        impWave = (impPrev == 0 || real > last) ? tmpWave : impWave;
        impPrev = (impPrev == 0 || real > last) ? numb : impPrev;
    } else {  // 测试完成更新结果
        for (int i=0; i < tmpImp.size(); i++) {
            textIMPR->insertHtml((i%12 == 0 && i != 0) ? "<br></br>" : "");
            textIMPR->insertHtml((i%12 == 0) ? "&nbsp;&nbsp;" : "");
            textIMPR->insertHtml(tmpImp.at(i));
        }
        tmpImp.clear();
        // 显示最差结果
        double volt = msg[addr + 4*impPrev + VOLTIMPR].toDouble();
        double real = msg[addr + 4*impPrev + DATAIMPR].toDouble();
        double isok = msg[addr + 4*impPrev + OKNGIMPR].toDouble();
        double gear = real >= 10000 ? 2 : 3;
        QString okng = (isok == DATANG) ? largeNG : largeOK;
        impLabels.at(0)->setText(okng.arg(QString::number(volt/1000, 'f', 3) + "kV"));
        impLabels.at(1)->setText(okng.arg(QString::number(real/1000, 'f', gear) + "%"));
        impLabels.at(2)->setText(okng.arg((isok == DATANG) ? "NG" : "OK"));
        tmpWave = impWave;
        numb = impPrev;
        impPrev = 0;
    }
    drawWaveIMPW((stdn == 0) ? numb-1 : stdn-1);
}

void AppTester::recvUpdate(QTmpMap msg)
{
    if (this->isHidden())
        return;
    int curr = msg.value(Qt::Key_1).toInt();
    switch (curr) {
    case Qt::Key_1:
    case Qt::Key_2:
    case Qt::Key_3:
        recvDCRMsg(msg);
        break;
    case Qt::Key_4:
    case Qt::Key_5:
    case Qt::Key_6:
    case Qt::Key_7:
        recvACWMsg(msg);
        break;
    case Qt::Key_8:
        recvIMPMsg(msg);
        break;
    default:
        break;
    }
}

void AppTester::recvAppMsg(QTmpMap msg)
{
    switch (msg.value(Qt::Key_0).toInt()) {
    case Qt::Key_Copy:
        tmpSet = msg;
        if (!msg.value(TEMPCODE).isNull()) {
            status->item(1, 3)->setText(msg.value(TEMPCODE).toString());
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
