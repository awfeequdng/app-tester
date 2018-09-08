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
    QHBoxLayout *layout = new QHBoxLayout;

    QGroupBox *group = new QGroupBox(tr("电阻综合"), this);
    group->setLayout(layout);
    boxLayout->addWidget(group);

    boxTemp = new QCheckBox(tr("温度折算"), this);
    boxTemp->setFixedWidth(97);
    layout->addWidget(boxTemp);
    connect(boxTemp, SIGNAL(clicked(bool)), this, SLOT(change()));

    layout->addWidget(new QLabel(tr("折算温度"), this));
    maxTemp = new QDoubleSpinBox(this);
    maxTemp->setDecimals(3);
    maxTemp->setFixedSize(97, 40);
    layout->addWidget(maxTemp);
    layout->addWidget(new QLabel(tr("°C"), this));
    connect(maxTemp, SIGNAL(valueChanged(double)), this, SLOT(change()));

    layout->addWidget(new QLabel(tr("测试时间"), this));
    boxTime = new QDoubleSpinBox(this);
    boxTime->setMaximum(2);
    boxTime->setDecimals(3);
    boxTime->setFixedSize(97, 40);
    layout->addWidget(boxTime);
    layout->addWidget(new QLabel(tr("s"), this));
    connect(boxTime, SIGNAL(valueChanged(double)), this, SLOT(change()));

    layout->addStretch();
}

void TypSetDcr::initWeldBar()
{
    QHBoxLayout *layout = new QHBoxLayout;

    QGroupBox *group = new QGroupBox(tr("片间电阻"), this);
    group->setLayout(layout);
    boxLayout->addWidget(group);

    boxWeld = new QCheckBox(tr("片间电阻"), this);
    boxWeld->setFixedWidth(97);
    layout->addWidget(boxWeld);
    connect(boxWeld, SIGNAL(clicked(bool)), this, SLOT(change()));

    layout->addWidget(new QLabel(tr("离散允差"), this));
    maxWeld = new QDoubleSpinBox(this);
    maxWeld->setDecimals(2);
    maxWeld->setFixedSize(97, 40);
    layout->addWidget(maxWeld);
    layout->addWidget(new QLabel(tr("%"), this));
    connect(maxWeld, SIGNAL(valueChanged(double)), this, SLOT(change()));

    layout->addStretch();

    boxTurn = new QComboBox(this);
    boxTurn->setView(new QListView);
    boxTurn->setFixedSize(97, 40);
    boxTurn->addItem(tr("顺时针"));
    boxTurn->addItem(tr("逆时针"));
    layout->addWidget(boxTurn);
    connect(boxTurn, SIGNAL(currentIndexChanged(int)), this, SLOT(change()));
}

void TypSetDcr::initChipBar()
{
    QHBoxLayout *layout = new QHBoxLayout;

    QGroupBox *group = new QGroupBox(tr("焊接电阻"), this);
    group->setLayout(layout);
    boxLayout->addWidget(group);

    boxChip = new QCheckBox(tr("焊接电阻"), this);
    boxChip->setFixedWidth(97);
    layout->addWidget(boxChip);
    connect(boxChip, SIGNAL(clicked(bool)), this, SLOT(change()));

    layout->addWidget(new QLabel(tr("合格上限"), this));
    maxChip = new QDoubleSpinBox(this);
    maxChip->setDecimals(3);
    maxChip->setFixedSize(97, 40);
    layout->addWidget(maxChip);
    layout->addWidget(new QLabel(tr("mΩ"), this));
    connect(maxChip, SIGNAL(valueChanged(double)), this, SLOT(change()));

    layout->addStretch();
}

