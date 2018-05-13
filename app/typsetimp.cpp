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
    drawImpWave();
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
            << tr("合格上限") << tr("合格下限") << tr("电压补偿") << tr("冲击间隔");

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

    impView = new QCustomPlot(this);
    layout->addWidget(impView);
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
    impView->xAxis->setRange(0, 400);
    impView->yAxis->setRange(0, 103);

    impView->xAxis->setTickLabelColor(Qt::white);
    impView->xAxis->setLabelColor(Qt::white);
    impView->xAxis->setTickLabelColor(Qt::white);

    impLine = impView->addGraph();
}

void TypSetImp::initButtons()
{
    boxLayout->addStretch();
    QHBoxLayout *layout = new QHBoxLayout;
    boxLayout->addLayout(layout);

    text = new QLabel(this);
    layout->addWidget(text);

    layout->addStretch();

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

void TypSetImp::drawImpWave()
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
    QString tmp = QString("档位:%1 频率:%2");
    w = AddrIMPSG;
    tmp = tmp.arg(tmpSet[s + w].toString());
    w = AddrIMPSF;
    tmp = tmp.arg(tmpSet[s + w].toString());
    text->setText(tmp);

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
    tmpSet.insert(AddrEnum, Qt::Key_Save);
    emit sendAppMsg(tmpSet);
}

void TypSetImp::waveUpdate()
{
    tmpMsg.insert(AddrEnum, Qt::Key_Send);
    tmpMsg.insert(AddrText, AddrModel);
    emit sendAppMsg(tmpMsg);
    tmpMsg.insert(AddrText, AddrIMPS1);
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();

    prev = 0;
    min = 0xffff;
    minb = 0;
}

int TypSetImp::calc()
{
    quint32 area = 0;
    for (int i=0; i < 400; i++) {
        area += abs(tmpWave.at(i) - 0x200);
    }
    area = area * 100 / 0x400 / 400;
    return area;
}

void TypSetImp::recvUpdate(QTmpMap msg)
{
    int r = msg.value(AddrData).toInt();
    if (r == 0) {
        tmpWave = impWave.mid((minb-1)*400, 400);
        int s = tmpSet[AddrIMPSW].toInt();  // 波形存储地址
        for (int i=0; i < 400; i++) {
            tmpSet[s + i] = tmpWave.at(i);
        }
    } else if (r != prev) {
        prev = r;
        int t = msg[AddrIMPW].toInt();  // 匝间波形地址
        tmpWave.clear();
        for (int i=0; i < 400; i++) {
            tmpWave.append(msg[t + i].toInt());
        }
        impWave.append(tmpWave);
        int c = calc();
        if ( c < min)
            minb = r;
        min = qMin(c, min);
        QString tmp = QString("档位:%1 频率:%2 面积:%3");
        tmp = tmp.arg(msg[AddrIMPG].toString());
        tmp = tmp.arg(msg[AddrIMPF].toString());
        tmp = tmp.arg(calc());
        text->setText(tmp);
    }
    drawImpWave();
    qDebug() << prev << min << minb;
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

