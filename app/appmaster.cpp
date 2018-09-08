/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       用户管理
*******************************************************************************/
#include "appmaster.h"

const int maxRow = 15;

AppMaster::AppMaster(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void AppMaster::initUI()
{
    initLayout();
    initViewBar();
    initLineBar();
    initDelegate();
}

void AppMaster::initLayout()
{
    QVBoxLayout *mlayout = new QVBoxLayout(this);
    QGroupBox *box = new QGroupBox(this);
    mlayout->addWidget(box);
    layout = new QVBoxLayout;
    box->setLayout(layout);
}

void AppMaster::initViewBar()
{
    QStringList headers;
    headers << tr("用户名称") << tr("用户密码")
            << tr("用户权限") << tr("末次登录时间");

    view = new QTableWidget(this);
    view->setColumnCount(headers.size());
    view->setHorizontalHeaderLabels(headers);
    view->verticalHeader()->setFixedWidth(26);
    view->horizontalHeader()->setFixedHeight(30);
#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    view->verticalHeader()->setResizeMode(QHeaderView::Stretch);
    view->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif
    connect(view, SIGNAL(clicked(QModelIndex)), this, SLOT(clickViewBar()));
    view->setRowCount(maxRow);
    for (int col=0; col < headers.size(); col++) {
        for (int row=0; row < maxRow; row++) {
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setTextAlignment(Qt::AlignCenter);
            view->setItem(row, col, item);
        }
    }
    layout->addWidget(view);
}

void AppMaster::initLineBar()
{
    gRole << tr("管理员") << tr("技术员") << tr("操作员");

    QHBoxLayout *blayout = new QHBoxLayout;
    layout->addLayout(blayout);

    blayout->addWidget(new QLabel(tr("名称"), this));
    iName = new QLineEdit(this);
    iName->setFixedSize(97, 40);
    blayout->addWidget(iName);

    blayout->addWidget(new QLabel(tr("密码"), this));
    iPass = new QLineEdit(this);
    iPass->setFixedSize(97, 40);
    iPass->setEchoMode(QLineEdit::Password);
    blayout->addWidget(iPass);

    blayout->addWidget(new QLabel(tr("权限"), this));
    iRole = new QComboBox(this);
    iRole->addItems(gRole);
    iRole->setFixedSize(97, 40);
    iRole->setView(new QListView);
    blayout->addWidget(iRole);

    blayout->addStretch();
    QPushButton *btnSave = new QPushButton(this);
    btnSave->setFixedSize(97, 44);
    btnSave->setText(tr("保存"));
    blayout->addWidget(btnSave);
    connect(btnSave, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));
}

void AppMaster::initDelegate()
{
    view->setItemDelegate(new BoxQItems);
}

void AppMaster::initSettings()
{
    int user = tmpSet.value(2000 + Qt::Key_5).toInt();  // 用户存放地址
    for (int i=0; i < maxRow; i++) {
        int addr = user + (i + 1) * 5;  // 隐藏超级用户
        int numb = (tmpSet.value(addr + mRole).toInt() - 1) % gRole.size();
        QString name = tmpSet.value(addr + mName).toString();
        if (!name.isEmpty()) {
            QString role = gRole.at(numb);
            QString time = tmpSet.value(addr + mLast).toString();
            view->item(i, mName)->setText(name);
            view->item(i, mPass)->setText("*");
            view->item(i, mRole)->setText(role);
            view->item(i, mLast)->setText(time);
        } else {
            view->item(i, mName)->setText("");
            view->item(i, mPass)->setText("");
            view->item(i, mRole)->setText("");
            view->item(i, mLast)->setText("");
        }
    }
}

void AppMaster::saveSettings()
{
    int row = view->currentRow();
    if (row >= 0) {
        int addr = tmpSet.value(2000 + Qt::Key_5).toInt() + (row + 1) * 5;  // 用户存放地址
        tmpMsg.insert(addr + mRole, iRole->currentIndex() + 1);
        tmpMsg.insert(addr + mName, iName->text());
        tmpMsg.insert(addr + mPass, iPass->text());
        tmpMsg.insert(Qt::Key_0, Qt::Key_Save);
        tmpMsg.insert(Qt::Key_1, "aip_system");
        qDebug() << tmpMsg;
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
        initSettings();
    }
}

void AppMaster::clickViewBar()
{
    int row = view->currentRow();
    iName->setText(view->item(row, mName)->text());
    iPass->setText(view->item(row, mPass)->text());
    iRole->setCurrentIndex(qMax(0, gRole.indexOf(view->item(row, mRole)->text())));
}

void AppMaster::recvAppMsg(QTmpMap msg)
{
    switch (msg.value(Qt::Key_0).toInt()) {
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