void TypSetDcr::initDiagBar()
{
    QHBoxLayout *layout = new QHBoxLayout;

    QGroupBox *group = new QGroupBox(tr("跨间电阻"), this);
    group->setLayout(layout);
    boxLayout->addWidget(group);

    boxDiag = new QCheckBox(tr("跨间电阻"), this);
    boxDiag->setFixedWidth(97);
    layout->addWidget(boxDiag);
    connect(boxDiag, SIGNAL(clicked(bool)), this, SLOT(change()));

    layout->addWidget(new QLabel(tr("合格范围"), this));
    minDiag = new QDoubleSpinBox(this);
    minDiag->setDecimals(3);
    minDiag->setMaximum(9999.999);
    minDiag->setFixedSize(97, 40);
    layout->addWidget(minDiag);
    connect(minDiag, SIGNAL(valueChanged(double)), this, SLOT(autoChange()));

    layout->addWidget(new QLabel(tr("~"), this));

    maxDiag = new QDoubleSpinBox(this);
    maxDiag->setDecimals(3);
    maxDiag->setMaximum(9999.999);
    maxDiag->setFixedSize(97, 40);
    layout->addWidget(maxDiag);
    layout->addWidget(new QLabel(tr("Ω"), this));
    connect(maxDiag, SIGNAL(valueChanged(double)), this, SLOT(autoChange()));

    layout->addStretch();
}

void TypSetDcr::initViewBar()
{
    view = new QTableWidget(this);
    view->setRowCount(6);
    view->setColumnCount(12);
    QStringList items;
    for (int i=0; i< 6; i++) {
        items << tr("片间%1*12").arg(i+1);
    }
    view->setVerticalHeaderLabels(items);
    view->horizontalHeader()->setVisible(false);

    for (int i=0; i < 12; i++) {
        for (int j=0; j < 6; j++) {
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setTextAlignment(Qt::AlignCenter);
            view->setItem(j, i, item);
        }
    }
#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    view->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    view->verticalHeader()->setResizeMode(QHeaderView::Stretch);
    //    view->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
#else
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //    view->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
#endif
    //    view->setColumnWidth(0, 72);
    boxLayout->addWidget(view);
}

void TypSetDcr::initButtons()
{
    QHBoxLayout *layout = new QHBoxLayout;
    boxLayout->addLayout(layout);

    layout->addStretch();

    btnWaveS = new QPushButton(this);
    btnWaveS->setFixedSize(97, 40);
    btnWaveS->setText(tr("电阻采样"));
    btnWaveS->setEnabled(false);
    layout->addWidget(btnWaveS);
    connect(btnWaveS, SIGNAL(clicked(bool)), this, SLOT(sampleAuto()));

    btnWaveD = new QPushButton(this);
    btnWaveD->setFixedSize(97, 40);
    btnWaveD->setText(tr("取消样品"));
    layout->addWidget(btnWaveD);
    connect(btnWaveD, SIGNAL(clicked(bool)), this, SLOT(sampleBack()));

    btnWaveC = new QPushButton(this);
    btnWaveC->setFixedSize(97, 40);
    btnWaveC->setText(tr("平均计算"));
    btnWaveC->setEnabled(false);
    layout->addWidget(btnWaveC);
    connect(btnWaveC, SIGNAL(clicked(bool)), this, SLOT(sampleOver()));

    btnSave = new QPushButton(this);
    btnSave->setFixedSize(97, 40);
    btnSave->setText(tr("保存"));
    layout->addWidget(btnSave);
    connect(btnSave, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));
}

void TypSetDcr::initSettings()
{
    isInit = false;
    work = WORKL;
    int s = 0;
    s = tmpSet.value(4000 + Qt::Key_1).toInt();  // 片间配置地址
    boxWeld->setChecked(tmpSet[s + ISCKDCR1] == "1" ? Qt::Checked : Qt::Unchecked);
    maxWeld->setValue(tmpSet[s + SMAXDCR1].toDouble()/1000);
    boxTemp->setChecked(tmpSet[s + ISTMDCR1] == "1" ? Qt::Checked : Qt::Unchecked);
    maxTemp->setValue(tmpSet[s + TEMPDCR1].toDouble()/1000);
    boxTime->setValue(tmpSet[s + TIMEDCR1].toDouble()/1000);
    boxTurn->setCurrentIndex(tmpSet[s + TURNDCR1].toDouble());

    s = tmpSet.value(4000 + Qt::Key_2).toInt();  // 焊接配置地址
    boxChip->setChecked(tmpSet[s + ISCHDCR2] == "1" ? Qt::Checked : Qt::Unchecked);
    maxChip->setValue(tmpSet[s + SMAXDCR2].toDouble()/1000);

    s = tmpSet.value(4000 + Qt::Key_3).toInt();  // 跨间配置地址
    boxDiag->setChecked(tmpSet[s + ISCHDCR3] == "1" ? Qt::Checked : Qt::Unchecked);
    minDiag->setValue(tmpSet[s + SMINDCR3].toDouble()/1000);
    maxDiag->setValue(tmpSet[s + SMAXDCR3].toDouble()/1000);
    isInit = true;
}

