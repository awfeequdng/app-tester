/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       电阻配置界面
*******************************************************************************/
#include "typsetdcr.h"

TypSetDcr::TypSetDcr(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void TypSetDcr::initUI()
{
    initLayout();
    initWeldBar();
    initChipBar();
    initDiagBar();
    initCompBar();
    initViewBar();
    initButtons();
}

void TypSetDcr::initLayout()
{
    boxLayout = new QVBoxLayout;

    QGroupBox *box = new QGroupBox(this);
    box->setLayout(boxLayout);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(box);
}

void TypSetDcr::initCompBar()
{
    QGroupBox *group = new QGroupBox(this);
    group->setTitle(tr("电阻综合"));
    boxLayout->addWidget(group);

    QHBoxLayout *layout = new QHBoxLayout;
    group->setLayout(layout);

    boxTemp = new QCheckBox(this);
    boxTemp->setText(tr("温度折算"));
    boxTemp->setFixedWidth(97);
    layout->addWidget(boxTemp);

    layout->addWidget(new QLabel(tr("折算温度"), this));
    maxTemp = new QDoubleSpinBox(this);
    maxTemp->setDecimals(3);
    maxTemp->setFixedSize(97, 40);
    layout->addWidget(maxTemp);
    layout->addWidget(new QLabel(tr("°C"), this));

    layout->addWidget(new QLabel(tr("测试时间"), this));
    boxTime = new QDoubleSpinBox(this);
    boxTime->setDecimals(3);
    boxTime->setFixedSize(97, 40);
    layout->addWidget(boxTime);
    layout->addWidget(new QLabel(tr("s"), this));

    layout->addStretch();
}

void TypSetDcr::initWeldBar()
{
    QGroupBox *group = new QGroupBox(this);
    group->setTitle(tr("片间电阻"));
    boxLayout->addWidget(group);

    QHBoxLayout *layout = new QHBoxLayout;
    group->setLayout(layout);

    boxWeld = new QCheckBox(this);
    boxWeld->setText(tr("片间电阻"));
    boxWeld->setFixedWidth(97);
    layout->addWidget(boxWeld);

    layout->addWidget(new QLabel(tr("离散允差"), this));
    maxWeld = new QDoubleSpinBox(this);
    maxWeld->setDecimals(3);
    maxWeld->setFixedSize(97, 40);
    layout->addWidget(maxWeld);
    layout->addWidget(new QLabel(tr("%"), this));

    layout->addStretch();

    QComboBox *box = new QComboBox(this);
    box->setView(new QListView);
    box->addItem(tr("自动排序"));
    box->setFixedSize(97, 40);
    layout->addWidget(box);
}

void TypSetDcr::initChipBar()
{
    QGroupBox *group = new QGroupBox(this);
    group->setTitle(tr("焊接电阻"));
    boxLayout->addWidget(group);

    QHBoxLayout *layout = new QHBoxLayout;
    group->setLayout(layout);

    boxChip = new QCheckBox(this);
    boxChip->setText(tr("焊接电阻"));
    boxChip->setFixedWidth(97);
    layout->addWidget(boxChip);

    layout->addWidget(new QLabel(tr("合格上限"), this));

    maxChip = new QDoubleSpinBox(this);
    maxChip->setDecimals(3);
    maxChip->setFixedSize(97, 40);
    layout->addWidget(maxChip);

    layout->addWidget(new QLabel(tr("mΩ"), this));

    layout->addStretch();
}

void TypSetDcr::initDiagBar()
{
    QGroupBox *group = new QGroupBox(this);
    group->setTitle(tr("跨间电阻"));
    boxLayout->addWidget(group);

    QHBoxLayout *layout = new QHBoxLayout;
    group->setLayout(layout);

    boxDiag = new QCheckBox(this);
    boxDiag->setText(tr("跨间电阻"));
    boxDiag->setFixedWidth(97);
    layout->addWidget(boxDiag);

    layout->addWidget(new QLabel(tr("合格范围"), this));

    minDiag = new QDoubleSpinBox(this);
    minDiag->setDecimals(3);
    minDiag->setMaximum(9999.999);
    minDiag->setFixedSize(97, 40);
    layout->addWidget(minDiag);

    layout->addWidget(new QLabel(tr("~"), this));

    maxDiag = new QDoubleSpinBox(this);
    maxDiag->setDecimals(3);
    maxDiag->setMaximum(9999.999);
    maxDiag->setFixedSize(97, 40);
    layout->addWidget(maxDiag);

    layout->addWidget(new QLabel(tr("Ω"), this));

    layout->addStretch();

    QComboBox *box = new QComboBox(this);
    box->setView(new QListView);
    box->addItem(tr("挂钩顺序"));
    box->setFixedSize(97, 40);
    layout->addWidget(box);
}

void TypSetDcr::initViewBar()
{
    view = new QTableWidget(this);
    view->setRowCount(6);
    view->setColumnCount(12);
    view->verticalHeader()->hide();
    view->horizontalHeader()->hide();

    for (int i=0; i < 12; i++) {
        for (int j=0; j < 8; j++) {
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setTextAlignment(Qt::AlignCenter);
            view->setItem(j, i, item);
            if (i%2 == 0) {
                item->setText(QString("%1").arg(i*3 + j + 1, 3, 10, QChar('0')));
            }
        }
    }
    //    connect(view, SIGNAL(cellChanged(int, int)), this, SLOT(autoInput(int, int)));
#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    view->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    view->verticalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    //    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    for (int i=0; i < 12; i++) {
        if (i%2 == 0) {
            view->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Fixed);
            view->setColumnWidth(i, 36);
        } else {
            view->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
        }
        view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    }
