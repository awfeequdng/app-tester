/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       系统设置
*******************************************************************************/
#include "appsystem.h"

AppSystem::AppSystem(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void AppSystem::initUI()
{
    initLayout();
    initSystem();
    initDevice();
    initButton();
    initDelegate();
}

void AppSystem::initLayout()
{
    isInit = false;
    layout = new QHBoxLayout;

    boxLayout = new QVBoxLayout(this);
    boxLayout->addLayout(layout);
}

void AppSystem::initSystem()
{
    QVBoxLayout *blayout = new QVBoxLayout;

    QGroupBox *group = new QGroupBox(tr("系统设置"), this);
    group->setLayout(blayout);
    layout->addWidget(group);

    QStringList names;
    names << tr("语言设置") << tr("测试模式") << tr("启动方式") << tr("亮度设定")
          << tr("音量设定") << tr("条码起始") << tr("条码长度") << tr("合格报警")
          << tr("报警提示") << tr("测试延时");

    mView = new BoxQModel(this);
    mView->setColumnCount(1);
    mView->setRowCount(names.size());
    mView->setVerticalHeaderLabels(names);
    connect(mView, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(beep()));

    view = new QTableView(this);
    view->setModel(mView);
    view->verticalHeader()->setFixedWidth(75);
    view->horizontalHeader()->setVisible(false);
    view->setEditTriggers(QAbstractItemView::AllEditTriggers);
#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    view->verticalHeader()->setResizeMode(QHeaderView::Stretch);
    view->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif
    connect(view, SIGNAL(clicked(QModelIndex)), this, SLOT(autoChange()));

    blayout->addWidget(view);
    blayout->addStretch();
    blayout->setStretch(0, 3);
    blayout->setStretch(1, 1);

    //    QVBoxLayout *boxLayout = new QVBoxLayout;
    //    group->setLayout(boxLayout);


    //    for (int i=0; i < names.size(); i++) {
    //        QHBoxLayout *box = new QHBoxLayout;
    //        boxLayout->addLayout(box);
    //        QString tmp = QString("%1").arg(i+1, 2, 10, QChar('0'));
    //        box->addWidget(new QLabel(tmp + " " + names.at(i) + ":", this));
    //        QComboBox *input = new QComboBox(this);
    //        input->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    //        input->setView(new QListView);
    //        input->setFixedHeight(35);
    //        if (i > names.indexOf(tr("音量设定")))
    //            input->setEditable(true);
    //        box->addWidget(input);
    //        texts.append(input);
    //        if (i == names.indexOf(tr("音量设定"))) {
    //            connect(input, SIGNAL(currentIndexChanged(int)), this, SLOT(beep()));
    //        }
    //        if (i == names.indexOf(tr("合格报警"))) {
    //            connect(input, SIGNAL(editTextChanged(QString)), this, SLOT(beep()));
    //        }
    //        if (i == names.indexOf(tr("报警提示"))) {
    //            connect(input, SIGNAL(editTextChanged(QString)), this, SLOT(beep()));
    //        }
    //    }
    //    boxLayout->addStretch();
}

void AppSystem::initDevice()
{
    QGroupBox *group = new QGroupBox(this);
    group->setTitle(tr("本机设置"));
    layout->addWidget(group);

    QVBoxLayout *boxLayout = new QVBoxLayout;
    group->setLayout(boxLayout);

    QStringList names;
    names << tr("自动地址") << tr("默认网关") << tr("网络地址") << tr("子网掩码")
          << tr("挂载地址") << tr("挂载路径") << tr("自动执行") << tr("执行参数");

    for (int i=0; i < names.size(); i++) {
        QHBoxLayout *box = new QHBoxLayout;
        boxLayout->addLayout(box);
        QString tmp = QString("%1").arg(i+1, 2, 10, QChar('0'));
        box->addWidget(new QLabel(tmp + " " + names.at(i) + ":", this));
        QLineEdit *input = new QLineEdit(this);
        input->setFixedHeight(35);
        box->addWidget(input);
        ctrls.append(input);
    }
    boxLayout->addStretch();
}

void AppSystem::initButton()
{
    QHBoxLayout *btn = new QHBoxLayout;
    boxLayout->addLayout(btn);
#ifdef __arm__
    btn->addWidget(new QLabel(tr("系统时间"), this));

    time = new QDateTimeEdit(this);
    time->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
    time->setFixedSize(200, 40);
    btn->addWidget(time);

    QPushButton *btnTime = new QPushButton(this);
    btnTime->setText(tr("设置"));
    btnTime->setFixedSize(97, 44);
    btn->addWidget(btnTime);
    connect(btnTime, SIGNAL(clicked(bool)), this, SLOT(setLocalTime()));
#endif
    btn->addStretch();

    QPushButton *btnSave = new QPushButton(this);
    btnSave->setText(tr("保存"));
    btnSave->setFixedSize(97, 44);
    btn->addWidget(btnSave);
    connect(btnSave, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));
}

void AppSystem::initDelegate()
{
    areas << tr("中文");
    modes << tr("常规模式");
    tests << tr("按钮启动") << tr("滑罩启动") << tr("脚踏启动") << tr("网络启动");
    view->setItemDelegateForRow(0, new BoxQItems);
    view->setItemDelegateForRow(1, new BoxQItems);
    view->setItemDelegateForRow(2, new BoxQItems);

    BoxDouble *rate = new BoxDouble;
    rate->setDecimals(0);
    rate->setMaxinum(99);
    view->setItemDelegateForRow(3, rate);
    view->setItemDelegateForRow(4, rate);
    view->setItemDelegateForRow(5, rate);
    view->setItemDelegateForRow(6, rate);

    BoxDouble *time = new BoxDouble;
    time->setDecimals(1);
    time->setMaxinum(5);
    view->setItemDelegateForRow(7, time);
    view->setItemDelegateForRow(8, time);
    view->setItemDelegateForRow(9, time);
}

