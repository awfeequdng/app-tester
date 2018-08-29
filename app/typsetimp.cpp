/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       匝间配置界面
*******************************************************************************/
#include "typsetimp.h"

TypSetImp::TypSetImp(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void TypSetImp::initUI()
{
    initLayout();
    initViewDat();
    initViewBar();
    initWaveBar();
    initWaveCtl();
    initButtons();
    initDelegate();
}

void TypSetImp::initLayout()
{
    boxLayout = new QVBoxLayout(this);
    boxLayout->addStretch();

    QGroupBox *box = new QGroupBox(this);
    box->setLayout(boxLayout);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(box);
}

void TypSetImp::initViewDat()
{
    work = WORKL;
    numbcalc = 0;
    QStringList headers;
    headers << tr("测试") << tr("匝间测试") << tr("峰值电压V") << tr("冲击次数")
            << tr("合格上限") << tr("采样点") << tr("电压补偿") << tr("冲击间隔");

    QStringList names;
    names << tr("匝间测试");

    mView = new BoxQModel(this);
    mView->setRowCount(names.size());
    mView->setColumnCount(headers.size());
    mView->setHorizontalHeaderLabels(headers);

    for (int i=0; i < names.size(); i++) {
        for (int j=0; j < headers.size(); j++) {
            mView->setData(mView->index(i, j), "", Qt::DisplayRole);
        }
        mView->item(i, 0)->setCheckable(true);
    }
    //    connect(mView, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(autoInput()));
}

void TypSetImp::initViewBar()
{
    view = new QTableView(this);
    view->setFixedHeight(80);
    view->setModel(mView);
    view->verticalHeader()->hide();
    view->horizontalHeader()->setFixedHeight(30);
#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    view->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    view->verticalHeader()->setResizeMode(QHeaderView::Stretch);
    view->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
#else
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
#endif
    view->setColumnWidth(0, 56);
    view->setEditTriggers(QAbstractItemView::AllEditTriggers);

    boxLayout->addWidget(view);
}

void TypSetImp::initWaveBar()
{
    QGroupBox *box = new QGroupBox(this);
    box->setFixedHeight(200);
    boxLayout->addWidget(box);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    box->setLayout(layout);

    impView = new BoxQLabel(this);
    layout->addWidget(impView);
    connect(impView, SIGNAL(clicked()), this, SLOT(lineUpdate()));
}

void TypSetImp::initWaveCtl()
{
    QHBoxLayout *layout = new QHBoxLayout;
    boxLayout->addLayout(layout);

    numbfrom = new QSpinBox(this);
    numbfrom->setFixedHeight(35);
    numbfrom->setMaximum(IMP_SIZE);
    layout->addWidget(numbfrom);
    numbfrom->hide();

    numbstop = new QSpinBox(this);
    numbstop->setFixedHeight(35);
    numbstop->setMaximum(IMP_SIZE);
    layout->addWidget(numbstop);
    numbstop->hide();

    layout->addStretch();

    QPushButton *btnPrev = new QPushButton(this);
    btnPrev->setFixedSize(97, 40);
    btnPrev->setText(tr("上一个"));
    layout->addWidget(btnPrev);
    connect(btnPrev, SIGNAL(clicked(bool)), this, SLOT(waveSwitch()));

    numbcurr = new QLabel(this);
    numbcurr->setFixedWidth(30);
    layout->addWidget(numbcurr);

    QPushButton *btnNext = new QPushButton(this);
    btnNext->setFixedSize(97, 40);
    btnNext->setText(tr("下一个"));
    layout->addWidget(btnNext);
    connect(btnNext, SIGNAL(clicked(bool)), this, SLOT(waveSwitch()));
}

void TypSetImp::initButtons()
{
    boxLayout->addStretch();
    QHBoxLayout *layout = new QHBoxLayout;
    boxLayout->addLayout(layout);

    layout->addWidget(new QLabel(tr("频率:"), this));

    numbfreq = new QLabel(this);
    layout->addWidget(numbfreq);

    layout->addStretch();

    btnSmin = new QPushButton(this);
    btnSmin->setFixedSize(97, 40);
    btnSmin->setText(tr("压缩"));
    btnSmin->setEnabled(false);
    layout->addWidget(btnSmin);
    connect(btnSmin, SIGNAL(clicked(bool)), this, SLOT(sample()));

    btnSmax = new QPushButton(this);
    btnSmax->setFixedSize(97, 40);
    btnSmax->setText(tr("拉伸"));
    btnSmax->setEnabled(false);
    layout->addWidget(btnSmax);
    connect(btnSmax, SIGNAL(clicked(bool)), this, SLOT(sample()));

    btnWave = new QPushButton(this);
    btnWave->setFixedSize(97, 40);
    btnWave->setText(tr("采样"));
    btnWave->setEnabled(false);
    layout->addWidget(btnWave);
    connect(btnWave, SIGNAL(clicked(bool)), this, SLOT(sample()));

    btnCalc = new QPushButton(this);
    btnCalc->setFixedSize(97, 40);
    btnCalc->setText(tr("计算"));
    btnCalc->setEnabled(false);
    layout->addWidget(btnCalc);
    connect(btnCalc, SIGNAL(clicked(bool)), this, SLOT(calc()));

    btnSave = new QPushButton(this);
    btnSave->setFixedSize(97, 40);
    btnSave->setText(tr("保存"));
    layout->addWidget(btnSave);
    connect(btnSave, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));
}

