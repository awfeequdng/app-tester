/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       登录界面
*******************************************************************************/
#include "appsignin.h"

AppSignin::AppSignin(QWidget *parent) : QDialog(parent)
{
    initUI();
}

void AppSignin::initUI()
{
    initLayout();
    initTitleBar();
    initLoginBar();
    initInputBar();
    initAboutBar();
    initApplyBar();
}

void AppSignin::initLayout()
{
    title = new QFrame(this);
    title->setStyleSheet("background:#005270;");

    stack = new QStackedWidget(this);

    QVBoxLayout *boxLayout = new QVBoxLayout;
    boxLayout->addWidget(title);
    boxLayout->addWidget(stack);
    boxLayout->setMargin(0);
    boxLayout->setSpacing(0);

    QGroupBox *group = new QGroupBox(this);
    group->setLayout(boxLayout);
    group->setFixedSize(435, 320);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addStretch();
    layout->addWidget(group);
    layout->addStretch();
    layout->setMargin(0);
}

void AppSignin::initTitleBar()
{
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    title->setLayout(layout);

    QLabel *icon  = new QLabel(this);
    icon->setPixmap(QPixmap(":/icon_link.ico"));
    icon->setScaledContents(true);
    icon->setMaximumSize(30, 30);
    layout->addWidget(icon);

    QLabel *text = new QLabel(tr("电枢转子测试系统登录"));
    layout->addWidget(text);
    layout->addStretch();

#ifndef __linux__
    QToolButton *exit = new QToolButton(this);
    exit->setIcon(QIcon(":/icon_ext.png"));
    exit->setFocusPolicy(Qt::NoFocus);
    exit->setIconSize(QSize(25, 25));
    exit->resize(25, 25);
    connect(exit, SIGNAL(clicked(bool)), this, SLOT(animationHide()));
    layout->addWidget(exit);
#endif
}

void AppSignin::initLoginBar()
{
    isOk = false;
    QGridLayout *layout = new QGridLayout;
    layout->setMargin(5);
    layout->setSpacing(5);
    layout->setColumnStretch(0, 1);
    layout->setColumnStretch(1, 1);
    layout->setColumnStretch(2, 1);
    layout->setColumnStretch(3, 1);
    QLabel *content = new QLabel(this);
    content->setPixmap(QPixmap(":/logo_aip.png"));
    content->setScaledContents(true);
    content->setFixedSize(425, 96);
    layout->addWidget(content, 0, 0, 1, 4);

    QLabel *label = new QLabel(this);
    QString web;
    web.append("<html><head/><body><p>");
    web.append("<img src=':/icon_net.png' height='81' width='81'/>");
    web.append("<br></br>");
#ifndef __linux__
    label->setOpenExternalLinks(true);
    web.append("<a style='font-size:18px;color:gray;'href=\"http://www.aipuo.com//\">联系我们");
#else
    web.append("<a style='font-size:18px;color:gray;'href=\"2\">联系我们");
    connect(label, SIGNAL(linkActivated(QString)), this, SLOT(clickLink(QString)));
#endif
    web.append("</p></body></html>");
    label->setText(web);
    layout->addWidget(label, 1, 0, 3, 1);

    username = new QComboBox(this);
    username->setView(new QListView);
    username->setMinimumSize(64, 32);
    layout->addWidget(username, 1, 1, 1, 2);

    password = new QLineEdit(this);
    password->setEchoMode(QLineEdit::Password);
    password->setMinimumSize(64, 32);
    layout->addWidget(password, 2, 1, 1, 2);
    password->setPlaceholderText(tr(" 密码"));
    connect(password, SIGNAL(returnPressed()), this, SLOT(checkSignin()));

    QLabel *setLabel = new QLabel(this);
    setLabel->setText(tr("<a style='font-size:18px;color:gray;'href=\"1\">登录设置"));
    layout->addWidget(setLabel, 3, 3);
    connect(setLabel, SIGNAL(linkActivated(QString)), this, SLOT(clickLink(QString)));

    autosave = new QCheckBox(this);
    autosave->setText(tr("记住密码"));
    autosave->setMinimumSize(64, 32);
    layout->addWidget(autosave, 3, 1);

    autosign = new QCheckBox(this);
    autosign->setText(tr("自动登录"));
    autosign->setMinimumSize(64, 32);
    layout->addWidget(autosign, 3, 2);

    QPushButton *btnSignin = new QPushButton(this);
    btnSignin->setText(tr("登录"));
    btnSignin->setMinimumSize(97, 44);
    btnSignin->setFocusPolicy(Qt::NoFocus);
    connect(btnSignin, SIGNAL(clicked(bool)), this, SLOT(checkSignin()));
    layout->addWidget(btnSignin, 4, 1, 1, 2);

    QFrame *frame = new QFrame(this);
    frame->setLayout(layout);
    stack->addWidget(frame);
}

