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
    initView();
    initLayout();
    initDelegate();
}

void AppMaster::initView()
{
    QStringList headers;
    headers << tr("用户名称") << tr("用户密码") << tr("用户组别") << tr("登录时间");

    view = new QTableWidget(this);
    view->setRowCount(M_ROW);
    view->verticalHeader()->hide();
    view->setColumnCount(headers.size());
    view->setHorizontalHeaderLabels(headers);
    for (int i=0; i < headers.size(); i++) {
        for (int j=0; j < M_ROW; j++) {
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setTextAlignment(Qt::AlignCenter);
            view->setItem(j, i, item);
        }
    }
    connect(view, SIGNAL(cellChanged(int, int)), this, SLOT(autoInput(int, int)));
#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    view->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    view->verticalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif
}

void AppMaster::initLayout()
{
    QHBoxLayout *btnLayout = new QHBoxLayout;

    QPushButton *prev = new QPushButton(this);
    prev->setFixedSize(97, 40);
    prev->setText(tr("上一页"));
    btnLayout->addWidget(prev);
    connect(prev, SIGNAL(clicked(bool)), this, SLOT(clickButtons()));

    page = new QLineEdit(this);
    page->setText("1");
    page->setFixedSize(50, 36);
    page->setAlignment(Qt::AlignCenter);
    btnLayout->addWidget(page);

    QPushButton *next = new QPushButton(this);
    next->setFixedSize(97, 40);
    next->setText(tr("下一页"));
    btnLayout->addWidget(next);
    connect(next, SIGNAL(clicked(bool)), this, SLOT(clickButtons()));

    btnLayout->addStretch();

    QPushButton *save = new QPushButton(this);
    save->setFixedSize(97, 40);
    save->setText(tr("保存"));
    btnLayout->addWidget(save);
    connect(save, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));

    QVBoxLayout *boxLayout = new QVBoxLayout;
    boxLayout->addWidget(view);
    boxLayout->addLayout(btnLayout);

    QGroupBox *box = new QGroupBox(this);
    box->setLayout(boxLayout);


    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(box);
}

void AppMaster::initDelegate()
{
    groups << tr("管理员") << tr("技术员") << tr("操作员");

    BoxQCombo *group = new BoxQCombo;
    group->setItemNames(groups);

    view->setItemDelegateForColumn(2, group);
    view->setItemDelegateForColumn(3, new BoxQItems);
}

void AppMaster::initSettings()
{
    int p = page->text().toInt() - 1;           // 页码
    int s = ADDR_MASTER + M_ROW*M_COL*p + 4;    // 起始地址,隐藏超级用户
    for (int i=0; i < M_ROW; i++) {
        int t = s + M_COL*i;
        QString username = config[QString::number(t+0)].toString();
        if (!username.isEmpty()) {
            QString usrgroup = groups.at(config[QString::number(t+2)].toInt()-1);
            view->item(i, 0)->setText(config[QString::number(t+0)].toString());
            view->item(i, 1)->setText("*");
            view->item(i, 2)->setText(usrgroup);
            view->item(i, 3)->setText(config[QString::number(t+3)].toString());
        } else {
            view->item(i, 0)->setText("");
            view->item(i, 1)->setText("");
            view->item(i, 2)->setText("");
            view->item(i, 3)->setText("");
        }
    }
}

void AppMaster::saveSettings()
{
    int p = page->text().toInt() - 1;           // 页码
    int s = ADDR_MASTER + M_ROW*M_COL*p + 4;    // 起始地址,隐藏超级用户
    for (int i=0; i < M_ROW; i++) {
        int t = s + M_COL*i;
        QString username = view->item(i, 0)->text();
        QString usrgroup = view->item(i, 2)->text();
        if (!username.isEmpty() && !usrgroup.isEmpty()) {
            config[QString::number(t + 0)] = username;
            config[QString::number(t + 2)] = QString::number(groups.indexOf(usrgroup) + 1);
        }
    }
    config.insert("enum", Qt::Key_Save);
    emit sendAppMap(config);
}

void AppMaster::clickButtons()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    int p = page->text().toInt();
    if (btn->text() == tr("上一页")) {
        p--;
    }
    if (btn->text() == tr("下一页")) {
        p++;
    }
    p = (p >= 1) ? p : 1;
    p = (p <= 4) ? p : 4;

    saveSettings();
    page->setText(QString::number(p));
    initSettings();
}

void AppMaster::autoInput(int r, int c)
{
    if (c != 1)
        return;
    if (view->item(r, 0)->text().isEmpty())
        return;
    QString pass = view->item(r, c)->text();
    if (pass == "*")
        return;
    int p = page->text().toInt() - 1;           // 页码
    int s = ADDR_MASTER + M_ROW*M_COL*p + 4 + r*4 + 1;    // 地址
    config[QString::number(s)] = pass;
    view->item(r, c)->setText("*");
}

void AppMaster::recvAppMap(QVariantMap msg)
{
    switch (msg.value("enum").toInt()) {
    case Qt::Key_Option:
        for (int i=ADDR_MASTER; i < ADDR_MASTER+0x0100; i++) {  // 用户信息存放在0x0100
            config[QString::number(i)] = msg[QString::number(i)];
        }
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