void TypSetImp::initDelegate()
{
    BoxDouble *volt = new BoxDouble;
    volt->setDecimals(0);
    volt->setMaxinum(3000);

    BoxDouble *time = new BoxDouble;
    time->setDecimals(0);
    time->setMaxinum(1);

    view->setItemDelegateForColumn(0, new BoxQItems);
    view->setItemDelegateForColumn(1, new BoxQItems);
    view->setItemDelegateForColumn(2, volt);
    view->setItemDelegateForColumn(3, time);
}

void TypSetImp::initSettings()
{
    int w = 0;
    int s = tmpSet[(4000 + Qt::Key_8)].toInt();
    w = AddrIMPSC; // 测试
    Qt::CheckState c = (tmpSet[s + w]) == "1" ? Qt::Checked : Qt::Unchecked;
    mView->item(0, w)->setCheckState(c);
    w = AddrIMPSN;  // 名称
    mView->item(0, w)->setText(tmpSet[s + w].toString());
    w = AddrIMPSV;  // 电压
    mView->item(0, w)->setText(tmpSet[s + w].toString());
    w = AddrIMPST;  // 次数
    mView->item(0, w)->setText(tmpSet[s + w].toString());
    w = AddrIMPSH;  // 上限
    mView->item(0, w)->setText(tmpSet[s + w].toString());
    w = AddrIMPSL;  // 下限
    mView->item(0, w)->setText(tmpSet[s + w].toString());
    numbcurr->setText(tmpSet[s + w].toString());
    w = AddrIMPSO;  // 补偿
    mView->item(0, w)->setText(tmpSet[s + w].toString());
    w = AddrIMPSA;  // 间隔
    mView->item(0, w)->setText(tmpSet[s + w].toString());
    w = AddrIMPSF;  // 频率
    numbfreq->setText(tmpSet[s + w].toString());
    w = AddrIMPF1;  // 起点
    numbfrom->setValue(qMax(1, tmpSet.value(s + w).toInt()));
    w = AddrIMPF2;  // 终点
    numbstop->setValue(qMax(20, tmpSet.value(s + w).toInt()));
    impView->setText(QString::number(numbfrom->value()), 3);
    impView->setText(QString::number(numbstop->value()), 2);

    int r = tmpSet[(4000 + Qt::Key_A)].toInt();  // 波形存储地址
    r += numbcurr->text().toInt() * IMP_SIZE;
    tmpWave.clear();
    for (int i=0; i < IMP_SIZE; i++) {
        tmpWave.append(tmpSet[r + i].toInt());
    }
    drawImpWave();
}

