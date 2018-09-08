/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       型号管理界面
*******************************************************************************/
#include "typconfig.h"

const int maxRow = 15;

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
    initDelegate();
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
    connect(view, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(selectModel()));
}

void TypConfig::initConfigBar()
{
    isShow = Qt::Key_Meta;
    setFrame = new QFrame(this);
    splitter->addWidget(setFrame);
    splitter->setStretchFactor(1, 1);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    setFrame->setLayout(layout);

    names << tr("电枢片数") << tr("夹具倍数");

    settings = new QTableWidget(this);
    settings->setRowCount(names.size());
    settings->setColumnCount(1);
    settings->setVerticalHeaderLabels(names);
    settings->horizontalHeader()->setVisible(false);
    for (int i=0; i < names.size(); i++) {
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setTextAlignment(Qt::AlignCenter);
        item->setText(names.at(i));
        settings->setItem(i, 0, item);
    }
#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    settings->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    settings->verticalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    settings->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    settings->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif
    settings->setFixedHeight(100);
    settings->setEditTriggers(QAbstractItemView::AllEditTriggers);
    layout->addWidget(settings);

    layout->addStretch();

    QHBoxLayout *nameLayout = new QHBoxLayout;
    nameLayout->addWidget(new QLabel(tr("型号:"), this));
    name = new QLineEdit(this);
    name->setFixedSize(125, 35);
    nameLayout->addWidget(name);
    layout->addLayout(nameLayout);
    connect(name, SIGNAL(textChanged(QString)), this, SLOT(change()));
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

    btnLayout->addWidget(new QLabel(tr("当前:")));
    type = new QLabel(this);
    type->setFixedHeight(40);
    btnLayout->addWidget(type);

    btnLayout->addStretch();

    btnSave = new QPushButton(this);
    btnSave->setFixedSize(97, 40);
    btnSave->setText(tr("保存"));
    btnLayout->addWidget(btnSave);
    connect(btnSave, SIGNAL(clicked(bool)), this, SLOT(clickSave()));

    boxLayout->addLayout(btnLayout);
}

void TypConfig::initDelegate()
{
    BoxDouble *t = new BoxDouble;
    t->setDecimals(0);
    t->setMininum(2);
    t->setMaxinum(72);
    settings->setItemDelegateForRow(0, t);
    settings->setItemDelegateForRow(1, new BoxQItems);
}

void TypConfig::initSettings()
{
    QSqlQuery query(QSqlDatabase::database("config"));
    query.exec("select name from sqlite_master where type='table' order by name");
    tmpTyp.clear();
    while (query.next()) {
        QString t = query.value(0).toString();
        QString numb = t.mid(1, 4);
        tmpTyp.insert(numb.toInt(), t.mid(6, NAME_SIZE));
    }
    int p = page->text().toInt() - 1;    // 页码
    int s = maxRow*p;       // 起始地址
    for (int i=0; i < maxRow; i++) {
        int t = s + i;
        view->item(i, 0)->setText(QString("%1").arg(t+1, 4, 10, QChar('0')));
        view->item(i, 1)->setText(tmpTyp.value(t+1).toString());
    }

    type->setText(QString("%1").arg(tmpSet.value(DataType).toString()));

    int r = tmpSet[(4000 + Qt::Key_0)].toInt();
    for (int i=0; i < 2; i++) {
        settings->item(i, 0)->setText(tmpSet[r + i].toString());
    }
    isRemove = false;
    isInit = true;
}

void TypConfig::saveSettings()
{
    int r = tmpSet[(4000 + Qt::Key_0)].toInt();
    for (int i=0; i < 2; i++) {
        tmpMsg[r + i] = settings->item(i, 0)->text();
    }
    tmpMsg.insert(Qt::Key_0, Qt::Key_Save);
    tmpMsg.insert(Qt::Key_1, "aip_config");
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
}

