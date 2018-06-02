/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       电枢测试仪主界面
*******************************************************************************/
#include "appwindow.h"

typedef int (AppWindow::*pClass)(void);
QMap<int, pClass> initMap;
QMap<QString, pClass> taskMap;
QMap<QString, pClass> testMap;

AppWindow::AppWindow(QWidget *parent) : QMainWindow(parent)
{
    initUI();
    initDevice();
}

AppWindow::~AppWindow()
{
    sql->quit();
    sql->wait();
}

void AppWindow::initUI()
{
    initTitle();
    initLayout();
    initAuthor();
    QTimer::singleShot(100, this, SLOT(initScreen()));
    //    this->setWindowFlags(Qt::FramelessWindowHint);
}

int AppWindow::initTitle()
{
#ifdef __arm__
    this->setWindowFlags(Qt::FramelessWindowHint);
#endif
    char s_month[5];
    static const char month_names[] = "JanFebMarAprMayJunJulAugSepOctNovDec";
    int month, day, year;

    sscanf((__DATE__), "%s %d %d", s_month, &day, &year);
    month = (strstr(month_names, s_month)-month_names)/3+1;

    QDate dt;
    dt.setDate(year, month, day);
    static const QTime tt = QTime::fromString(__TIME__, "hh:mm:ss");

    QDateTime t(dt, tt);
    verNumb = QString("V-%1").arg(t.toString("yyMMdd-hhmm"));
    qDebug() << "app data:" << verNumb;
    this->setWindowTitle(tr("电枢测试仪%1").arg(verNumb));
    return Qt::Key_Away;
}

int AppWindow::initLayout()
{
    preindex = 0;
    stack = new QStackedWidget(this);
    btnLayout = new QVBoxLayout;
    btnLayout->setContentsMargins(0, 9, 0, 9);
    btnLayout->setSpacing(0);

    btnFrame = new QFrame(this);
    btnFrame->setLayout(btnLayout);
    btnFrame->setFixedWidth(WIDTH);
    btnFrame->hide();

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(btnFrame);
    layout->addWidget(stack);

    QFrame *frame = new QFrame(this);
    frame->setLayout(layout);

    this->setCentralWidget(frame);
    this->resize(800, 600);
    return Qt::Key_Away;
}