void TypSetImp::saveSettings()
{
    btnSave->setEnabled(false);
    int w = 0;
    int s = tmpSet[(4000 + Qt::Key_8)].toInt();
    w = AddrIMPSC; // 测试
    tmpMsg[s + w] = mView->item(0, w)->checkState() == Qt::Checked ? 1 : 0;
    w = AddrIMPSN;  // 名称
    tmpMsg[s + w] = mView->item(0, w)->text();
    w = AddrIMPSV;  // 电压
    tmpMsg[s + w] = mView->item(0, w)->text();
    w = AddrIMPST;  // 次数
    tmpMsg[s + w] = mView->item(0, w)->text();
    w = AddrIMPSH;  // 上限
    tmpMsg[s + w] = mView->item(0, w)->text();
    w = AddrIMPSL;  // 下限
    tmpMsg[s + w] = mView->item(0, w)->text();
    w = AddrIMPSO;  // 补偿
    tmpMsg[s + w] = mView->item(0, w)->text();
    w = AddrIMPSA;  // 间隔
    tmpMsg[s + w] = mView->item(0, w)->text();
    w = AddrIMPSF;  // 频率
    tmpMsg[s + w] = numbfreq->text();
    w = AddrIMPF1;  // 起点
    tmpMsg[s + w] = QString::number(numbfrom->value());
    w = AddrIMPF2;  // 终点
    tmpMsg[s + w] = QString::number(numbstop->value());

    int r = tmpSet[(4000 + Qt::Key_A)].toInt();  // 波形存储地址
    int c = tmpSet[tmpSet[(4000 + Qt::Key_0)].toInt()].toInt();  // 电枢片数
    for (int i=0; i < c*IMP_SIZE; i++) {
        tmpMsg.insert(r + i, tmpSet.value(r + i));
    }
    tmpMsg.insert(Qt::Key_0, Qt::Key_Save);
    tmpMsg.insert(Qt::Key_1, "aip_config");
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
}

void TypSetImp::drawImpWave()
{
    if (tmpWave.isEmpty()) {
        impView->setZero();
    } else {
        QVector<double> y(IMP_SIZE);
        for (int i=0; i < IMP_SIZE; i++) {
            y[i] = tmpWave.at(i)*100/0x0400;
        }
        impView->setWave(y, 4);
    }
    QString s = QString("编号:%1").arg(numbcurr->text().toInt(), 2, 10, QChar('0'));
    impView->setText(s, 1);
    impView->update();
}

void TypSetImp::lineUpdate()
{
    numbfrom->setValue(impView->getFrom());
    numbstop->setValue(impView->getStop());
    impView->setText(QString::number(impView->getFrom()), 3);
    impView->setText(QString::number(impView->getStop()), 2);
}

void TypSetImp::waveSwitch()
{
    volatile int addr = tmpSet.value(4000 + Qt::Key_0).toInt();  // 综合配置地址
    volatile int parm = tmpSet.value(4000 + Qt::Key_8).toInt();  // 匝间配置地址
    int quan = tmpSet.value(addr + AddrDCRSC).toInt();  // 电枢片数
    int spac = tmpSet.value(parm + AddrIMPSA).toInt();  // 采样间隔
    quan = (spac * 2 == quan) ? quan / 2 : quan;  // 对角测试

    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    int numb = numbcurr->text().toInt();
    if (btn->text() == tr("上一个")) {
        numb--;
    }
    if (btn->text() == tr("下一个")) {
        numb++;
    }
    numb = qMin(quan, numb);
    numb = qMax(1, numb);
    numbcurr->setText(QString::number(numb));

    int real = tmpSet.value(4000 + Qt::Key_A).toInt();  // 波形存储地址
    real += (numb-1)*IMP_SIZE;
    tmpWave.clear();
    for (int i=0; i < IMP_SIZE; i++) {
        tmpWave.append(tmpSet.value(real + i).toInt());
    }
    drawImpWave();
}

void TypSetImp::sample()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    int freq = numbfreq->text().toInt();
    if (btn->text() == tr("压缩")) {
        freq = (freq >= 14) ? 14 : freq + 1;
        tmpBuf.clear();
        numbcalc = 0;
        btnCalc->setText(tr("计算"));
        btnCalc->setEnabled(false);
    }
    if (btn->text() == tr("拉伸")) {
        freq = (freq <= 0) ? 0 : freq -1;
        tmpBuf.clear();
        numbcalc = 0;
        btnCalc->setText(tr("计算"));
        btnCalc->setEnabled(false);
    }
    numbfreq->setText(QString::number(freq));
    tmpMsg.insert(Qt::Key_0, Qt::Key_Send);
    tmpMsg.insert(Qt::Key_1, Qt::Key_8);
    tmpMsg.insert(Qt::Key_4, work);
    tmpMsg.insert(Qt::Key_5, freq);
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
    time = 0;
}

