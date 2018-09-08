/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       调试信息
*******************************************************************************/
#include "applogger.h"

const int outShow = 0x00;
const int outFile = 0x01;
const int outFlip = 0x02;

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
    int grade = (combo->currentIndex()-1) % 4;
    switch (type) {
    case QtDebugMsg:  // 常规输出
        if (grade > 0)
            return;
        message = QString("D: ");
        break;
    case QtWarningMsg:  // 警告信息
        if (grade > 1)
            return;
        message = QString("W: ");
        break;
    case QtCriticalMsg:  // 严重错误
        if (grade > 2)
            return;
        message = QString("E: ");
        break;
    case QtFatalMsg:  // 灾难错误
        message = QString("F: ");
        break;
    default:
        break;
    }

    QString tstring = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    message.append(QString("[%1] ").arg(tstring));
    message.append(QString("%1 ").arg(msg));
    message.append(newLine);

    int outSave = (combo->currentIndex() - 1) / 4;

    if (outSave == outShow) {  // 常规输出
        text->insertPlainText(message);
    }
    if (outSave == outFile) {  // 文件输出
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
    if (outSave == outFlip) {  // 网络输出
        tmpMsg.insert(Qt::Key_0, Qt::Key_Flip);
        tmpMsg.insert(Qt::Key_1, msg);
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
    }
}

void AppLogger::initUI()
{
    initLayout();
    initTextBar();
    initButtonBar();
}

void AppLogger::initLayout()
{
    QVBoxLayout *mlayout = new QVBoxLayout(this);
    QGroupBox *box = new QGroupBox(this);
    box->setTitle(tr("调试信息"));
    mlayout->addWidget(box);
    layout = new QVBoxLayout;
    box->setLayout(layout);
}

void AppLogger::initTextBar()
{
    text = new QTextBrowser(this);
    layout->addWidget(text);
}

void AppLogger::initButtonBar()
{
    QHBoxLayout *btnLayout = new QHBoxLayout;
    layout->addLayout(btnLayout);

    combo = new QComboBox(this);
    combo->setFixedSize(280, 44);
    combo->setView(new QListView);
    combo->addItem(tr("目标:默认"));
    combo->addItem(tr("目标:窗口; 等级:一般"));
    combo->addItem(tr("目标:窗口; 等级:警告"));
    combo->addItem(tr("目标:窗口; 等级:严重"));
    combo->addItem(tr("目标:窗口; 等级:灾难"));
    combo->addItem(tr("目标:文件; 等级:一般"));
    combo->addItem(tr("目标:文件; 等级:警告"));
    combo->addItem(tr("目标:文件; 等级:严重"));
    combo->addItem(tr("目标:文件; 等级:灾难"));
    combo->addItem(tr("目标:网络; 等级:一般"));
    combo->addItem(tr("目标:网络; 等级:警告"));
    combo->addItem(tr("目标:网络; 等级:严重"));
    combo->addItem(tr("目标:网络; 等级:灾难"));
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

void AppLogger::saveSettings()
{
    tmpMsg.insert(DataSave, QString::number(combo->currentIndex()));
    tmpMsg.insert(Qt::Key_0, Qt::Key_Save);
    tmpMsg.insert(Qt::Key_1, "aip_system");
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
}

void AppLogger::changeHandle(int t)
{
#ifndef __arm__
    switch (t) {
    case 0:
        qInstallMessageHandler(0);
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        qInstallMessageHandler(outputHandle);
    default:  // PC端不使用打印到网络
        break;
    }
#else
    switch (t) {
    case 0:
        qInstallMsgHandler(0);
        break;
    default:
        qInstallMsgHandler(outputHandle);
        break;
    }
#endif
}

void AppLogger::recvAppMsg(QTmpMap msg)
{
    switch (msg.value(Qt::Key_0).toInt()) {
    case Qt::Key_Copy:
        tmpSet = msg;
        combo->setCurrentIndex(tmpSet.value(DataSave).toInt());
        break;
    default:
        break;
    }
}