void AppSystem::initSettings()
{
    isInit = false;
    int r = tmpSet.value(2000 + Qt::Key_1).toInt();
    for (int i=0; i < mView->rowCount(); i++) {
        QString str = tmpSet.value(r + i).toString();
        str = (i == 0) ? areas.at(str.toInt()%areas.size()) : str;
        str = (i == 1) ? modes.at(str.toInt()%modes.size()) : str;
        str = (i == 2) ? tests.at(str.toInt()%tests.size()) : str;
        mView->setData(mView->index(i, 0), str, Qt::DisplayRole);
    }
    int s = tmpSet[(2000 + Qt::Key_2)].toInt();
    for (int i=0; i < ctrls.size(); i++) {  // 本机设置存放在0x0030
        ctrls.at(i)->setText(tmpSet[s + i].toString());
    }
#ifdef __arm__
    time->setDateTime(QDateTime::currentDateTime());
#endif
    isInit = true;
}

void AppSystem::saveSettings()
{
    int r = tmpSet.value(2000 + Qt::Key_1).toInt();
    for (int i=0; i < mView->rowCount(); i++) {
        QString str = mView->item(i, 0)->text();
        str = (i == 0) ? QString::number(areas.indexOf(str)) : str;
        str = (i == 1) ? QString::number(modes.indexOf(str)) : str;
        str = (i == 2) ? QString::number(tests.indexOf(str)) : str;
        tmpMsg.insert(r + i, str);
    }
    int s = tmpSet.value(2000 + Qt::Key_2).toInt();
    for (int i=0; i < ctrls.size(); i++) {  // 本机设置存放在0x0030
        tmpMsg[s + i] = ctrls.at(i)->text();
    }
    tmpMsg.insert(Qt::Key_0, Qt::Key_Save);
    tmpMsg.insert(Qt::Key_1, "aip_system");
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();

    QSettings *set = new QSettings("./nandflash/userinfo.txt", QSettings::IniFormat);
    set->beginGroup("LOCAL_MACHINE");
    set->setValue("DHCP", ctrls.at(0)->text());
    set->setValue("DefaultGateway", ctrls.at(1)->text());
    set->setValue("IPAddress", ctrls.at(2)->text());
    set->setValue("SubnetMask", ctrls.at(3)->text());
    set->endGroup();
    set->beginGroup("NFS_SERVER");
    set->setValue("IPAddress", ctrls.at(4)->text());
    set->setValue("Mountpath", ctrls.at(5)->text());
    set->endGroup();
    set->beginGroup("USER_EXE");
    set->setValue("Name", ctrls.at(6)->text());
    set->setValue("Parameters", ctrls.at(7)->text());
    set->endGroup();
}

void AppSystem::setLocalTime()
{
    tmpMsg.insert(Qt::Key_0, Qt::Key_Xfer);
    tmpMsg.insert(Qt::Key_1, "system");
    tmpMsg.insert(Qt::Key_2, time->dateTime());
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
}

void AppSystem::autoChange()
{
    change();
    if (isInit) {
        int r = view->currentIndex().row();
        QString str = mView->index(r, 0).data().toString();
        str = (r == 0) ? areas.at((areas.indexOf(str) + 1) % areas.size()) : str;
        str = (r == 1) ? modes.at((modes.indexOf(str) + 1) % modes.size()) : str;
        str = (r == 2) ? tests.at((tests.indexOf(str) + 1) % tests.size()) : str;
        mView->setData(mView->index(r, 0), str, Qt::DisplayRole);
    }
}

void AppSystem::change()
{
    if (isInit) {  // 初始化完成后才发送界面修改
        tmpMsg.insert(Qt::Key_0, Qt::Key_Plus);
        tmpMsg.insert(Qt::Key_1, this->objectName());
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
    }
}

void AppSystem::beep()
{
    change();
    if (isInit) {
        double voic = mView->item(SystBeep, 0)->text().toDouble();
        double time = mView->item(SystTime, 0)->text().toDouble();
        int r = view->currentIndex().row();
        if (r > SystMode) {
            double real = mView->item(r, 0)->text().toDouble();
            mView->item(r, 0)->setText(QString::number(real));
            if (r == SystBeep || r == SystTime || r == SystWarn || r == SystWait) {
                tmpMsg.insert(Qt::Key_0, Qt::Key_Down);
                tmpMsg.insert(Qt::Key_3, voic);
                emit sendAppMsg(tmpMsg);
                tmpMsg.clear();
                QTimer::singleShot(time*1000, this, SLOT(stop()));
            }
        }
    }
}

void AppSystem::stop()
{
    tmpMsg.insert(Qt::Key_0, Qt::Key_Down);
    tmpMsg.insert(Qt::Key_3, 0);
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
}

void AppSystem::recvAppMsg(QTmpMap msg)
{
    switch (msg.value(Qt::Key_0).toInt()) {
    case Qt::Key_Copy:
        tmpSet = msg;
        break;
    case Qt::Key_Save:
        if (!this->isHidden())
            saveSettings();
        break;
    default:
        break;
    }
}

void AppSystem::showEvent(QShowEvent *e)
{
    this->setFocus();
    initSettings();
    e->accept();
}