void TypSetImp::calc()
{
    volatile int conf = tmpSet.value(4000 + Qt::Key_0).toInt();  // 综合测试参数地址
    volatile int parm = tmpSet.value(4000 + Qt::Key_8).toInt();  // 匝间测试参数地址
    volatile int stdw = tmpSet.value(4000 + Qt::Key_A).toInt();  // 匝间标准波形地址
    volatile int quan = tmpSet.value(conf + AddrDCRSC).toInt();  // 片间数量
    volatile int spac = tmpSet.value(parm + AddrIMPSA).toInt();  // 采样间隔
    volatile int stdp = tmpSet.value(parm + AddrIMPSL).toInt();  // 采样点
    quan = (quan == spac * 2) ? quan / 2 : quan;  // 是否对角
    tmpWave.clear();
    for (int i=0; i < quan; i++) {
        for (int j=0; j < IMP_SIZE; j++) {
            int addr = stdw + i * IMP_SIZE + j;
            int t = tmpBuf.value(addr).toInt()  / numbcalc;
            tmpSet.insert(addr, t);
            if (stdp - 1 == i || (stdp == 0 && i == quan - 1)) {
                tmpWave.append(t);
            }
        }
    }
    tmpBuf.clear();
    numbcalc = 0;
    btnCalc->setText(tr("计算"));
    btnCalc->setEnabled(false);
    drawImpWave();
}

void TypSetImp::recvUpdate(QTmpMap msg)
{
    volatile int real = tmpSet.value(3000 + Qt::Key_8).toInt();  // 匝间测试结果地址
    volatile int tmpw = tmpSet.value(3000 + Qt::Key_A).toInt();  // 匝间测试波形地址
    volatile int parm = tmpSet.value(4000 + Qt::Key_8).toInt();  // 匝间测试参数地址
    volatile int stdw = tmpSet.value(4000 + Qt::Key_A).toInt();  // 匝间标准波形地址
    volatile int stdn = tmpSet.value(parm + AddrIMPSL).toInt();  // 采样点
    volatile int tmps = msg.value(real + STATIMPA).toInt();  // 状态
    volatile int tmpn = msg.value(real + NUMBIMPA).toInt();  // 编号
    if (tmps == DataTest) {
        tmpWave.clear();
        for (int i=0; i < IMP_SIZE; i++) {
            int t = msg.value(tmpw + i).toInt();
            tmpWave.append(t);
            tmpSet.insert(stdw + (tmpn-1)*IMP_SIZE + i, t);
            int k = tmpBuf.value(stdw + (tmpn-1)*IMP_SIZE + i).toInt();
            tmpBuf.insert(stdw + (tmpn-1)*IMP_SIZE + i, t + k);
        }
        if (tmpn == stdn) {
            impWave = tmpWave;
        }
        numbcurr->setText(QString::number(tmpn));
    } else {
        numbcalc++;
        numbcurr->setText(QString::number(stdn));
        btnCalc->setText(tr("计算%1").arg(numbcalc));
        if (numbcalc >= 4)
            btnCalc->setEnabled(true);
    }
    if (tmps != DataTest && stdn != 0) {
        tmpWave = impWave;
    }
    drawImpWave();
}

void TypSetImp::recvAppMsg(QTmpMap msg)
{
    switch (msg.value(Qt::Key_0).toInt()) {
    case Qt::Key_Copy:
        tmpSet = msg;
        btnSave->setEnabled(true);
        break;
    case Qt::Key_News:
        if (this->isHidden())
            return;
        recvUpdate(msg);
        break;
    case Qt::Key_Zoom:
        if (msg.value(Qt::Key_1).toInt() == Qt::Key_8)
            btnWave->click();
        if (msg.value(Qt::Key_1).toInt() == Qt::Key_Save)
            btnSave->click();
        break;
    case Qt::Key_Call:
        if (this->isHidden())
            break;
        btnWave->setEnabled(false);
        btnSmax->setEnabled(false);
        btnSmin->setEnabled(false);
        break;
    case Qt::Key_Play:
        if (this->isHidden())
            break;
        work = msg.value(Qt::Key_1).toString() == "R" ? WORKR : WORKL;
        btnWave->setEnabled(true);
        btnSmax->setEnabled(true);
        btnSmin->setEnabled(true);
        break;
    default:
        break;
    }
}

void TypSetImp::showEvent(QShowEvent *e)
{
    this->setFocus();
    initSettings();
    e->accept();
}

