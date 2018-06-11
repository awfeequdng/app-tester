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
    initViewBar();
    initWaveBar();
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

void TypSetImp::initViewBar()
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

    QPushButton *btnWave = new QPushButton(this);
    btnWave->setFixedSize(97, 40);
    btnWave->setText(tr("采样"));
    layout->addWidget(btnWave);
    connect(btnWave, SIGNAL(clicked(bool)), this, SLOT(waveUpdate()));

    QPushButton *btnSave = new QPushButton(this);
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
    w = AddrIMPSO;  // 补偿
    mView->item(0, w)->setText(tmpSet[s + w].toString());
    w = AddrIMPSA;  // 间隔
    mView->item(0, w)->setText(tmpSet[s + w].toString());
    w = AddrIMPSF;  // 频率
    text->setText(tmpSet[s + w].toString());

    int r = tmpSet[AddrIMPSW].toInt();  // 波形存储地址
    tmpWave.clear();
    for (int i=0; i < 400; i++) {
        tmpWave.append(tmpSet[r + i].toInt());
    }
    drawImpWave();
}

void TypSetImp::saveSettings()
{
    int w = 0;
    int s = tmpSet[AddrIMPS1].toInt();
    w = AddrIMPSC; // 测试
    tmpSet[s + w] = mView->item(0, w)->checkState() == Qt::Checked ? 1 : 0;
    w = AddrIMPSN;  // 名称
    tmpSet[s + w] = mView->item(0, w)->text();
    w = AddrIMPSV;  // 电压
    tmpSet[s + w] = mView->item(0, w)->text();
    w = AddrIMPST;  // 次数
    tmpSet[s + w] = mView->item(0, w)->text();
    w = AddrIMPSH;  // 上限
    tmpSet[s + w] = mView->item(0, w)->text();
    w = AddrIMPSL;  // 下限
    tmpSet[s + w] = mView->item(0, w)->text();
    w = AddrIMPSO;  // 补偿
    tmpSet[s + w] = mView->item(0, w)->text();
    w = AddrIMPSA;  // 间隔
    tmpSet[s + w] = mView->item(0, w)->text();
    w = AddrIMPSF;  // 频率
    tmpSet[s + w] = text->text();

    tmpSet.insert(AddrEnum, Qt::Key_Save);
    tmpSet.insert(AddrText, "aip_config");
    emit sendAppMsg(tmpSet);
}

void TypSetImp::drawImpWave()
{
    if (tmpWave.isEmpty()) {
        impView->setZero();
    } else {
        QVector<double> y(400);
        for (int i=0; i < 400; i++) {
            y[i] = tmpWave.at(i)*100/0x0400;
        }
        impView->setWave(y, 0);
    }
    impView->update();
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
    int imps = msg.value(real + AddrDataS).toInt();  // 状态
    int numb = msg.value(real + AddrDataR).toInt();  // 编号
    int stdn = tmpSet[tmpSet[AddrIMPS1].toInt() + AddrIMPSL].toInt();  // 采样点

    if (imps == DataTest) {
        tmpWave.clear();
        for (int i=0; i < 400; i++) {
            tmpWave.append(msg[addr + i].toInt());
            tmpSet[parm + (numb-1)*400 + i] = msg[addr + i].toInt();
        }
        if (numb == stdn) {
            impWave = tmpWave;
        }
    } else if (stdn != 0) {
        tmpWave = impWave;
    }
    drawImpWave();
}

void TypSetImp::recvAppMsg(QTmpMap msg)
{
    int c = msg.value(0).toInt();
    switch (c) {
    case Qt::Key_Copy:
        tmpSet = msg;
        break;
    case Qt::Key_News:
        if (this->isHidden())
            return;
        recvUpdate(msg);
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

