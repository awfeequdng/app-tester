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
    maxWeld->setDecimals(2);
    maxWeld->setFixedSize(97, 40);
    layout->addWidget(maxWeld);
    layout->addWidget(new QLabel(tr("%"), this));

    layout->addStretch();

    QComboBox *box1 = new QComboBox(this);
    box1->setView(new QListView);
    box1->addItem(tr("挂钩顺序"));
    box1->setFixedSize(97, 40);
    layout->addWidget(box1);

    QComboBox *box = new QComboBox(this);
    box->setView(new QListView);
    box->addItem(tr("计算跨阻"));
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
}

void TypSetDcr::initViewBar()
{
    view = new QTableWidget(this);
    view->setRowCount(6);
    view->setColumnCount(13);
    view->verticalHeader()->hide();
    view->horizontalHeader()->hide();

    for (int i=0; i < 13; i++) {
        for (int j=0; j < 6; j++) {
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setTextAlignment(Qt::AlignCenter);
            view->setItem(j, i, item);
            if (i == 0) {
                item->setText(tr("片间%1*12").arg(j+1));
            }
        }
    }
#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    view->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    view->verticalHeader()->setResizeMode(QHeaderView::Stretch);
    view->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
#else
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
#endif
    view->setColumnWidth(0, 72);
    boxLayout->addWidget(view);
}

void TypSetDcr::initButtons()
{
    QHBoxLayout *layout = new QHBoxLayout;
    boxLayout->addLayout(layout);

    layout->addStretch();

    btnWeld = new QPushButton(this);
    btnWeld->setFixedSize(97, 40);
    btnWeld->setText(tr("片间采样"));
    layout->addWidget(btnWeld);
    connect(btnWeld, SIGNAL(clicked(bool)), this, SLOT(sample()));

    btnSave = new QPushButton(this);
    btnSave->setFixedSize(97, 40);
    btnSave->setText(tr("保存"));
    layout->addWidget(btnSave);
    connect(btnSave, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));
}

void TypSetDcr::initSettings()
{
    int s = 0;
    s = tmpSet[(4000 + Qt::Key_1)].toInt();
    boxWeld->setChecked(tmpSet[s + ISCKDCR1] == "1" ? Qt::Checked : Qt::Unchecked);
    maxWeld->setValue(tmpSet[s + SMAXDCR1].toDouble()/1000);
    boxTemp->setChecked(tmpSet[s + ISTMDCR1] == "1" ? Qt::Checked : Qt::Unchecked);
    maxTemp->setValue(tmpSet[s + TEMPDCR1].toDouble()/1000);
    boxTime->setValue(tmpSet[s + TIMEDCR1].toDouble()/1000);

    s = tmpSet[(4000 + Qt::Key_2)].toInt();
    boxChip->setChecked(tmpSet[s + ISCHDCR2] == "1" ? Qt::Checked : Qt::Unchecked);
    maxChip->setValue(tmpSet[s + SMAXDCR2].toDouble()/1000);

    s = tmpSet[(4000 + Qt::Key_3)].toInt();
    boxDiag->setChecked(tmpSet[s + ISCHDCR3] == "1" ? Qt::Checked : Qt::Unchecked);
    minDiag->setValue(tmpSet[s + SMINDCR3].toDouble()/1000);
    maxDiag->setValue(tmpSet[s + SMAXDCR3].toDouble()/1000);
}

void TypSetDcr::initViewData()
{
    int c = tmpSet[tmpSet[(4000 + Qt::Key_0)].toInt()].toInt();  // 电枢片数
    int r = tmpSet[(4000 + Qt::Key_9)].toInt();  // 标准波形地址
    for (int i=0; i < 72; i++) {
        if (i < c) {
            double t = tmpSet[r + i*2 + 0].toDouble();
            double p = tmpSet[r + i*2 + 1].toDouble();
            p = (p > 3) ? p-3 : p;
            t = t * qPow(10, -p);
            view->item(i/12, i%12+1)->setText(QString::number(t, 'f', p));
        } else {
            view->item(i/12, i%12+1)->setText("");
        }
    }
}

