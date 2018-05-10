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
    boxLayout = new QVBoxLayout;

    QGroupBox *box = new QGroupBox(this);
    box->setLayout(boxLayout);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(box);
}

void AppMaster::initViewBar()
{
    QStringList headers;
    headers << tr("编号") << tr("用户名称") << tr("用户密码") << tr("用户组别");

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
    view->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    view->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
    view->verticalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif
    view->setColumnWidth(0, 50);
    view->setEditTriggers(QAbstractItemView::AllEditTriggers);
    connect(view, SIGNAL(clicked(QModelIndex)), this, SLOT(clickViewBar()));
    boxLayout->addWidget(view);
}

void AppMaster::initLineBar()
{
    groups << tr("管理员") << tr("技术员") << tr("操作员");

    QHBoxLayout *layout = new QHBoxLayout;
    boxLayout->addLayout(layout);

    layout->addWidget(new QLabel(tr("编号"), this));
    lineNumb = new QLineEdit(this);
    lineNumb->setFixedSize(64, 40);
    layout->addWidget(lineNumb);

    layout->addWidget(new QLabel(tr("名称"), this));
    lineName = new QLineEdit(this);
    lineName->setFixedSize(97, 40);
    layout->addWidget(lineName);

    layout->addWidget(new QLabel(tr("密码"), this));
    linePass = new QLineEdit(this);
    linePass->setFixedSize(97, 40);
    linePass->setEchoMode(QLineEdit::Password);
    layout->addWidget(linePass);

    layout->addWidget(new QLabel(tr("组别"), this));
    boxGroup = new QComboBox(this);
    boxGroup->setFixedSize(97, 40);
    boxGroup->addItems(groups);
    layout->addWidget(boxGroup);

    layout->addStretch();
}

void AppMaster::initButtons()
{
    QHBoxLayout *layout = new QHBoxLayout;
    boxLayout->addLayout(layout);

    QPushButton *prev = new QPushButton(this);
    prev->setFixedSize(97, 40);
    prev->setText(tr("上一页"));
    layout->addWidget(prev);
    connect(prev, SIGNAL(clicked(bool)), this, SLOT(clickButtons()));

    page = new QLineEdit(this);
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

    QPushButton *btnAdd = new QPushButton(this);
    btnAdd->setFixedSize(97, 40);
    btnAdd->setText(tr("添加"));
    layout->addWidget(btnAdd);
    connect(btnAdd, SIGNAL(clicked(bool)), this, SLOT(appendMaster()));

    QPushButton *btnDel = new QPushButton(this);
    btnDel->setFixedSize(97, 40);
    btnDel->setText(tr("删除"));
    layout->addWidget(btnDel);
    connect(btnDel, SIGNAL(clicked(bool)), this, SLOT(removeMaster()));

    QPushButton *save = new QPushButton(this);
    save->setFixedSize(97, 40);
    save->setText(tr("保存"));
    layout->addWidget(save);
    connect(save, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));
}

void AppMaster::initDelegate()
{
    BoxQCombo *group = new BoxQCombo;
    group->setItemNames(groups);

    view->setItemDelegateForColumn(0, new BoxQItems);
    view->setItemDelegateForColumn(1, new BoxQItems);
    view->setItemDelegateForColumn(2, new BoxQItems);
    view->setItemDelegateForColumn(3, group);
}

void AppMaster::initSettings()
{
    int p = page->text().toInt() - 1;           // 页码
    int s = AddrMaster + M_ROW*M_COL*p + 4;    // 起始地址,隐藏超级用户
    for (int i=0; i < M_ROW; i++) {
        int t = s + M_COL*i;
        view->item(i, 0)->setText(QString("%1").arg(p*M_ROW+i+1, 2, 10, QChar('0')));
        QString username = config[QString::number(t+0)].toString();
        if (!username.isEmpty()) {
            QString usrgroup = groups.at(config[QString::number(t+2)].toInt()-1);
            view->item(i, 1)->setText(username);
            view->item(i, 2)->setText("*");
            view->item(i, 3)->setText(usrgroup);
        } else {
            view->item(i, 1)->setText("");
            view->item(i, 2)->setText("");
            view->item(i, 3)->setText("");
        }
    }
}

