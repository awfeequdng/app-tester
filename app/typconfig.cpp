/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       型号管理界面
*******************************************************************************/
#include "typconfig.h"

TypConfig::TypConfig(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void TypConfig::initUI()
{
    initLayout();
    initViewBar();
    initConfigBar();
    initButtonBar();
}

void TypConfig::initLayout()
{
    boxLayout = new QVBoxLayout;

    splitter = new QSplitter(this);
    boxLayout->addWidget(splitter);

    QGroupBox *box = new QGroupBox(this);
    box->setLayout(boxLayout);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(3, 0, 3, 0);
    layout->addWidget(box);
}

void TypConfig::initViewBar()
{
    QStringList headers;
    headers << tr("编号") << tr("型号名称");

    view = new QTableWidget(this);
    view->setRowCount(C_ROW);
    view->setColumnCount(headers.size());
    view->setHorizontalHeaderLabels(headers);
    view->verticalHeader()->hide();
    for (int i=0; i < headers.size(); i++) {
        for (int j=0; j < C_ROW; j++) {
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setText("");
            item->setTextAlignment(Qt::AlignCenter);
            view->setItem(j, i, item);
        }
    }
#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    view->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
    view->verticalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    view->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif
    view->setColumnWidth(0, 50);
    splitter->addWidget(view);
    splitter->setStretchFactor(0, 5);
    view->setItemDelegate(new BoxQItems);
    connect(view, SIGNAL(clicked(QModelIndex)), this, SLOT(clickViewBar()));
}

void TypConfig::initConfigBar()
{
    isShow = Qt::Key_Equal;
    setFrame = new QFrame(this);
    splitter->addWidget(setFrame);
    splitter->setStretchFactor(1, 1);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    setFrame->setLayout(layout);


    QStringList headers;
    headers << tr("型号编号") << tr("型号名称");

    names << tr("当前型号") << tr("电枢片数") << tr("夹具类型")
          << tr("选中编号") << tr("选中型号");

    settings = new QTableWidget(this);
    settings->setRowCount(names.size());
    settings->setColumnCount(headers.size());
    settings->setHorizontalHeaderLabels(headers);
    settings->verticalHeader()->hide();
    settings->horizontalHeader()->hide();
    for (int i=0; i < headers.size(); i++) {
        for (int j=0; j < names.size(); j++) {
            QTableWidgetItem *item = new QTableWidgetItem;
            if (i == 0)
                item->setText(names.at(j));
            else
                item->setText("");
            item->setTextAlignment(Qt::AlignCenter);
            settings->setItem(j, i, item);
        }
    }
#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    settings->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
    settings->verticalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    settings->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    settings->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif
    settings->setColumnWidth(0, 70);
    settings->setItemDelegateForColumn(0, new BoxQItems);
    settings->setEditTriggers(QAbstractItemView::AllEditTriggers);
    layout->addWidget(settings);

    QHBoxLayout *box2 = new QHBoxLayout;
    layout->addLayout(box2);
    box2->setMargin(0);
    box2->addStretch();

    QPushButton *btnAdd = new QPushButton(this);
    btnAdd->setFixedSize(97, 40);
    btnAdd->setText(tr("添加"));
    box2->addWidget(btnAdd);
    connect(btnAdd, SIGNAL(clicked(bool)), this, SLOT(appendModelType()));

    QPushButton *btnDel = new QPushButton(this);
    btnDel->setFixedSize(97, 40);
    btnDel->setText(tr("删除"));
    box2->addWidget(btnDel);
    connect(btnDel, SIGNAL(clicked(bool)), this, SLOT(deleteModelType()));

    layout->addStretch();
}

void TypConfig::initButtonBar()
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

    QPushButton *btnGet = new QPushButton(this);
    btnGet->setFixedSize(97, 40);
    btnGet->setText(tr("调入"));
    btnLayout->addWidget(btnGet);

    QPushButton *btnSave = new QPushButton(this);
    btnSave->setFixedSize(97, 40);
    btnSave->setText(tr("保存"));
    btnLayout->addWidget(btnSave);
    connect(btnSave, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));

    boxLayout->addLayout(btnLayout);
}

void TypConfig::initSettings()
{
    int p = page->text().toInt() - 1;    // 页码
    int s = AddrModels + C_ROW*p;       // 起始地址
    for (int i=0; i < C_ROW; i++) {
        int t = s + i;
        view->item(i, 0)->setText(QString("%1").arg(t-AddrModels+1, 3, 10, QChar('0')));
        view->item(i, 1)->setText(config[QString::number(t)].toString());
    }
    config[QString::number(AddrSC + AddrTM)] = config[QString::number(AddrConfig)];
    for (int i=0; i < 3; i++) {
        settings->item(i, 1)->setText(config[QString::number(AddrSC + i)].toString());
    }

    if (isShow == Qt::Key_Less)
        setFrame->show();
    else
        setFrame->hide();
}

