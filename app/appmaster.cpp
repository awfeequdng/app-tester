/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       用户管理
*******************************************************************************/
#include "appmaster.h"

AppMaster::AppMaster(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void AppMaster::initUI()
{
    initLayout();
    initViewBar();
    initLineBar();
    initButtons();
    initDelegate();
}

void AppMaster::initLayout()
{
    boxLayout = new QHBoxLayout;
    btnLayout = new QVBoxLayout;

    QHBoxLayout *tmpLayout = new QHBoxLayout;
    tmpLayout->addLayout(boxLayout);
    tmpLayout->addLayout(btnLayout);
    tmpLayout->setStretch(0, 1);

    QGroupBox *box = new QGroupBox(this);
    box->setLayout(tmpLayout);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(box);
}

void AppMaster::initViewBar()
{
    QStringList headers;
    headers << tr("编号") << tr("用户名称") << tr("用户密码") << tr("用户权限") << tr("登录时间");

    view = new QTableWidget(this);
    view->setRowCount(M_ROW);
    view->verticalHeader()->hide();
    view->setColumnCount(headers.size());
    view->setHorizontalHeaderLabels(headers);
    view->horizontalHeader()->setFixedHeight(30);
    for (int i=0; i < headers.size(); i++) {
        for (int j=0; j < M_ROW; j++) {
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setTextAlignment(Qt::AlignCenter);
            view->setItem(j, i, item);
        }
    }
#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    view->horizontalHeader()->setResizeMode(4, QHeaderView::Stretch);
    view->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
    view->verticalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    view->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    view->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif
    view->setColumnWidth(0, 50);
    view->setEditTriggers(QAbstractItemView::AllEditTriggers);
    connect(view, SIGNAL(clicked(QModelIndex)), this, SLOT(clickViewBar()));
    boxLayout->addWidget(view);
    boxLayout->setStretch(0, 1);
}

void AppMaster::initLineBar()
{
    roles << tr("管理员") << tr("技术员") << tr("操作员");

    QVBoxLayout *layout = new QVBoxLayout;
    btnLayout->addLayout(layout);

    QStringList tmp;
    tmp << tr("编号") << tr("名称") << tr("密码") << tr("权限");
    QList<QWidget*> win;

    lineNumb = new QLineEdit(this);
    lineNumb->setFixedSize(97, 40);
    win.append(lineNumb);

    lineName = new QLineEdit(this);
    lineName->setFixedSize(97, 40);
    win.append(lineName);

    linePass = new QLineEdit(this);
    linePass->setFixedSize(97, 40);
    linePass->setEchoMode(QLineEdit::Password);
    win.append(linePass);

    boxGroup = new QComboBox(this);
    boxGroup->setFixedSize(97, 40);
    boxGroup->addItems(roles);
    win.append(boxGroup);

    for (int i=0; i < tmp.size(); i++) {
        QHBoxLayout *box = new QHBoxLayout;
        box->addWidget(new QLabel(tmp.at(i), this));
        box->addWidget(win.at(i));
        layout->addLayout(box);
    }

    layout->addStretch();
}

void AppMaster::initButtons()
{
    btnLayout->addStretch();

    QPushButton *btnAdd = new QPushButton(this);
    btnAdd->setFixedHeight(40);
    btnAdd->setText(tr("添加"));
    btnLayout->addWidget(btnAdd);
    connect(btnAdd, SIGNAL(clicked(bool)), this, SLOT(appendMaster()));

    QPushButton *btnDel = new QPushButton(this);
    btnDel->setFixedHeight(40);
    btnDel->setText(tr("删除"));
    btnLayout->addWidget(btnDel);
    connect(btnDel, SIGNAL(clicked(bool)), this, SLOT(removeMaster()));

    QPushButton *save = new QPushButton(this);
    save->setFixedHeight(40);
    save->setText(tr("保存"));
    btnLayout->addWidget(save);
    connect(save, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));
}

void AppMaster::initDelegate()
{
    BoxQCombo *group = new BoxQCombo;
    group->setItemNames(roles);

    view->setItemDelegateForColumn(0, new BoxQItems);
    view->setItemDelegateForColumn(1, new BoxQItems);
    view->setItemDelegateForColumn(2, new BoxQItems);
    view->setItemDelegateForColumn(3, new BoxQItems);
    view->setItemDelegateForColumn(4, new BoxQItems);
}

void AppMaster::initSettings()
{
    int r = tmpSet[AddrUser].toInt() + 5;  // 用户存放地址
    for (int i=0; i < M_ROW; i++) {
        int s = r + i*5;
        QString name = tmpSet[s + AddrName].toString();
        if (!name.isEmpty()) {
            QString role = roles.at(tmpSet[s + AddrRole].toInt()-1);
            QString time = tmpSet[s + AddrLast].toString();
            view->item(i, 1)->setText(name);
            view->item(i, 2)->setText("*");
            view->item(i, 3)->setText(role);
            view->item(i, 4)->setText(time);
        } else {
            view->item(i, 1)->setText("");
            view->item(i, 2)->setText("");
            view->item(i, 3)->setText("");
            view->item(i, 4)->setText("");
        }
        view->item(i, 0)->setText(QString("%1").arg(i+1, 2, 10, QChar('0')));
    }
}

void AppMaster::saveSettings()
{
    tmpSet.insert(AddrEnum, Qt::Key_Save);
    emit sendAppMsg(tmpSet);
}

void AppMaster::clickViewBar()
{
    int row = view->currentRow();
    if (row < 0)
        return;
    lineNumb->setText(view->item(row, 0)->text());
    lineName->setText(view->item(row, 1)->text());
    linePass->setText(view->item(row, 2)->text());

    int t = roles.indexOf(view->item(row, 3)->text());
    if (t >= 0)
        boxGroup->setCurrentIndex(t);
}

void AppMaster::appendMaster()
{
    QString t_numb = lineNumb->text();
    QString t_name = lineName->text();
    QString t_pass = linePass->text();
    QString t_time = QDateTime::currentDateTime().toString("yy-MM-dd hh:mm:ss");
    if (t_name.isEmpty())
        return;
    int r = tmpSet[AddrUser].toInt();
    int s = r + t_numb.toInt() * 5;
    tmpSet[s + AddrName] = t_name;
    tmpSet[s + AddrPass] = t_pass;
    tmpSet[s + AddrRole] = boxGroup->currentIndex() + 1;
    tmpSet[s + AddrLast] = t_time;
    initSettings();
}

void AppMaster::removeMaster()
{
    int row = view->currentRow();
    if (row < 0)
        return;
    int p = view->item(row, 0)->text().toInt();
    int s = tmpSet[AddrUser].toInt() + p*5;
    if (s == tmpSet[DataUser].toInt())
        QMessageBox::warning(this, tr("警告"), tr("不能删除当前用户"), QMessageBox::Ok);
    else {
        tmpSet[s + AddrName] = "";
        tmpSet[s + AddrPass] = "";
        tmpSet[s + AddrRole] = "";
        tmpSet[s + AddrLast] = "";
        tmpSet[s + AddrSave] = "";
        initSettings();
    }
}

void AppMaster::recvAppMsg(QTmpMap msg)
{
    int c = msg.value(0).toInt();
    switch (c) {
    case Qt::Key_Copy:
        tmpSet = msg;
        break;
    default:
        break;
    }
}

void AppMaster::showEvent(QShowEvent *e)
{
    this->setFocus();
    initSettings();
    e->accept();
}

