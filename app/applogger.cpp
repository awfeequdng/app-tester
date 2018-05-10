/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       调试信息
*******************************************************************************/
#include "applogger.h"

AppLogger *logger;
AppLogger *AppLogger::_instance = 0;  // 初始化静态变量

AppLogger *AppLogger::instance()  // 构建单实例日志对象
{
    if (!AppLogger::_instance)
        AppLogger::_instance = new AppLogger;
    return AppLogger::_instance;
}
#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
void outputHandle(QtMsgType type, const char* msg) {
    if (!logger) {
        logger = AppLogger::instance();
    }
    if (logger) {
        logger->output(type, QString(msg));
    }
}
#else
void outputHandle(QtMsgType type, const QMessageLogContext &, const QString &msg) {
    if (!logger) {
        logger = AppLogger::instance();
    }
    if (logger) {
        logger->output(type, msg);
    }
}
#endif
AppLogger::AppLogger(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void AppLogger::output(QtMsgType type, const QString &msg)
{
    QString message;
    QString newLine = "\r\n";
#ifdef __linux__
    newLine = "\n";
#endif
    switch (type) {
    case QtDebugMsg:
        message = QString("Debug: ");
        break;
    case QtWarningMsg:
        message = QString("Warn : ");
        break;
    case QtCriticalMsg:
        message = QString("Error: ");
        break;
    case QtFatalMsg:
        message = QString("Fatal: ");
        break;
    default:
        break;
    }

    QString tstring = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    message.append(QString("[%1] ").arg(tstring));
    message.append(QString("%1 ").arg(msg));
    message.append(newLine);

    if (combo->currentIndex() == 1) {
        text->insertPlainText(message);
    } else if (combo->currentIndex() == 2) {
        QDir dir;
        dir.mkdir("log");
        QString name = QString("log/%1.txt").arg(QDate::currentDate().toString("yyyy-MM-dd"));
        QFile log(name);
        log.open(QIODevice::WriteOnly | QIODevice::Append);
        QTextStream cout(&log);
        cout << message;
        log.flush();
        log.close();
    }
}

void AppLogger::initUI()
{
    initText();
    initButton();
    initLayout();
}

void AppLogger::initText()
{
    text = new QTextBrowser(this);
}

void AppLogger::initButton()
{
    btnLayout = new QHBoxLayout;

    combo = new QComboBox(this);
    combo->setFixedSize(120, 44);
    combo->setView(new QListView);
    combo->addItem(tr("默认输出"));
    combo->addItem(tr("输出到窗口"));
    combo->addItem(tr("输出到文件"));
    btnLayout->addWidget(combo);
    connect(combo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeHandle(int)));

    btnLayout->addStretch();

    QPushButton *btnClear = new QPushButton(tr("清空显示"), this);
    btnClear->setFixedSize(97, 44);
    btnLayout->addWidget(btnClear);
    connect(btnClear, SIGNAL(clicked(bool)), text, SLOT(clear()));

    QPushButton *btnSave = new QPushButton(tr("保存设置"), this);
    btnSave->setFixedSize(97, 44);
    btnLayout->addWidget(btnSave);
    connect(btnSave, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));
}

void AppLogger::initLayout()
{
    QVBoxLayout *boxLayout = new QVBoxLayout;
    boxLayout->addWidget(text);
    boxLayout->addLayout(btnLayout);

    QGroupBox *group = new QGroupBox(this);
    group->setTitle(tr("调试信息"));
    group->setLayout(boxLayout);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(group);
}

void AppLogger::saveSettings()
{  // 调试信息输出保存在0x001F
    config[QString::number(AddrLogger)] = QString::number(combo->currentIndex());
    config.insert("enum", Qt::Key_Save);
    emit sendAppMap(config);
}

void AppLogger::changeHandle(int t)
{
#ifndef __arm__
    switch (t) {
    case 0:
        qInstallMessageHandler(0);
        break;
    case 1:
        qInstallMessageHandler(outputHandle);
        break;
    case 2:
        qInstallMessageHandler(outputHandle);
        break;
    default:
        break;
    }
#else
    switch (t) {
    case 0:
        qInstallMsgHandler(0);
        break;
    case 1:
        qInstallMsgHandler(outputHandle);
        break;
    case 2:
        qInstallMsgHandler(outputHandle);
        break;
    default:
        break;
    }
#endif
}

void AppLogger::recvAppMap(QVariantMap msg)
{  // 调试信息输出保存在0x001F
    switch (msg.value("enum").toInt()) {
    case Qt::Key_Copy:
        config[QString::number(AddrLogger)] = msg[QString::number(AddrLogger)];
        combo->setCurrentIndex(config[QString::number(AddrLogger)].toInt());
        break;
    default:
        break;
    }
}