#endif
    boxLayout->addWidget(view);
}

void TypSetDcr::initButtons()
{
    QHBoxLayout *layout = new QHBoxLayout;
    boxLayout->addLayout(layout);

    QPushButton *prev = new QPushButton(this);
    prev->setFixedSize(97, 40);
    prev->setText(tr("上一页"));
    layout->addWidget(prev);
    connect(prev, SIGNAL(clicked(bool)), this, SLOT(clickButtons()));

    QLineEdit *page = new QLineEdit(this);
    page->setText("1");
    page->setFixedSize(50, 36);
    page->setAlignment(Qt::AlignCenter);
    layout->addWidget(page);

    QPushButton *next = new QPushButton(this);
    next->setFixedSize(97, 40);
    next->setText(tr("下一页"));
    layout->addWidget(next);
    connect(next, SIGNAL(clicked(bool)), this, SLOT(clickButtons()));

    layout->addStretch();

    grade = new QComboBox(this);
    grade->setView(new QListView);
    grade->setFixedSize(97, 40);
    grade->addItem(tr("自动"));
    grade->addItem(tr("100Ω+"));
    grade->addItem(tr("10Ω+"));
    grade->addItem(tr("1Ω+"));
    grade->addItem(tr("50mΩ+"));
    grade->addItem(tr("50mΩ-"));
    layout->addWidget(grade);

    QPushButton *btnCell = new QPushButton(this);
    btnCell->setFixedSize(97, 40);
    btnCell->setText(tr("采样"));
    layout->addWidget(btnCell);
    connect(btnCell, SIGNAL(clicked(bool)), this, SLOT(sample()));

    QPushButton *btnSave = new QPushButton(this);
    btnSave->setFixedSize(97, 40);
    btnSave->setText(tr("保存"));
    layout->addWidget(btnSave);
    connect(btnSave, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));
}

