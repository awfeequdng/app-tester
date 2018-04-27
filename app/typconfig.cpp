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

    QGroupBox *box = new QGroupBox(this);
    box->setLayout(boxLayout);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(3, 0, 3, 0);
    layout->addWidget(box);
}

void TypConfig::initViewBar()
{
    QStringList headers;
    headers << tr("型号编号") << tr("型号名称");

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
    boxLayout->addWidget(view);
    view->setItemDelegate(new BoxQItems);
    connect(view, SIGNAL(clicked(QModelIndex)), this, SLOT(clickViewBar()));
}

void TypConfig::initConfigBar()
{
    setFrame = new QFrame(this);
    boxLayout->addWidget(setFrame);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setMargin(0);
    setFrame->setLayout(layout);

    layout->addWidget(new QLabel(tr("编号"), this));
    numb = new QLineEdit(this);
    numb->setFixedSize(50, 36);
    layout->addWidget(numb);

    layout->addWidget(new QLabel(tr("名称"), this));
    type = new QLineEdit(this);
    type->setFixedSize(150, 36);
    layout->addWidget(type);

    layout->addStretch();

    QPushButton *btnAdd = new QPushButton(this);
    btnAdd->setFixedSize(97, 40);
    btnAdd->setText(tr("添加"));
    layout->addWidget(btnAdd);
    connect(btnAdd, SIGNAL(clicked(bool)), this, SLOT(appendModelType()));

    QPushButton *btnDel = new QPushButton(this);
    btnDel->setFixedSize(97, 40);
    btnDel->setText(tr("删除"));
    layout->addWidget(btnDel);
    connect(btnDel, SIGNAL(clicked(bool)), this, SLOT(deleteModelType()));

    //    QPushButton *btnExport = new QPushButton(this);
    //    btnExport->setFixedSize(97, 40);
    //    btnExport->setText(tr("导出"));
    //    layout->addWidget(btnExport);
    //    connect(btnExport, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));
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

    btnLayout->addWidget(new QLabel(tr("当前型号:"), this));

    curr = new QLabel(this);
    btnLayout->addWidget(curr);

    btnLayout->addStretch();

    QPushButton *btnGet = new QPushButton(this);
    btnGet->setFixedSize(97, 40);
    btnGet->setText(tr("调入"));
    btnLayout->addWidget(btnGet);
    connect(btnGet, SIGNAL(clicked(bool)), this, SLOT(selectModelType()));

    boxLayout->addLayout(btnLayout);
}

void TypConfig::initSettings()
{
    int p = page->text().toInt() - 1;    // 页码
    int s = 0x0300 + C_ROW*p;       // 起始地址
    for (int i=0; i < C_ROW; i++) {
        int t = s + i;
        view->item(i, 0)->setText(QString("%1").arg(t-0x0300+1, 3, 10, QChar('0')));
        view->item(i, 1)->setText(config[QString::number(t)].toString());
    }

    curr->setText(config[ConfigAddr].toString());
}

void TypConfig::appendModelType()
{
    QString t_numb = numb->text();
    QString t_name = type->text();
    if (t_name.isEmpty())
        return;
    if (t_numb.isEmpty()) {
        for (int i=0x0300; i < 0x0300+0x0100; i++) {
            if (config[QString::number(i)].toString().isEmpty()) {
                t_numb = QString::number(i - 0x0300 + 1);
                break;
            }
        }
    }
    for (int i=0x0300; i < 0x0300+0x0100; i++) {
        if (config[QString::number(i)].toString() == t_name) {
            QString tmp = tr("发现编号%1与此型号名称相同").arg(i-0x0300+1, 3, 10, QChar('0'));
            QMessageBox::warning(this, tr("警告"), tmp, QMessageBox::Ok);
            return;
        }
    }
    t_numb = QString::number(0x0300 + (t_numb.toInt() - 1)%0x0100);
    if (!config[t_numb].toString().isEmpty()) {
        QString tmp = tr("此编号已使用");
        QMessageBox::warning(this, tr("警告"), tmp, QMessageBox::Ok);
        return;
    }

    QString name = "sqlite";
    QString c_name = config[ConfigAddr].toString();
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
    config[ConfigAddr] = t_name;
    emit sendAppMap(config);

    initSettings();
}

void TypConfig::deleteModelType()
{
    QString t_numb = numb->text();
    QString t_name = type->text();
    if (t_numb.isEmpty())
        return;
    if (t_name.isEmpty())
        return;

    QString c_name = config[ConfigAddr].toString();
    if (t_name == c_name) {
        QMessageBox::warning(this, tr("警告"), tr("不能删除当前型号"), QMessageBox::Ok);
        return;
    }

    QSqlQuery query(QSqlDatabase::database("sqlite"));
    query.exec(tr("drop table M_%1").arg(t_name));

    t_numb = QString::number(0x0300 + (t_numb.toInt() - 1)%0x0100);
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
    int s = 0x0300 + C_ROW*p + row; // 地址
    numb->setText(QString("%1").arg(s-0x0300+1, 3, 10, QChar('0')));
    type->setText(config[QString::number(s)].toString());
}

void TypConfig::recvAppMap(QVariantMap msg)
{
    switch (msg.value("enum").toInt()) {
    case Qt::Key_Option:
        for (int i=0x0300; i < 0x0300+0x0100; i++) {  // 型号信息存放在0x0300
            config[QString::number(i)] = msg[QString::number(i)];
        }
        config[ConfigAddr] = msg[ConfigAddr];  // 当前型号
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