void AppSignin::initInputBar()
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addStretch();

    QStringList tmp;
    tmp << tr("地址") << tr("端口") << tr("产线") << tr("工位")
        << tr("备用") << tr("备用") << tr("备用") << tr("备用");

    for (int i=0; i < tmp.size()/2; i++) {
        QComboBox *box1 = new QComboBox(this);
        box1->setFixedSize(148, 35);
        inputs.append(box1);
        QComboBox *box2 = new QComboBox(this);
        box2->setFixedSize(148, 35);
        inputs.append(box2);
        QHBoxLayout *boxLayout = new QHBoxLayout;
        boxLayout->addStretch();
        boxLayout->addWidget(new QLabel(tmp.at(i*2+0), this));
        boxLayout->addWidget(box1);
        boxLayout->addWidget(new QLabel(tmp.at(i*2+1), this));
        boxLayout->addWidget(box2);
        boxLayout->addStretch();
        layout->addLayout(boxLayout);
    }
    QLabel *setLabel = new QLabel(this);
    setLabel->setText(tr("<a style='font-size:25px;color:gray;'href=\"0\">返回登录"));
    layout->addWidget(setLabel);
    connect(setLabel, SIGNAL(linkActivated(QString)), this, SLOT(clickLink(QString)));

    layout->addStretch();

    QFrame *frame = new QFrame(this);
    frame->setLayout(layout);
    stack->addWidget(frame);
}

void AppSignin::initAboutBar()
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addStretch();

    QString base = "<p style='font-size:18px' align='left'>&nbsp;&nbsp;&nbsp;&nbsp;%1</p>";
    QStringList tmp;
    tmp << tr("青岛艾普智能仪器有限公司");
    tmp << tr("销售直线：18653230898") << tr("公司电话：0532-87973318")
        << tr("公司传真：0532-87973308") << tr("企业邮箱：aip@aipuo.com")
        << tr("公司地址：山东青岛李沧区九水东路320号");

    for (int i=0; i < tmp.size(); i++) {
        layout->addWidget(new QLabel(base.arg(tmp.at(i)), this));
    }

    QLabel *setLabel = new QLabel(this);
    setLabel->setText(tr("<a style='font-size:25px;color:gray;' href=\"0\">返回登录"));
    layout->addWidget(setLabel);
    connect(setLabel, SIGNAL(linkActivated(QString)), this, SLOT(clickLink(QString)));

    layout->addStretch();

    QFrame *frame = new QFrame(this);
    frame->setLayout(layout);
    stack->addWidget(frame);
}

void AppSignin::initApplyBar()
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addStretch();

    QString base = "<p style='font-size:25px;color:#00FF00;' align='center'>%1</p>";

    layout->addWidget(new QLabel(base.arg(tr("登录成功")), this));

    QLabel *setLabel = new QLabel(this);
    setLabel->setObjectName("logoff");
    setLabel->setText(tr("<a style='font-size:25px;color:gray;' href=\"0\">重新登录"));
    layout->addWidget(setLabel);
    connect(setLabel, SIGNAL(linkActivated(QString)), this, SLOT(clickLink(QString)));

    layout->addStretch();

    QFrame *frame = new QFrame(this);
    frame->setLayout(layout);
    stack->addWidget(frame);
}