void AppMaster::saveSettings()
{
    int p = page->text().toInt() - 1;           // 页码
    int s = AddrMaster + M_ROW*M_COL*p + 4;    // 起始地址,隐藏超级用户
    for (int i=0; i < M_ROW; i++) {
        int t = s + M_COL*i;
        QString username = view->item(i, 1)->text();
        QString usrgroup = view->item(i, 3)->text();
        if (!username.isEmpty() && !usrgroup.isEmpty()) {
            config[QString::number(t + 0)] = username;
            config[QString::number(t + 2)] = QString::number(groups.indexOf(usrgroup) + 1);
        }
    }
    config.insert("enum", Qt::Key_Save);
    emit sendAppMap(config);
}

void AppMaster::clickViewBar()
{
    int row = view->currentRow();
    if (row < 0)
        return;
    int p = page->text().toInt() - 1;           // 页码
    int s = AddrMaster + M_ROW*M_COL*p + 4 + row*4;    // 地址
    lineNumb->setText(view->item(row, 0)->text());
    lineName->setText(config[QString::number(s + 0)].toString());
    linePass->setText(config[QString::number(s + 1)].toString());
    int t = config[QString::number(s + 2)].toInt() - 1;
    if (t >= 0)
        boxGroup->setCurrentIndex(t);
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

    page->setText(QString::number(p));
    initSettings();
}

void AppMaster::appendMaster()
{
    QString t_numb = lineNumb->text();
    QString t_name = lineName->text();
    QString t_pass = linePass->text();
    QString tGroup = QString::number(boxGroup->currentIndex() + 1);
    QString t_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    if (t_name.isEmpty())
        return;
    if (t_numb.isEmpty()) {
        for (int i=AddrMaster; i < AddrMaster+0x0100; i+=4) {
            if (config[QString::number(i)].toString().isEmpty()) {
                t_numb = QString::number((i - AddrMaster)/4);
                break;
            }
        }
    }
    for (int i=AddrMaster; i < AddrMaster+0x0100; i+=4) {
        if (config[QString::number(i)].toString() == t_name) {
            QString tmp = tr("用户已存在");
            QMessageBox::warning(this, tr("警告"), tmp, QMessageBox::Ok);
            return;
        }
    }
    t_numb = QString::number(AddrMaster + t_numb.toInt()*4);
    if (!config[t_numb].toString().isEmpty()) {
        QString tmp = tr("此编号已使用");
        QMessageBox::warning(this, tr("警告"), tmp, QMessageBox::Ok);
        return;
    }
    config[QString::number(t_numb.toInt() + 0)] = t_name;
    config[QString::number(t_numb.toInt() + 1)] = t_pass;
    config[QString::number(t_numb.toInt() + 2)] = tGroup;
    config[QString::number(t_numb.toInt() + 3)] = t_time;
    initSettings();
}

void AppMaster::removeMaster()
{
    int row = view->currentRow();
    if (row < 0)
        return;
    int p = view->item(row, 0)->text().toInt();
    int s = AddrMaster + p*4;

    QString t_name = view->item(row, 1)->text();
    QString c_name = config[QString::number(AddrSignIn)].toString();
    if (t_name == c_name) {
        QMessageBox::warning(this, tr("警告"), tr("不能删除当前用户"), QMessageBox::Ok);
        return;
    }

    config[QString::number(s + 0)] = "";
    config[QString::number(s + 1)] = "";
    config[QString::number(s + 2)] = "";
    config[QString::number(s + 3)] = "";
    initSettings();
}

void AppMaster::recvAppMap(QVariantMap msg)
{
    switch (msg.value("enum").toInt()) {
    case Qt::Key_Copy:
        for (int i=AddrMaster; i < AddrMaster+0x0100; i++) {  // 用户信息存放在0x0100
            config[QString::number(i)] = msg[QString::number(i)];
        }
        config[QString::number(AddrSignIn)] = msg[QString::number(AddrSignIn)];  // 当前用户
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