int AppWindow::initAuthor()
{
    QString name = "author";
    AppAuthor *app = new AppAuthor(this);
    app->setObjectName(name);
    connect(this, SIGNAL(sendAppMsg(QTmpMap)), app, SLOT(recvAppMsg(QTmpMap)));
    connect(app, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    stack->addWidget(app);

    initButton(tr("返回主页"), name);
    return Qt::Key_Away;
}

int AppWindow::initDevice()
{
#ifdef __arm__
    DevSerial *key = new DevSerial(this);
    connect(this, SIGNAL(sendAppMsg(QTmpMap)), key, SLOT(recvAppMsg(QTmpMap)));
    connect(key, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));

    DevSetRtc *rtc = new DevSetRtc(this);
    connect(this, SIGNAL(sendAppMsg(QTmpMap)), rtc, SLOT(recvAppMsg(QTmpMap)));

    DevBuzzer *pwm = new DevBuzzer(this);
    connect(this, SIGNAL(sendAppMsg(QTmpMap)), pwm, SLOT(recvAppMsg(QTmpMap)));

    DevScreen *lcd = new DevScreen(this);
    connect(this, SIGNAL(sendAppMsg(QTmpMap)), lcd, SLOT(recvAppMsg(QTmpMap)));

    DevSetCan *can = new DevSetCan(this);
    connect(this, SIGNAL(sendAppMsg(QTmpMap)), can, SLOT(recvAppMsg(QTmpMap)));
    connect(can, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
#endif
    return Qt::Key_Away;
}

int AppWindow::  initScreen()
{
    boxbar = new BoxDialog(this);
    boxbar->setValue(0);
    boxbar->show();

    QStringList names;

    initMap[names.size()] = &AppWindow::initTester;
    names << tr("正在初始化测试界面");
    initMap[names.size()] = &AppWindow::initSignin;
    names << tr("正在初始化登录界面");
    initMap[names.size()] = &AppWindow::initSystem;
    names << tr("正在初始化系统设置");
    initMap[names.size()] = &AppWindow::initOnline;
    names << tr("正在初始化在线设备");
    initMap[names.size()] = &AppWindow::initMaster;
    names << tr("正在初始化用户管理");
    initMap[names.size()] = &AppWindow::initAction;
    names << tr("正在初始化权限管理");
    initMap[names.size()] = &AppWindow::initBackup;
    names << tr("正在初始化后台设置");
    initMap[names.size()] = &AppWindow::initLogger;
    names << tr("正在初始化调试信息");
    initMap[names.size()] = &AppWindow::initConfig;
    names << tr("正在初始化型号管理");
    initMap[names.size()] = &AppWindow::initSetDcr;
    names << tr("正在初始化电阻配置");
    initMap[names.size()] = &AppWindow::initSetAcw;
    names << tr("正在初始化高压配置");
    initMap[names.size()] = &AppWindow::initSetImp;
    names << tr("正在初始化匝间配置");
    initMap[names.size()] = &AppWindow::initOffDcr;
    names << tr("正在初始化电阻调试");
    initMap[names.size()] = &AppWindow::initOffAcw;
    names << tr("正在初始化高压调试");
    initMap[names.size()] = &AppWindow::initOffImp;
    names << tr("正在初始化匝间调试");
    initMap[names.size()] = &AppWindow::initImport;
    names << tr("正在初始化数据存储");
    initMap[names.size()] = &AppWindow::initRecord;
    names << tr("正在初始化数据管理");
    initMap[names.size()] = &AppWindow::initUpload;
    names << tr("正在初始化上传管理");
    initMap[names.size()] = &AppWindow::initSdcard;
    names << tr("正在初始化历史数据");
    initMap[names.size()] = &AppWindow::initUnqual;
    names << tr("正在初始化数据分析");
    initMap[names.size()] = &AppWindow::initSqlDir;
    names << tr("正在创建配置数据库");
    initMap[names.size()] = &AppWindow::readSystem;
    names << tr("正在读取配置数据库");
    initMap[names.size()] = &AppWindow::readConfig;
    names << tr("正在读取型号数据库");
    initMap[names.size()] = &AppWindow::sendSqlite;
    names << tr("正在分发数据库配置");
    initMap[names.size()] = &AppWindow::initSocket;
    names << tr("正在初始化网络连接");
    initMap[names.size()] = &AppWindow::sendSignin;
    names << tr("正在初始化登录验证");
    initMap[names.size()] = &AppWindow::initThread;
    names << tr("正在初始化系统线程");

    for (int i=0; i < names.size(); i++) {
        showBoxPop(names.at(i), i);
    }
    btnLayout->addStretch();
    return Qt::Key_Away;
}

int AppWindow:: initSqlDir()
{  // 创建并打开数据库
    SqlCreate *app = new SqlCreate(this);
    connect(app, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    app->initSqlDir();
    return Qt::Key_Away;
}

int AppWindow::initTester()
{
    QString name = "tester";
    AppTester *app = new AppTester(this);
    app->setObjectName("tester");
    connect(app, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    connect(this, SIGNAL(sendAppMsg(QTmpMap)), app, SLOT(recvAppMsg(QTmpMap)));
    stack->addWidget(app);

    initButton(tr("返回测试"), name);
    return Qt::Key_Away;
}

int AppWindow::initSignin()
{
    tmpSet[DataSign] = 0;
    QString name = "signin";
    AppSignin *app = new AppSignin(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    connect(this, SIGNAL(sendAppMsg(QTmpMap)), app, SLOT(recvAppMsg(QTmpMap)));
    stack->addWidget(app);

    initButton(tr("用户登录"), name);
    return Qt::Key_Away;
}

int AppWindow::initSystem()
{
    QString name = "system";
    AppSystem *app = new AppSystem(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    connect(this, SIGNAL(sendAppMsg(QTmpMap)), app, SLOT(recvAppMsg(QTmpMap)));
    stack->addWidget(app);

    initButton(tr("系统设置"), name);
    return Qt::Key_Away;
}

int AppWindow::initOnline()
{
#ifndef __arm__
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL3", "mysql");
    db.setHostName("192.168.1.55");
    db.setUserName("root");
    db.setPassword("87973318");
    db.setDatabaseName("aip-server");
    if (!db.open()) {
        qDebug() << "open fail" << db.lastError();
    }
#endif
    return Qt::Key_Away;
}

int AppWindow::initMaster()
{
    QString name = "master";
    AppMaster *app = new AppMaster(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    connect(this, SIGNAL(sendAppMsg(QTmpMap)), app, SLOT(recvAppMsg(QTmpMap)));
    stack->addWidget(app);

    initButton(tr("用户管理"), name);
    return Qt::Key_Away;
}

int AppWindow::initAction()
{
    QString name = "action";
    AppAction *app = new AppAction(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    connect(this, SIGNAL(sendAppMsg(QTmpMap)), app, SLOT(recvAppMsg(QTmpMap)));
    stack->addWidget(app);

    initButton(tr("权限管理"), name);
    return Qt::Key_Away;
}

int AppWindow::initBackup()
{
    QString name = "backup";
    AppBackup *app = new AppBackup(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    connect(this, SIGNAL(sendAppMsg(QTmpMap)), app, SLOT(recvAppMsg(QTmpMap)));
    stack->addWidget(app);

    initButton(tr("后台管理"), name);
    return Qt::Key_Away;
}

int AppWindow::initLogger()
{
    QString name = "logger";
    AppLogger *app = AppLogger::instance();
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    connect(this, SIGNAL(sendAppMsg(QTmpMap)), app, SLOT(recvAppMsg(QTmpMap)));
    stack->addWidget(app);

    initButton(tr("调试信息"), name);
    return Qt::Key_Away;
}

int AppWindow::initConfig()
{
    QString name = "config";
    TypConfig *app = new TypConfig(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    connect(this, SIGNAL(sendAppMsg(QTmpMap)), app, SLOT(recvAppMsg(QTmpMap)));
    stack->addWidget(app);

    initButton(tr("型号管理"), name);
    return Qt::Key_Away;
}

int AppWindow::initSetDcr()
{
    QString name = "setdcr";
    TypSetDcr *app = new TypSetDcr(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    connect(this, SIGNAL(sendAppMsg(QTmpMap)), app, SLOT(recvAppMsg(QTmpMap)));
    stack->addWidget(app);

    initButton(tr("电阻配置"), name);
    return Qt::Key_Away;
}

int AppWindow::initSetAcw()
{
    QString name = "setacw";
    TypSetAcw *app = new TypSetAcw(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    connect(this, SIGNAL(sendAppMsg(QTmpMap)), app, SLOT(recvAppMsg(QTmpMap)));
    stack->addWidget(app);

    initButton(tr("介电强度"), name);
    return Qt::Key_Away;
}

int AppWindow::initSetImp()
{
    QString name = "setimp";
    TypSetImp *app = new TypSetImp(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    connect(this, SIGNAL(sendAppMsg(QTmpMap)), app, SLOT(recvAppMsg(QTmpMap)));
    stack->addWidget(app);

    initButton(tr("匝间配置"), name);
    return Qt::Key_Away;
}

int AppWindow::initOffDcr()
{
    QString name = "offdcr";
    TypOffDcr *app = new TypOffDcr(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    connect(this, SIGNAL(sendAppMsg(QTmpMap)), app, SLOT(recvAppMsg(QTmpMap)));
    stack->addWidget(app);

    initButton(tr("电阻调试"), name);
    return Qt::Key_Away;
}

int AppWindow::initOffAcw()
{
    QString name = "offacw";
    TypOffAcw *app = new TypOffAcw(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    connect(this, SIGNAL(sendAppMsg(QTmpMap)), app, SLOT(recvAppMsg(QTmpMap)));
    stack->addWidget(app);

    initButton(tr("绝缘调试"), name);
    return Qt::Key_Away;
}

int AppWindow::initOffImp()
{
    QString name = "offimp";
    TypOffImp *app = new TypOffImp(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    connect(this, SIGNAL(sendAppMsg(QTmpMap)), app, SLOT(recvAppMsg(QTmpMap)));
    stack->addWidget(app);

    initButton(tr("匝间调试"), name);
    return Qt::Key_Away;
}

int AppWindow::initImport()
{
    sql = new QThread(this);
    sql->start();
    SqlImport *app = new SqlImport;
    connect(this, SIGNAL(sendAppMsg(QTmpMap)), app, SLOT(recvAppMsg(QTmpMap)));
    app->moveToThread(sql);
    return Qt::Key_Away;
}

int AppWindow::initRecord()
{
    QString name = "record";
    SqlRecord *app = new SqlRecord(this);
    app->setObjectName(name);
    //    connect(app, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    //    connect(this, SIGNAL(sendAppMsg(QTmpMap)), app, SLOT(recvAppMsg(QTmpMap)));
    stack->addWidget(app);

    initButton(tr("数据管理"), name);
    return Qt::Key_Away;
}

int AppWindow::initUpload()
{
    QString name = "upload";
    SqlUpload *app = new SqlUpload(this);
    app->setObjectName(name);
    //    connect(app, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    //    connect(this, SIGNAL(sendAppMsg(QTmpMap)), app, SLOT(recvAppMsg(QTmpMap)));
    stack->addWidget(app);

    initButton(tr("数据上传"), name);
    return Qt::Key_Away;
}

int AppWindow::initSdcard()
{
    QString name = "sdcard";
    SqlSdcard *app = new SqlSdcard(this);
    app->setObjectName(name);
    //    connect(app, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    //    connect(this, SIGNAL(sendAppMsg(QTmpMap)), app, SLOT(recvAppMsg(QTmpMap)));
    stack->addWidget(app);

    initButton(tr("数据历史"), name);
    return Qt::Key_Away;
}

int AppWindow::initUnqual()
{
    QString name = "unqual";
    SqlUnqual *app = new SqlUnqual(this);
    app->setObjectName(name);
    //    connect(app, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    //    connect(this, SIGNAL(sendAppMsg(QTmpMap)), app, SLOT(recvAppMsg(QTmpMap)));
    stack->addWidget(app);

    initButton(tr("数据分析"), name);
    return Qt::Key_Away;
}

int AppWindow::readSystem()
{
    QString name = "aip_system";
    QSqlQuery query(QSqlDatabase::database("system"));
    query.exec("select * from aip_system");
    while (query.next()) {
        int uuid = query.value(0).toInt();
        //        if (uuid >= 2000 && uuid < 3000)  // 系统设置区
        tmpSet[uuid] = query.value(1).toString();
    }
    query.clear();
    qDebug() << "app read:" << name;
    return Qt::Key_Away;
}

int AppWindow::readConfig()
{
    QSqlQuery query(QSqlDatabase::database("config"));
    int r = tmpSet[DataFile].toInt();
    QString name = tmpSet[r].toString();
    query.exec(QString("select * from %1").arg(name));
    while (query.next()) {
        int uuid = query.value(0).toInt();
        //        if (uuid >= 3000 && uuid < 4000)  // 型号参数区
        tmpSet[uuid] = query.value(1).toString();
    }
    query.clear();
    qDebug() << "app read:" << name;
    return Qt::Key_Away;
}

int AppWindow::sendSqlite()
{
    users.clear();  // 用户名称
    shows.clear();  // 界面名称
    roles.clear();  // 界面权限
    forms.clear();  // 界面组别
    int r = tmpSet[AddrUser].toInt();
    for (int i=0; i < 100; i+=5) {
        users.append(tmpSet[r + i].toString());
    }
    int s = tmpSet[AddrShow].toInt();
    for (int i=0; i < 100; i+=4) {
        shows.append(tmpSet[s + i + AddrName].toString());
        roles.append(tmpSet[s + i + AddrRole].toInt());
        forms.append(tmpSet[s + i + AddrForm].toInt());
    }
    tmpSet[AddrEnum] = Qt::Key_Copy;
    tmpSet[DataSoft] = verNumb;
    emit sendAppMsg(tmpSet);

    return Qt::Key_Away;
}

int AppWindow::sendSignin()
{
    tmpMsg.insert(AddrEnum, Qt::Key_Game);
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();

    if (tmpSet[DataSign].toInt() == 1) {
        QPushButton *btn = NULL;
        for (int i=0; i < buttons.size(); i++) {
            if (buttons.at(i)->objectName() == "tester") {
                btn = buttons.at(i);
                QTimer::singleShot(500, btn, SLOT(click()));
                break;
            }
        }
    } else {
        tmpMsg.insert(AddrEnum, Qt::Key_View);
        tmpMsg.insert(AddrText, "signin");
        recvAppMsg(tmpMsg);
        tmpMsg.clear();
    }
    return Qt::Key_Away;
}

int AppWindow::initSocket()
{
    //    TcpSocket *tcp = new TcpSocket(this);
    //    tcp->setObjectName("socket");
    //    connect(tcp, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    //    connect(this, SIGNAL(sendNetMsg(QTmpMap)), tcp, SLOT(recvAppMsg(QTmpMap)));
    //    tcp->connectToServer(tmpSet);

    TcpServer *app = new TcpServer;
    app->setObjectName("socket");
    connect(app, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    connect(this, SIGNAL(sendNetMsg(QTmpMap)), app, SLOT(recvAppMsg(QTmpMap)));
#ifdef __arm__
    app->listen(QHostAddress::Any, 5999);
#else
    app->initSocket();
#endif
    app->moveToThread(sql);

    //    UdpSocket *udp = new UdpSocket(this);
    //    udp->setObjectName("socket");
    //    connect(udp, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    //    connect(this, SIGNAL(sendNetMsg(QTmpMap)), udp, SLOT(recvAppMsg(QTmpMap)));
    return Qt::Key_Away;
}

int AppWindow::initThread()
{
    taskMap.clear();
    taskMap["0"] = &AppWindow::taskClearData;
    taskMap["1"] = &AppWindow::taskCheckPlay;
    taskMap["2"] = &AppWindow::taskCheckSafe;
    taskMap["3"] = &AppWindow::taskCheckCode;
    taskMap["4"] = &AppWindow::taskStartView;
    taskMap["5"] = &AppWindow::taskStartTest;
    taskMap["6"] = &AppWindow::taskStartSave;
    taskMap["7"] = &AppWindow::taskStartBeep;
    taskMap["8"] = &AppWindow::taskClearBeep;
    taskMap["9"] = &AppWindow::taskResetTest;

    testMap.clear();
    testMap["0"] = &AppWindow::testClearData;
    testMap["1"] = &AppWindow::testStartSend;
    testMap["2"] = &AppWindow::testStartTest;

    QTimer *tasks = new QTimer(this);
    connect(tasks, SIGNAL(timeout()), this, SLOT(taskThread()));
    tasks->start(10);

    currTask = 0;
    currTest = 0;

    return Qt::Key_Away;
}

void AppWindow::initButton(QString title, QString name)
{
    QPushButton *btnTester = new QPushButton(title, this);
    btnTester->setFlat(true);
    btnTester->setCheckable(true);
    btnTester->setFixedSize(150, 55);
    btnTester->setFocusPolicy(Qt::NoFocus);
    buttons.append(btnTester);
    btnLayout->addWidget(btnTester);
    btnTester->setObjectName(name);
    connect(btnTester, SIGNAL(clicked(bool)), this, SLOT(clickButtons()));
}

void AppWindow::showBoxPop(QString text, int t)
{
    boxbar->setLabelText(text);
    wait(10);
    (this->*initMap[t])();
    boxbar->setValue((t+1)*100/initMap.size());
}

void AppWindow::saveSqlite()
{
    boxbar->setLabelText(tr("正在保存数据"));
    boxbar->show();
    wait(10);
    QString name = "system";
    QSqlQuery query(QSqlDatabase::database(name));
    QList<int> uuids = tmpSet.keys();
    QSqlDatabase::database(name).transaction();
    for (int i=0; i < uuids.size(); i++) {
        int uuid = uuids.at(i);
        if (uuid < 20000 && uuid >= 10000) {
            query.prepare("replace into aip_system values(?,?)");
            query.addBindValue(uuid);
            query.addBindValue(tmpSet[uuid]);
            query.exec();
        }
        boxbar->setValue(i*50/uuids.size());
    }
    QSqlDatabase::database(name).commit();
    boxbar->setValue(99);
    wait(500);
    query.clear();
    boxbar->setValue(100);
}

void AppWindow::saveModels()
{
    QElapsedTimer tt;
    tt.start();
    boxbar->setLabelText(tr("正在保存数据"));
    boxbar->show();
    wait(10);
    QString name = "config";
    QSqlQuery query(QSqlDatabase::database(name));
    QList<int> uuids = tmpSet.keys();
    QSqlDatabase::database(name).transaction();
    QString type = tmpSet[tmpSet[DataFile].toInt()].toString();
    for (int i=0; i < uuids.size(); i++) {
        int uuid = uuids.at(i);
        if (uuid >= 40000) {
            query.prepare(QString("replace into %1 values(?,?)").arg(type));
            query.addBindValue(uuid);
            query.addBindValue(tmpSet[uuid]);
            query.exec();
        }
        boxbar->setValue(i*99/uuids.size());
    }
    QSqlDatabase::database(name).commit();
    boxbar->setValue(99);
    wait(500);
    query.clear();
    boxbar->setValue(100);
    qDebug() << "app save:" << tr("%1ms").arg(tt.elapsed(), 4, 10, QChar('0')) << type;
}

void AppWindow::clickButtons()
{
    tmpMsg.insert(AddrEnum, Qt::Key_View);
    tmpMsg.insert(AddrText, sender()->objectName());
    recvAppMsg(tmpMsg);
    tmpMsg.clear();
}

bool AppWindow::checkAction(QString msg)
{
    int s = tmpSet[DataSign].toInt();  // 是否已登录
    int r = tmpSet[DataUser].toInt();  // 当前用户地址
    int c = (s == 1) ? tmpSet[r + AddrRole].toInt() : 4;  // 当前权限等级

    int addr = shows.indexOf(msg);
    int form = forms.at(addr);  // 当前界面组别
    int role = roles.at(addr);  // 当前界面权限
    QStringList gs;
    for (int i=0; i < shows.size(); i++) {
        if ((forms.at(i) == form && roles.at(i) >= c) || forms.at(i) == 0) {
            gs.append(shows.at(i));
        }
    }
    for (int i=0; i < buttons.size(); i++) {
        if (gs.contains(buttons.at(i)->objectName()))
            buttons.at(i)->show();
        else
            buttons.at(i)->hide();
        if (buttons.at(i)->objectName() == msg)
            buttons.at(i)->setChecked(true);
        else
            buttons.at(i)->setChecked(false);
    }
    if (form > 0)
        btnFrame->show();
    else
        btnFrame->hide();
    if (c > role)
        return false;
    tmpMsg.insert(AddrEnum, c < role ? Qt::Key_Less : Qt::Key_Meta);
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
    return true;
}

void AppWindow::screensShow(QString msg)
{
    qDebug() << "app show:" << msg;
    for (int i=0; i < stack->count(); i++) {
        if (stack->widget(i)->objectName() == msg) {
            if (preindex == i)
                break;
            stack->setCurrentIndex(i);
            cloudAntimation();
            preindex = i;
            break;
        }
    }
    if (msg == "tester") {
        tmpMsg.insert(AddrEnum, Qt::Key_Send);
        tmpMsg.insert(AddrText, AddrModel);
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
    }
}

void AppWindow::recvAppShow(QString msg)
{
    if (!checkAction(msg)) {
        QString w = tr("您无权限进行此操作,请重新登录");
        QMessageBox::warning(this, tr("警告"), w, QMessageBox::Ok);
        screensShow("signin");
    } else {
        screensShow(msg);
    }
}

void AppWindow::cloudAntimation()
{
    QLabel* circle = new QLabel(stack->currentWidget());
    QLabel* line = new QLabel(this);
    line->setObjectName(QString("AntimationLine"));
    line->resize(0, 2);
    line->show();
#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    circle->setPixmap(QPixmap::grabWindow(stack->widget(preindex)->winId()));
#else
    circle->setPixmap(stack->widget(preindex)->grab());
#endif
    circle->show();
    circle->resize(stack->currentWidget()->size());
    QPropertyAnimation *animation = new QPropertyAnimation(circle, "geometry");

    animation->setDuration(1000);
    animation->setStartValue(circle->geometry());

    QPropertyAnimation* animation_line = new QPropertyAnimation(line, "size");
    animation_line->setDuration(1000);
    animation_line->setEasingCurve(QEasingCurve::OutCubic);

    animation->setEndValue(QRect(-3, 0, 0, circle->height()));
    line->move(stack->x(), stack->y() - 2);
    animation_line->setStartValue(QSize(0, 2));
    animation_line->setEndValue(QSize(stack->width(), 2));

    animation->setEasingCurve(QEasingCurve::OutCubic);

    QPropertyAnimation* animation_opacity = new QPropertyAnimation(circle, "windowOpacity");
    animation_opacity->setDuration(1000);
    animation_opacity->setStartValue(1);
    animation_opacity->setEndValue(0);
    animation_opacity->setEasingCurve(QEasingCurve::OutCubic);

    QParallelAnimationGroup *group = new QParallelAnimationGroup;

    connect(group, SIGNAL(finished()), circle, SLOT(hide()));
    connect(group, SIGNAL(finished()), circle, SLOT(deleteLater()));
    connect(group, SIGNAL(finished()), line, SLOT(deleteLater()));
    connect(group, SIGNAL(finished()), group, SLOT(deleteLater()));
    connect(group, SIGNAL(finished()), animation, SLOT(deleteLater()));
    connect(group, SIGNAL(finished()), animation_opacity, SLOT(deleteLater()));
    connect(group, SIGNAL(finished()), animation_line, SLOT(deleteLater()));
    group->addAnimation(animation);
    group->addAnimation(animation_opacity);
    group->addAnimation(animation_line);
    group->start();
}

int AppWindow::taskThread()
{
    tmpMsg.insert(AddrEnum, Qt::Key_Plus);
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();

    int ret = Qt::Key_Meta;
    if (taskMap.keys().contains(QString::number(currTask)))
        ret = (this->*taskMap[QString::number(currTask)])();
    if (ret == Qt::Key_Away) {
        currTask++;
        currTask = (currTask >= taskMap.size()) ? 0 : currTask;
    }
    return ret;
}

int AppWindow::taskClearData()
{
    timeOut = 0;
    timeTst = 0;
    currTask = 0;
    currTest = 0;
    currItem = 0;
    taskShift = Qt::Key_Meta;
    testShift = Qt::Key_Meta;
    return Qt::Key_Away;
}

int AppWindow::taskCheckPlay()
{
    int ret = Qt::Key_Meta;
    if (taskShift == Qt::Key_Play) {
        if (stack->currentWidget()->objectName() != "tester") {
            taskClearData();
        } else {
            ret = Qt::Key_Away;
            t.restart();
        }
    }
    return ret;
}

int AppWindow::taskCheckSafe()
{
    return Qt::Key_Away;
}

int AppWindow::taskCheckCode()
{
    return Qt::Key_Away;
}

int AppWindow::taskStartView()
{
    tmpMsg.insert(AddrEnum, Qt::Key_Call);
    tmpMsg.insert(AddrText, "LEDY");
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
    currItem = getNextItem();
    qDebug() << "app view:" << tr("%1ms").arg(t.elapsed(), 4, 10, QChar('0'));
    return Qt::Key_Away;
}

int AppWindow::taskStartTest()
{
    int ret = testThread();
    return ret;
}

int AppWindow::taskStartSave()
{
    tmpSet.insert(AddrEnum, Qt::Key_Book);
    emit sendAppMsg(tmpSet);
    qDebug() <<"app save:" << tr("%1ms").arg(t.elapsed(), 4, 10, QChar('0'));
    return Qt::Key_Away;
}

int AppWindow::taskStartBeep()
{
    timeOut = t.elapsed();
    qDebug() <<"app beep:" << tr("%1ms").arg(timeOut, 4, 10, QChar('0'));
    tmpMsg.insert(AddrEnum, Qt::Key_Call);
    tmpMsg.insert(AddrText, tmpSet[DataOKNG].toInt() == DataOK ? "LEDG" : "LEDR");
    tmpMsg.insert(AddrBeep, 100);
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
    return Qt::Key_Away;
}

int AppWindow::taskClearBeep()
{
    int ret = Qt::Key_Meta;
    int rr = tmpSet[AddrSyst].toInt();
    int tt = tmpSet[rr + (tmpSet[DataOKNG].toInt() == DataOK ? 0x07 : 0x08)].toDouble()*100;
    if (t.elapsed() - timeOut > tt) {
        timeOut = t.elapsed();
        ret = Qt::Key_Away;
        tmpMsg.insert(AddrEnum, Qt::Key_Call);
        tmpMsg.insert(AddrText, tmpSet[DataOKNG].toInt() == DataOK ? "LEDG" : "LEDR");
        tmpMsg.insert(AddrBeep, 0);
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
        qDebug() <<"app wait:" << tr("%1ms").arg(t.elapsed(), 4, 10, QChar('0'));
    }
    return ret;
}

int AppWindow::taskResetTest()
{
    int addr = tmpSet[AddrBack].toInt();
    int test = tmpSet[addr + 7].toInt();
    int time = tmpSet[addr + 8].toInt();
    if (test == 1) {
        if (t.elapsed() - timeOut >= time*100) {
            for (int i=0; i < taskMap.size(); i++) {
                if (taskMap[QString::number(i)] == &AppWindow::taskCheckPlay) {
                    currTask = i + 1;
                    t.restart();
                }
            }
        }
    }
    return (test == 1) ? Qt::Key_Meta : Qt::Key_Away;
}

int AppWindow::testThread()
{
    int ret = Qt::Key_Meta;
    if (testMap.keys().contains(QString::number(currTest))) {
        if ((this->*testMap[QString::number(currTest)])() == Qt::Key_Away) {
            currTest++;
            currTest = (currTest >= testMap.size()) ? 0 : currTest;  // 测试流程
            if (currTest == 0) {
                currItem = getNextItem();
                if (currItem == 0)
                    ret = Qt::Key_Away;
            }
        }
    }
    return ret;
}

int AppWindow::testClearData()
{
    testShift = Qt::Key_Meta;
    timeTst = 0;
    return Qt::Key_Away;
}

int AppWindow::testStartSend()
{
    tmpMsg.insert(AddrEnum, Qt::Key_Play);
    tmpMsg.insert(AddrText, currItem);
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
    return Qt::Key_Away;
}

int AppWindow::testStartTest()
{
    int ret = Qt::Key_Meta;
    if (testShift == Qt::Key_Away) {
        ret = Qt::Key_Away;
    }
    return ret;
}

int AppWindow::getNextItem()
{
    int item = currItem;
    while (1) {
        item++;
        if (AddrModel + item > AddrIMPS1) {
            item = 0;
            break;
        }
        int r = tmpSet[AddrModel + item].toInt();
        if (tmpSet[r].toInt() == 1) {
            break;
        }
    }
    return item;
}

void AppWindow::recvNewMsg(QTmpMap msg)
{
#ifdef __arm__
    emit sendNetMsg(msg);
#endif
    int addr = msg.value(AddrText).toInt();
    QList <int> keys = msg.keys();
    int tt = 0;
    switch (addr) {
    case AddrModel:
        break;
    case AddrDCRS1:
    case AddrDCRS2:
    case AddrDCRS3:
        if (msg[tmpSet[AddrDCRR1 + addr - AddrDCRS1].toInt() + AddrDataS].toInt() == 2) {
            testShift = Qt::Key_Away;
            for (int i=0; i < keys.size(); i++) {
                tt = keys.at(i);
                if (tt >= 30000 && tt < 40000)
                    tmpSet.insert(tt, msg[tt].toString());
            }
        }
        if (msg[tmpSet[AddrDCRR1 + addr - AddrDCRS1].toInt() + AddrDataJ].toInt() == 1) {
            tmpSet[DataOKNG] = DataNG;
        }
        break;
    case AddrACWS1:
    case AddrACWS2:
    case AddrACWS3:
    case AddrACWS4:
        if (msg[tmpSet[AddrACWR1 + addr - AddrACWS1].toInt() + AddrDataS].toInt() == 0) {
            testShift = Qt::Key_Away;
            for (int i=0; i < keys.size(); i++) {
                tt = keys.at(i);
                if (tt >= 30000 && tt < 40000)
                    tmpSet.insert(tt, msg[tt].toString());
            }
        }
        if (msg[tmpSet[AddrACWR1 + addr - AddrACWS1].toInt() + AddrDataJ].toInt() == 1) {
            tmpSet[DataOKNG] = DataNG;
        }
        break;
    case AddrIMPS1:
        if (msg[tmpSet[AddrIMPR1 + addr - AddrIMPS1].toInt() + AddrDataS].toInt() == 0) {
            testShift = Qt::Key_Away;
            for (int i=0; i < keys.size(); i++) {
                tt = keys.at(i);
                if (tt >= 30000 && tt < 40000)
                    tmpSet.insert(tt, msg[tt].toString());
            }
        }
        if (msg[tmpSet[AddrIMPR1 + addr - AddrIMPS1].toInt() + AddrDataJ].toInt() == 1) {
            tmpSet[DataOKNG] = DataNG;
        }
        break;
    default:
        break;
    }
    emit sendAppMsg(msg);
    qDebug() << "app recv:" << tr("%1ms").arg(t.elapsed(), 4, 10, QChar('0'));
}

void AppWindow::recvTmpMsg(QTmpMap msg)
{
#ifdef __arm__
    emit sendNetMsg(msg);
#endif
    emit sendAppMsg(msg);
}

void AppWindow::recvAppMsg(QTmpMap msg)
{
#ifndef __arm__
    if (sender()->objectName() != "socket")
        emit sendNetMsg(msg);
#endif
    int c = msg.value(AddrEnum).toInt();
    switch (c) {
    case Qt::Key_Shop:
        recvTmpMsg(msg);
        break;
    case Qt::Key_View:
        recvAppShow(msg.value(AddrText).toString());
        break;
    case Qt::Key_Back:
        tmpSet[DataSign] = 0;
        recvAppShow("signin");
        break;
    case Qt::Key_Save:
        tmpSet = msg;
        saveSqlite();
        sendSqlite();
        break;
    case Qt::Key_Memo:
        tmpSet = msg;
        saveModels();
        sendSqlite();
        break;
    case Qt::Key_Play:
        taskShift = Qt::Key_Play;
        break;
    case Qt::Key_Stop:  // 停止测试
        taskClearData();
        tmpMsg.insert(AddrEnum, Qt::Key_Call);
        tmpMsg.insert(AddrText, "LED1");
        tmpMsg.insert(AddrBeep, 0);
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
        break;
    case Qt::Key_Word:  // 调入参数
        tmpSet = msg;
        saveSqlite();
        readConfig();
        sendSqlite();
        break;
    case Qt::Key_Time:
        emit sendAppMsg(msg);
        break;
    case Qt::Key_WLAN:
        emit sendAppMsg(msg);
        break;
    case Qt::Key_Call:
        emit sendAppMsg(msg);
        break;
    case Qt::Key_News:
        recvNewMsg(msg);
        break;
    case Qt::Key_Send:  // 下发配置
        emit sendAppMsg(msg);
        break;
    case Qt::Key_Xfer:
        tmpSet = msg;
        break;
    case Qt::Key_Flip:
#ifdef __arm__
        emit sendNetMsg(msg);
#else
        qDebug() << "tcp recv:" << msg.value(AddrText).toString();
#endif
        break;
    default:
        break;
    }
}

void AppWindow::wait(int ms)
{
    QElapsedTimer t;
    t.start();
    while (t.elapsed() < ms)
        QCoreApplication::processEvents();
}