void TypSetDcr::initViewData()
{
    int conf = tmpSet.value(4000 + Qt::Key_0).toInt();
    int tool = tmpSet.value(conf + 0x00).toInt();  // 电枢片数
    int addr = tmpSet.value(4000 + Qt::Key_9).toInt();  // 标准波形地址
    for (int i=0; i < 72; i++) {
        if (i < tool) {
            double t = tmpSet[addr + i*2 + 0].toDouble();
            double p = tmpSet[addr + i*2 + 1].toDouble();
            p = (p > 3) ? p-3 : p;
            t = t * qPow(10, -p);
            view->item(i/12, i%12)->setText(QString::number(t, 'f', p));
        } else {
            view->item(i/12, i%12)->setText("");
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
    tmpMsg[s + TURNDCR1] = boxTurn->currentIndex();

    s = tmpSet[(4000 + Qt::Key_2)].toInt();
    tmpMsg[s + ISCHDCR2] = boxChip->isChecked() ? "1" : "0";
    tmpMsg[s + SMAXDCR2] = QString::number(maxChip->value()*1000);

    s = tmpSet[(4000 + Qt::Key_3)].toInt();
    tmpMsg[s + ISCHDCR3] = boxDiag->isChecked() ? "1" : "0";
    tmpMsg[s + SMINDCR3] = QString::number(minDiag->value()*1000);
    tmpMsg[s + SMAXDCR3] = QString::number(maxDiag->value()*1000);

    s = tmpSet[(4000 + Qt::Key_9)].toInt();
    for (int i=0; i < 72; i++) {
        if (view->item(i/12, i%12)->text().isEmpty())
            break;
        double p = tmpSet[s + i*2 + 1].toDouble();
        tmpMsg.insert(s + i*2 + 1, p);
        p = (p > 3) ? p-3 : p;
        double r = view->item(i/12, i%12)->text().toDouble();
        tmpMsg[s + i*2] = r * qPow(10, p);
    }
    tmpMsg.insert(Qt::Key_0, Qt::Key_Save);
    tmpMsg.insert(Qt::Key_1, "aip_config");
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
}

void TypSetDcr::autoChange()
{
    change();
    if (isInit && (minDiag->value() > maxDiag->value())) {
        QMessageBox::warning(this, tr("警告"), tr("下限大于上限"), QMessageBox::Ok);
        minDiag->setValue(0);
    }
}

void TypSetDcr::change()
{
    if (isInit) {  // 初始化完成后才发送界面修改
        tmpMsg.insert(Qt::Key_0, Qt::Key_Plus);
        tmpMsg.insert(Qt::Key_1, this->objectName());
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
    }
}

void TypSetDcr::clearView()
{
    for (int i=0; i < 12; i++) {
        for (int j=0; j < 6; j++) {
            view->item(j, i)->setText("");
        }
    }
}

void TypSetDcr::sampleAuto()
{
    test = 0;
    gear = 0;
    tmpMsg.insert(Qt::Key_0, Qt::Key_Send);
    tmpMsg.insert(Qt::Key_1, Qt::Key_1);
    tmpMsg.insert(Qt::Key_2, "auto");
    tmpMsg.insert(Qt::Key_4, work);
    tmpMsg.insert(Qt::Key_5, 0);
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
    clearView();
}

void TypSetDcr::sampleTest()
{
    tmpMsg.insert(Qt::Key_0, Qt::Key_Send);
    tmpMsg.insert(Qt::Key_1, Qt::Key_1);
    tmpMsg.insert(Qt::Key_2, "test");
    tmpMsg.insert(Qt::Key_4, work);
    tmpMsg.insert(Qt::Key_5, gear);
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
    clearView();
    gear = 0;
}

void TypSetDcr::sampleBack()
{
    if (waveCopys.size() > 0) {
        int s = waveCopys.size();
        btnWaveC->setText((s >= 1) ? tr("平均计算%1").arg(s) : tr("平均计算"));
        btnWaveC->setEnabled((s >= 2) ? true : false);
        waveCopys.removeLast();
    }
}

void TypSetDcr::sampleOver()
{
    if (waveCopys.size() < 2)
        return;
    tmpMsg.clear();
    int loop = 0x04;
    int addr = tmpSet.value(3000 + Qt::Key_1).toInt();  // 片间结果地址
    int conf = tmpSet.value(4000 + Qt::Key_0).toInt();  // 综合配置地址
    int real = tmpSet.value(4000 + Qt::Key_9).toInt();  // 电阻标准地址
    int quan = tmpSet.value(conf + AddrDCRSC).toInt();
    int tool = tmpSet.value(conf + AddrDEVSC).toInt() * quan;  // 夹具针数
    int gear = 0;
    for (int i=0; i < waveCopys.size(); i++) {
        QTmpMap tmp = waveCopys.at(i);
        for (int j=0; j < tool; j++) {
            volatile int t1 = tmp.value(addr + CACHEDCR + loop*j + DATADCRR).toInt();
            volatile int t2 = tmp.value(addr + CACHEDCR + loop*j + GEARDCRR).toInt();
            volatile int t3 = 0;
            if (i != 0)
                t3 = tmpMsg.value(real + j*2 + 0).toInt();
            gear = (j == 0) ? t2 : gear;
            tmpMsg.insert(real + j*2 + 0, t3 + t1);
        }
    }
    for (int i=0; i < tool; i++) {
        volatile double t3 = tmpMsg.value(real + i*2 + 0).toInt();
        tmpMsg.insert(real + i*2 + 0, int(t3/waveCopys.size()));
    }
    for (int i=0; i < tmpMsg.keys().size(); i++) {
        tmpSet.insert(tmpMsg.keys().at(i), tmpMsg.value(tmpMsg.keys().at(i)));
    }
    initViewData();
    initBtnShow();
}

void TypSetDcr::initBtnShow()
{
    waveCopys.clear();
    btnWaveC->setText(tr("平均计算"));
    btnWaveC->setEnabled(false);
}

void TypSetDcr::recvUpdate(QTmpMap msg)
{
    if (msg.value(Qt::Key_1).toInt() == Qt::Key_1) {
        int loop = 0x04;
        int addr = tmpSet.value(3000 + Qt::Key_1).toInt();  // 片间结果地址
        int conf = tmpSet.value(4000 + Qt::Key_0).toInt();  // 综合配置地址
        int parm = tmpSet.value(4000 + Qt::Key_1).toInt();  // 片间配置地址
        int real = tmpSet.value(4000 + Qt::Key_9).toInt();  // 电阻标准地址
        int quan = tmpSet.value(conf + AddrDCRSC).toInt();
        int tool = tmpSet.value(conf + AddrDEVSC).toInt() * quan;  // 夹具针数
        for (int i=0; i < tool; i++) {
            int t1 = msg.value(addr + CACHEDCR + loop*i + DATADCRR).toInt();
            int t2 = msg.value(addr + CACHEDCR + loop*i + GEARDCRR).toInt();
            gear = (gear == 0) ? t2 : (qMin(gear, t2));
            tmpSet.insert(real + i*2 + 0, t1);
            tmpSet.insert(real + i*2 + 1, gear);
        }
        tmpSet.insert(parm + GEARDCR1, gear);
        if (test % 2 == 1) {
            change();
            initViewData();
            waveCopys.append(msg);
            btnWaveC->setText(tr("平均计算%1").arg(waveCopys.size()));
            btnWaveC->setEnabled(waveCopys.size() >= 2 ? true : false);
            boxTurn->setCurrentIndex(msg.value(addr + TURNDCRA).toInt());  // 自动更新挂钩顺序
        }
        test++;
        if (test % 2 == 1) {
            sampleTest();
        }
    }
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
    case Qt::Key_Call:
        if (this->isHidden())
            break;
        btnWaveS->setEnabled(false);
        break;
    case Qt::Key_Play:
        if (this->isHidden())
            break;
        work = msg.value(Qt::Key_1).toInt();
        btnWaveS->setEnabled(true);
        break;
    case Qt::Key_Save:
        if (!this->isHidden())
            saveSettings();
        break;
    default:
        break;
    }
}

void TypSetDcr::showEvent(QShowEvent *e)
{
    initSettings();
    initViewData();
    initBtnShow();
    e->accept();
}