void AppSignin::initSettings()
{
    QStringList users;
    QString curruser = config[QString::number(AddrSignIn + AddrSkewSU)].toString();
    QString currpass = config[QString::number(AddrSignIn + AddrSkewSP)].toString();
    QString currsave = config[QString::number(AddrSignIn + AddrSkewSS)].toString();
    QString currauto = config[QString::number(AddrSignIn + AddrSkewSA)].toString();
    users.append(curruser);
    for (int i=AddrMaster; i < AddrMaster+0x0100; i+=4) {  // 用户信息存放在0x0100
        if (config[QString::number(i)].toString() != curruser)
            users.append(config[QString::number(i)].toString());
    }
    users.removeAll("");
    username->clear();
    username->addItems(users);
    if (currsave == "1") {
        autosave->setChecked(true);
        password->setText(currpass);
    } else {
        password->clear();
    }
    if (currauto == "1") {
        autosign->setChecked(true);
        if (!isOk) {
            checkSignin();
        }
    }
}

void AppSignin::saveSettings()
{
    tmpMap.insert(QString::number(AddrSignIn + AddrSkewSU), username->currentText());
    tmpMap.insert(QString::number(AddrSignIn + AddrSkewSP), password->text());
    tmpMap.insert(QString::number(AddrSignIn + AddrSkewSS), (autosave->isChecked()) ? "1" : "0");
    tmpMap.insert(QString::number(AddrSignIn + AddrSkewSA), (autosign->isChecked()) ? "1" : "0");
    tmpMap.insert("enum", Qt::Key_Save);
    emit sendAppMap(tmpMap);
    tmpMap.clear();
}

void AppSignin::checkSignin()
{
    for (int i=AddrMaster; i < AddrMaster+0x0100; i+=4) {  // 用户信息存放在0x0100
        QString curruser = config[QString::number(i+AddrSkewMU)].toString();
        QString currpass = config[QString::number(i+AddrSkewMP)].toString();
        if (curruser == username->currentText()) {
            if (currpass == password->text()) {
                QString t = QDateTime::currentDateTime().toString("yy-MM-dd hh:mm:ss");
                config[QString::number(i + AddrSkewMT)] = t;
                stack->setCurrentIndex(3);
                saveSettings();
                tmpMap.insert("enum", Qt::Key_Game);
                emit sendAppMap(tmpMap);
                tmpMap.clear();
                isOk = true;
            } else {
                QMessageBox::warning(this, tr("警告"), tr("密码错误"), QMessageBox::Ok);
            }
            break;
        }
    }
}

void AppSignin::clickLink(QString msg)
{
    stack->setCurrentIndex(msg.toInt());
    if (sender()->objectName() == "logoff") {
        tmpMap.insert("enum", Qt::Key_Back);
        emit sendAppMap(tmpMap);
        tmpMap.clear();
    }
}

void AppSignin::recvAppMap(QVariantMap msg)
{
    switch (msg.value("enum").toInt()) {
    case Qt::Key_Copy:
        for (int i=AddrSignIn; i < AddrSignIn + 0x10; i++) {  // 登录信息存放在0x0040
            config[QString::number(i)] = msg[QString::number(i)];
        }
        for (int i=AddrMaster; i < AddrMaster+0x0100; i++) {  // 用户信息存放在0x0100
            config[QString::number(i)] = msg[QString::number(i)];
        }
        break;
    case Qt::Key_Game:
        initSettings();
        break;
    default:
        break;
    }
}

void AppSignin::showEvent(QShowEvent *e)
{
    this->setFocus();
    initSettings();
    e->accept();
}
