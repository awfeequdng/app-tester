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

    from = new QSpinBox(this);
    from->setFixedHeight(35);
    from->setMaximum(IMP_SIZE);
    layout->addWidget(from);

    layout->addWidget(new QLabel(tr("计算终点:"), this));

    stop = new QSpinBox(this);
    stop->setFixedHeight(35);
    stop->setMaximum(IMP_SIZE);
    layout->addWidget(stop);

    layout->addStretch();

    QPushButton *btnPrev = new QPushButton(this);
    btnPrev->setFixedSize(97, 40);
    btnPrev->setText(tr("上一个"));
    layout->addWidget(btnPrev);
    connect(btnPrev, SIGNAL(clicked(bool)), this, SLOT(waveSwitch()));

    numb = new QLabel(this);
    numb->setFixedWidth(30);
    layout->addWidget(numb);

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

    text = new QLabel(this);
    layout->addWidget(text);

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
    int s = tmpSet[AddrIMPS1].toInt();
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
    numb->setText(tmpSet[s + w].toString());
    w = AddrIMPSO;  // 补偿
    mView->item(0, w)->setText(tmpSet[s + w].toString());
    w = AddrIMPSA;  // 间隔
    mView->item(0, w)->setText(tmpSet[s + w].toString());
    w = AddrIMPSF;  // 频率
    text->setText(tmpSet[s + w].toString());
    w = AddrIMPF1;  // 起点
    from->setValue(qMax(1, tmpSet.value(s + w).toInt()));
    w = AddrIMPF2;  // 终点
    stop->setValue(qMax(200, tmpSet.value(s + w).toInt()));
    impView->setText(QString::number(from->value()), 3);
    impView->setText(QString::number(stop->value()), 2);

    int r = tmpSet[AddrIMPSW].toInt();  // 波形存储地址
    r += numb->text().toInt() * IMP_SIZE;
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
    int s = tmpSet[AddrIMPS1].toInt();
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
    tmpMsg[s + w] = text->text();
    w = AddrIMPF1;  // 起点
    tmpMsg[s + w] = QString::number(from->value());
    w = AddrIMPF2;  // 终点
    tmpMsg[s + w] = QString::number(stop->value());

    int r = tmpSet[AddrIMPSW].toInt();  // 波形存储地址
    int c = tmpSet[tmpSet[AddrModel].toInt()].toInt();  // 电枢片数
    for (int i=0; i < c*IMP_SIZE; i++) {
        tmpMsg.insert(r + i, tmpSet.value(r + i));
    }
    tmpMsg.insert(AddrEnum, Qt::Key_Save);
    tmpMsg.insert(AddrText, "aip_config");
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
    QString s = QString("编号:%1").arg(numb->text().toInt(), 2, 10, QChar('0'));
    impView->setText(s, 1);
    impView->update();
}

void TypSetImp::lineUpdate()
{
    from->setValue(impView->getFrom());
    stop->setValue(impView->getStop());
}

void TypSetImp::waveSwitch()
{
    int addr = tmpSet[AddrModel].toInt();  // 综合配置地址
    int quan = tmpSet[addr + 0].toInt();  // 电枢片数

    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    int n = numb->text().toInt();
    if (btn->text() == tr("上一个")) {
        n--;
    }
    if (btn->text() == tr("下一个")) {
        n++;
    }
    n = qMin(quan, n);
    n = qMax(1, n);
    numb->setText(QString::number(n));

    int r = tmpSet[AddrIMPSW].toInt();  // 波形存储地址
    r += (n-1)*IMP_SIZE;
    tmpWave.clear();
    for (int i=0; i < IMP_SIZE; i++) {
        tmpWave.append(tmpSet[r + i].toInt());
    }
    drawImpWave();
}

void TypSetImp::waveUpdate()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    int f = text->text().toInt();
    if (btn->text() == tr("压缩")) {
        f = (f >= 14) ? 14 : f + 1;
    }
    if (btn->text() == tr("拉伸")) {
        f = (f <= 0) ? 0 : f -1;
    }
    text->setText(QString::number(f));
    tmpMsg.insert(AddrEnum, Qt::Key_Send);
    tmpMsg.insert(AddrText, AddrIMPS1);
    tmpMsg.insert(AddrFreq, f);
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
    time = 0;
}

void TypSetImp::recvUpdate(QTmpMap msg)
{
    int real = tmpSet[AddrIMPR1].toInt();  // 测试结果地址
    int addr = tmpSet[AddrIMPW1].toInt();  // 测试波形地址
    int parm = tmpSet[AddrIMPSW].toInt();  // 标准波形地址
    int imps = msg.value(real + STATIMPA).toInt();  // 状态
    int code = msg.value(real + NUMBIMPA).toInt();  // 编号
    int stdn = tmpSet[tmpSet[AddrIMPS1].toInt() + AddrIMPSL].toInt();  // 采样点

    if (imps == DataTest) {
        tmpWave.clear();
        for (int i=0; i < IMP_SIZE; i++) {
            tmpWave.append(msg[addr + i].toInt());
            tmpSet[parm + (code-1)*IMP_SIZE + i] = msg[addr + i].toInt();
        }
        if (code == stdn) {
            impWave = tmpWave;
        }
        numb->setText(QString::number(code));
    } else if (stdn != 0) {
        tmpWave = impWave;
        numb->setText(QString::number(stdn));
    }
    drawImpWave();
}

void TypSetImp::recvAppMsg(QTmpMap msg)
{
    int c = msg.value(0).toInt();
    switch (c) {
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
        if (msg.value(AddrText).toInt() == AddrIMPS1)
            btnWave->click();
        if (msg.value(AddrText).toInt() == AddrIMPSW)
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

