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

    layout->addWidget(new QLabel(tr("计算起点:"), this));

    numbfrom = new QSpinBox(this);
    numbfrom->setFixedHeight(35);
    numbfrom->setMaximum(IMP_SIZE);
    layout->addWidget(numbfrom);

    layout->addWidget(new QLabel(tr("计算终点:"), this));

    numbstop = new QSpinBox(this);
    numbstop->setFixedHeight(35);
    numbstop->setMaximum(IMP_SIZE);
    layout->addWidget(numbstop);

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

    QPushButton *btnVacuo = new QPushButton(this);
    btnVacuo->setFixedSize(97, 40);
    btnVacuo->setText(tr("压缩"));
    layout->addWidget(btnVacuo);
    connect(btnVacuo, SIGNAL(clicked(bool)), this, SLOT(waveUpdate()));

    QPushButton *btnStretch = new QPushButton(this);
    btnStretch->setFixedSize(97, 40);
    btnStretch->setText(tr("拉伸"));
    layout->addWidget(btnStretch);
    connect(btnStretch, SIGNAL(clicked(bool)), this, SLOT(waveUpdate()));

    btnWave = new QPushButton(this);
    btnWave->setFixedSize(97, 40);
    btnWave->setText(tr("采样"));
    layout->addWidget(btnWave);
    connect(btnWave, SIGNAL(clicked(bool)), this, SLOT(waveUpdate()));

    btnSave = new QPushButton(this);
    btnSave->setFixedSize(97, 40);
    btnSave->setText(tr("保存"));
    layout->addWidget(btnSave);
    connect(btnSave, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));
}

void TypSetImp::initDelegate()
{
    view->setItemDelegateForColumn(0, new BoxQItems);
    view->setItemDelegateForColumn(1, new BoxQItems);
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
}

void TypSetImp::waveSwitch()
{
    int addr = tmpSet.value(4000 + Qt::Key_0).toInt();  // 综合配置地址
    int quan = tmpSet.value(addr + AddrDCRSC).toInt();  // 电枢片数

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
        tmpWave.append(tmpSet[real + i].toInt());
    }
    drawImpWave();
}

void TypSetImp::waveUpdate()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    int freq = numbfreq->text().toInt();
    if (btn->text() == tr("压缩")) {
        freq = (freq >= 14) ? 14 : freq + 1;
    }
    if (btn->text() == tr("拉伸")) {
        freq = (freq <= 0) ? 0 : freq -1;
    }
    numbfreq->setText(QString::number(freq));
    tmpMsg.insert(Qt::Key_0, Qt::Key_Send);
    tmpMsg.insert(Qt::Key_1, Qt::Key_8);
    tmpMsg.insert(Qt::Key_5, freq);
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
    time = 0;
}

void TypSetImp::recvUpdate(QTmpMap msg)
{
    int real = tmpSet[(3000 + Qt::Key_8)].toInt();  // 测试结果地址
    int addr = tmpSet[(3000 + Qt::Key_A)].toInt();  // 测试波形地址
    int parm = tmpSet[(4000 + Qt::Key_A)].toInt();  // 标准波形地址
    int imps = msg.value(real + STATIMPA).toInt();  // 状态
    int code = msg.value(real + NUMBIMPA).toInt();  // 编号
    int stdn = tmpSet[tmpSet[(4000 + Qt::Key_8)].toInt() + AddrIMPSL].toInt();  // 采样点
    if (imps == DataTest) {
        tmpWave.clear();
        for (int i=0; i < IMP_SIZE; i++) {
            tmpWave.append(msg[addr + i].toInt());
            tmpSet[parm + (code-1)*IMP_SIZE + i] = msg[addr + i].toInt();
        }
        if (code == stdn) {
            impWave = tmpWave;
        }
        numbcurr->setText(QString::number(code));
    } else if (stdn != 0) {
        tmpWave = impWave;
        numbcurr->setText(QString::number(stdn));
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