void TypConfig::createModel()
{
    int row = view->currentRow();
    QString t_numb = view->item(row, 0)->text();
    QString t_name = name->text();
    if (t_name.isEmpty())
        return;
    if (t_numb.isEmpty()) {
        return;
    }
    for (int i=0; i < tmpTyp.keys().size(); i++) {
        QString name = tmpTyp.value(tmpTyp.keys().at(i)).toString();
        if (t_name == name) {
            QMessageBox::warning(this, tr("警告"), tr("型号已存在"), QMessageBox::Ok);
            return;
        }
    }
    QString sqlName = "config";
    QString c_numb = tr("%1").arg(tmpSet.value(DataFile).toInt(), 4, 10, QChar('0'));
    QString c_name = tmpSet.value(DataType).toString();
    c_name = tr("T%1_%2").arg(c_numb).arg(c_name);
    t_name = tr("T%1_%2").arg(t_numb).arg(t_name);
    QSqlQuery query(QSqlDatabase::database(sqlName));
    QSqlDatabase::database(sqlName).transaction();
    QString cmd = tr("create table if not exists '%1' (").arg(t_name);
    cmd += "uuid integer primary key, parm text)";
    if (!query.exec(cmd)) {
        qWarning() << t_name << query.lastError();
    }
    if (!query.exec(tr("insert into '%1' select * from '%2'").arg(t_name).arg(c_name)))
        qWarning() << "sql error:" << query.lastError() << t_name << c_name;
    QSqlDatabase::database(sqlName).commit();

    initSettings();
}

void TypConfig::selectModel()
{
    int row = view->currentRow();
    if (row < 0)
        return;
    QString t_numb = view->item(row, 0)->text();
    QString t_name = view->item(row, 1)->text();
    if (t_name.isEmpty())
        return;

    tmpMsg.insert(DataFile, t_numb);
    tmpMsg.insert(DataType, t_name);
    tmpMsg.insert(Qt::Key_0, Qt::Key_Save);
    tmpMsg.insert(Qt::Key_1, "aip_reload");
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();

    initSettings();
}

void TypConfig::removeModel()
{
    int row = view->currentRow();
    if (row < 0)
        return;
    QString t_numb = view->item(row, 0)->text();
    QString c_name = view->item(row, 1)->text();
    QString t_name = name->text();
    if (c_name == type->text()) {
        QMessageBox::warning(this, tr("警告"), tr("不能修改当前型号"), QMessageBox::Ok);
        btnSave->setEnabled(true);
        return;
    }
    c_name = tr("T%1_%2").arg(t_numb).arg(c_name);
    QString sqlName = "config";
    QSqlQuery query(QSqlDatabase::database(sqlName));
    QSqlDatabase::database(sqlName).transaction();
    if (!isRemove) {
        t_name = tr("T%1_%2").arg(t_numb).arg(t_name);
        QString cmd = tr("create table if not exists '%1' (").arg(t_name);
        cmd += "uuid integer primary key, parm text)";
        if (!query.exec(cmd)) {
            qWarning() << t_name << query.lastError();
        }
        if (!query.exec(tr("insert into '%1' select * from '%2'").arg(t_name).arg(c_name)))
            qWarning() << "sql error:" << query.lastError() << t_name << c_name;
    }
    query.exec(tr("drop table '%1'").arg(c_name));
    QSqlDatabase::database(sqlName).commit();

    initSettings();
    btnSave->setEnabled(true);
}

void TypConfig::clickSave()
{
    if (isRemove) {
        removeModel();
        isRemove = false;
        return;
    }
    btnSave->setEnabled(false);
    int row = qMax(0, view->currentRow());
    QString n = view->item(row, 1)->text();  // 点中的型号
    QString c = name->text();  // 输出的型号
    if ((!c.isEmpty() && !n.isEmpty() && n == c) || c.isEmpty()) {  // 保存型号
        saveSettings();
    }
    if ((!c.isEmpty() && !n.isEmpty() && n != c)) {  // 修改型号
        removeModel();
    }
    if ((!c.isEmpty() && n.isEmpty())) {  // 添加型号
        createModel();
        selectModel();
    }
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
    if (row >= 0)
        name->setText(view->item(row, 1)->text());
}

void TypConfig::change()
{
    int row = view->currentRow();
    if (row >= 0) {
        isRemove = (!view->item(row, 1)->text().isEmpty() && name->text().isEmpty());
    }
}

void TypConfig::recvAppMsg(QTmpMap msg)
{
    switch (msg.value(Qt::Key_0).toInt()) {
    case Qt::Key_Copy:
        tmpSet = msg;
        btnSave->setEnabled(true);
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