void TypSetDcr::saveSettings()
{
    btnSave->setEnabled(false);
    int s = 0;
    s = tmpSet[(4000 + Qt::Key_1)].toInt();
    tmpMsg[s + ISCKDCR1] = boxWeld->isChecked() ? "1" : "0";
    tmpMsg[s + SMAXDCR1] = QString::number(maxWeld->value()*1000);
    tmpMsg[s + ISTMDCR1] = boxTemp->isChecked() ? "1" : "0";
    tmpMsg[s + TEMPDCR1] = QString::number(maxTemp->value()*1000);
    tmpMsg[s + TIMEDCR1] = QString::number(boxTime->value()*1000);
    tmpMsg[s + GEARDCR1] = tmpSet.value(s + GEARDCR1);

    s = tmpSet[(4000 + Qt::Key_2)].toInt();
    tmpMsg[s + ISCHDCR2] = boxChip->isChecked() ? "1" : "0";
    tmpMsg[s + SMAXDCR2] = QString::number(maxChip->value()*1000);

    s = tmpSet[(4000 + Qt::Key_3)].toInt();
    tmpMsg[s + ISCHDCR3] = boxDiag->isChecked() ? "1" : "0";
    tmpMsg[s + SMINDCR3] = QString::number(minDiag->value()*1000);
    tmpMsg[s + SMAXDCR3] = QString::number(maxDiag->value()*1000);

    s = tmpSet[(4000 + Qt::Key_9)].toInt();
    for (int i=0; i < 72; i++) {
        if (view->item(i/12, i%12 + 1)->text().isEmpty())
            break;
        double p = tmpSet[s + i*2 + 1].toDouble();
        tmpMsg.insert(s + i*2 + 1, p);
        p = (p > 3) ? p-3 : p;
        double r = view->item(i/12, i%12 + 1)->text().toDouble();
        tmpMsg[s + i*2] = r * qPow(10, p);
    }
    tmpMsg.insert(Qt::Key_0, Qt::Key_Save);
    tmpMsg.insert(Qt::Key_1, "aip_config");
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
}

void TypSetDcr::sample()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (btn->text() == tr("片间采样")) {
        currItem = (4000 + Qt::Key_1);
    }
    tmpMsg.insert(Qt::Key_0, Qt::Key_Send);
    tmpMsg.insert(Qt::Key_1, currItem);
    tmpMsg.insert(Qt::Key_5, 0);
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
}

void TypSetDcr::recvUpdate(QTmpMap msg)
{
    int t = 0x04;
    int addr = tmpSet[(4000 + Qt::Key_0)].toInt();
    int quan = tmpSet[addr + AddrDCRSC].toInt();
    int tool = tmpSet[addr + AddrDEVSC].toInt() * quan;  // 夹具针数
    int r = tmpSet[(4000 + Qt::Key_9)].toInt();  // 电阻标准

    int g = 0;
    if (currItem == (4000 + Qt::Key_1)) {
        int s = tmpSet[(3000 + Qt::Key_1)].toInt() + CACHEDCR;  // 电阻结果
        for (int i=0; i < tool; i++) {
            int t1 = msg[s + t*i + DATADCRR].toInt();
            int t2 = msg[s + t*i + GEARDCRR].toInt();
            if (i == 0) {
                g = t2;
            }
            tmpSet[r + i*2 + 0] = t1;
            tmpSet[r + i*2 + 1] = g;
        }
        tmpSet[tmpSet[(4000 + Qt::Key_1)].toInt() + 5] = g;
    }
    initViewData();
}

void TypSetDcr::recvAppMsg(QTmpMap msg)
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
        if (msg.value(Qt::Key_1).toInt() == (4000 + Qt::Key_1))
            btnWeld->click();
        if (msg.value(Qt::Key_1).toInt() == (4000 + Qt::Key_3))
            btnDiag->click();
        if (msg.value(Qt::Key_1).toInt() == (4000 + Qt::Key_9))
            btnSave->click();
        break;
    default:
        break;
    }
}

void TypSetDcr::showEvent(QShowEvent *e)
{
    initSettings();
    initViewData();
    e->accept();
}