void TypSetDcr::initSettings()
{
    int s = 0;
    s = tmpSet[AddrDCRS1].toInt();
    boxWeld->setChecked(tmpSet[s + 0] == "1" ? Qt::Checked : Qt::Unchecked);
    maxWeld->setValue(tmpSet[s + 1].toDouble()/1000);
    boxTemp->setChecked(tmpSet[s + 2] == "1" ? Qt::Checked : Qt::Unchecked);
    maxTemp->setValue(tmpSet[s + 3].toDouble()/1000);
    boxTime->setValue(tmpSet[s + 4].toDouble()/1000);
    grade->setCurrentIndex(tmpSet[s + 5].toInt());

    s = tmpSet[AddrDCRS2].toInt();
    boxChip->setChecked(tmpSet[s + 0] == "1" ? Qt::Checked : Qt::Unchecked);
    maxChip->setValue(tmpSet[s + 1].toDouble()/1000);

    s = tmpSet[AddrDCRS3].toInt();
    boxDiag->setChecked(tmpSet[s + 0] == "1" ? Qt::Checked : Qt::Unchecked);
    minDiag->setValue(tmpSet[s + 1].toDouble()/1000);
    maxDiag->setValue(tmpSet[s + 2].toDouble()/1000);

    int c = tmpSet[tmpSet[AddrModel].toInt()].toInt();
    int r = tmpSet[AddrDCRSW].toInt();
    for (int i=0; i < 36; i++) {
        if (i < c) {
            double t = tmpSet[r + i*2 + 0].toDouble();
            double p = tmpSet[r + i*2 + 1].toDouble();
            t = t*qPow(10, -p);
            view->item(i%6, (i/6)*2+1)->setText(tr("%1").arg(t, 0, 'f', p));
        } else {
            view->item(i%6, (i/6)*2+1)->setText("");
        }
    }
}

void TypSetDcr::saveSettings()
{
    int s = 0;
    s = tmpSet[AddrDCRS1].toInt();
    tmpSet[s + 0] = boxWeld->isChecked() ? "1" : "0";
    tmpSet[s + 1] = QString::number(maxWeld->value()*1000);
    tmpSet[s + 2] = boxTemp->isChecked() ? "1" : "0";
    tmpSet[s + 3] = QString::number(maxTemp->value()*1000);
    tmpSet[s + 4] = QString::number(boxTime->value()*1000);
    tmpSet[s + 5] = QString::number(grade->currentIndex());

    s = tmpSet[AddrDCRS2].toInt();
    tmpSet[s + 0] = boxChip->isChecked() ? "1" : "0";
    tmpSet[s + 1] = QString::number(maxChip->value()*1000);

    s = tmpSet[AddrDCRS3].toInt();
    tmpSet[s + 0] = boxDiag->isChecked() ? "1" : "0";
    tmpSet[s + 1] = QString::number(minDiag->value()*1000);
    tmpSet[s + 2] = QString::number(maxDiag->value()*1000);

    tmpSet.insert(AddrEnum, Qt::Key_Memo);
    emit sendAppMsg(tmpSet);
}

void TypSetDcr::clickButtons()
{
}

void TypSetDcr::sample()
{
    tmpMsg.insert(AddrEnum, Qt::Key_Send);
    tmpMsg.insert(AddrText, AddrDCRS1);
    tmpMsg.insert(AddrFreq, grade->currentIndex());
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
}

void TypSetDcr::recvUpdate(QTmpMap msg)
{
    int t = 0x04;
    int c = tmpSet[tmpSet[AddrModel].toInt() + 1].toInt();  // 夹具针数
    int r = tmpSet[AddrDCRSW].toInt();  // 电阻标准
    int s = tmpSet[AddrDCRR1].toInt();  // 电阻结果
    for (int i=0; i < c; i++) {
        tmpSet[r + i*2 + 0] = msg[s + t*(i + 1) + AddrDataR];
        tmpSet[r + i*2 + 1] = msg[s + t*(i + 1) + AddrDataS];
    }
    initSettings();
}

void TypSetDcr::recvAppMsg(QTmpMap msg)
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

void TypSetDcr::showEvent(QShowEvent *e)
{
    initSettings();
    e->accept();
}