void TypConfig::saveSettings()
{
    for (int i=0; i < 3; i++) {
        tmpMap[QString::number(AddrSC + i)] = settings->item(i, 1)->text();
    }
    tmpMap.insert("enum", Qt::Key_Option);
    emit sendAppMap(tmpMap);
    tmpMap.clear();
}

void TypConfig::appendModelType()
{
    int t = names.indexOf(tr("选中编号"));
    QString t_numb = settings->item(t+0, 1)->text();
    QString t_name = settings->item(t+1, 1)->text();
    if (t_name.isEmpty())
        return;
    if (t_numb.isEmpty()) {
        for (int i=AddrModels; i < AddrModels+0x0100; i++) {
            if (config[QString::number(i)].toString().isEmpty()) {
                t_numb = QString::number(i - AddrModels + 1);
                break;
            }
        }
    }
    for (int i=AddrModels; i < AddrModels+0x0100; i++) {
        if (config[QString::number(i)].toString() == t_name) {
            QString tmp = tr("发现编号%1与此型号名称相同").arg(i-AddrModels+1, 3, 10, QChar('0'));
            QMessageBox::warning(this, tr("警告"), tmp, QMessageBox::Ok);
            return;
        }
    }
    t_numb = QString::number(AddrModels + (t_numb.toInt() - 1)%0x0100);
    if (!config[t_numb].toString().isEmpty()) {
        QString tmp = tr("此编号已使用");
        QMessageBox::warning(this, tr("警告"), tmp, QMessageBox::Ok);
        return;
    }

    QString name = "sqlite";
    QString c_name = config[QString::number(AddrConfig)].toString();
    QSqlQuery query(QSqlDatabase::database(name));
    QSqlDatabase::database(name).transaction();
    query.exec(tr("create table M_%1 as select * from M_%2").arg(t_name).arg(c_name));
    QSqlDatabase::database(name).commit();

    config[t_numb] = t_name;
    config.insert("enum", Qt::Key_Save);
    emit sendAppMap(config);

    initSettings();
}

void TypConfig::selectModelType()
{
    int row = view->currentRow();
    if (row < 0)
        return;
    QString t_name = view->item(row, 1)->text();
    if (t_name.isEmpty())
        return;

    config.insert("enum", Qt::Key_Reload);
    config[QString::number(AddrConfig)] = t_name;
    emit sendAppMap(config);

    initSettings();
}

void TypConfig::deleteModelType()
{
    int t = names.indexOf(tr("选中编号"));
    QString t_numb = settings->item(t+0, 1)->text();
    QString t_name = settings->item(t+1, 1)->text();
    if (t_numb.isEmpty())
        return;
    if (t_name.isEmpty())
        return;

    QString c_name = config[QString::number(AddrConfig)].toString();
    if (t_name == c_name) {
        QMessageBox::warning(this, tr("警告"), tr("不能删除当前型号"), QMessageBox::Ok);
        return;
    }

    QSqlQuery query(QSqlDatabase::database("sqlite"));
    query.exec(tr("drop table M_%1").arg(t_name));

    t_numb = QString::number(AddrModels + (t_numb.toInt() - 1)%0x0100);
    config[t_numb] = "";
    config.insert("enum", Qt::Key_Save);
    emit sendAppMap(config);

    initSettings();
}

void TypConfig::clickButtons()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    int p = page->text().toInt();
    if (btn->text() == tr("上一页")) {
        p--;
    }
    if (btn->text() == tr("下一页")) {
        p++;
    }
    p = (p >= 0x01) ? p : 0x01;
    p = (p <= 0x10) ? p : 0x10;

    page->setText(QString::number(p));
    initSettings();
}

void TypConfig::clickViewBar()
{
    int row = view->currentRow();
    if (row < 0)
        return;
    int p = page->text().toInt() - 1;    // 页码
    int s = AddrModels + C_ROW*p + row; // 地址
    int t = names.indexOf(tr("选中编号"));
    settings->item(t+0, 1)->setText(QString("%1").arg(s-AddrModels+1, 3, 10, QChar('0')));
    settings->item(t+1, 1)->setText(config[QString::number(s)].toString());
}

void TypConfig::recvAppMap(QVariantMap msg)
{
    switch (msg.value("enum").toInt()) {
    case Qt::Key_Option:
        config[QString::number(AddrConfig)] = msg[QString::number(AddrConfig)];  // 当前型号
        for (int i=AddrModels; i < AddrModels+0x0100; i++) {
            config[QString::number(i)] = msg[QString::number(i)];
        }
        for (int i=AddrSC; i < AddrSC + 0x10; i++) {  // 综合设置
            config[QString::number(i)] = msg[QString::number(i)];
        }
        break;
    case Qt::Key_Less:
    case Qt::Key_Equal:
        isShow = msg.value("enum").toInt();
        break;
    default:
        break;
    }
}

void TypConfig::showEvent(QShowEvent *e)
{
    this->setFocus();
    initSettings();
    e->accept();
}

