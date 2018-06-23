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
    t.start();
    //    calc();
    initUI();
    initDevice();
}

AppWindow::~AppWindow()
{
    sql->quit();
    sql->wait();
}

int AppWindow::initUI()
{
    initTitle();
    initLayout();
    initSqlDir();
    initAuthor();
    QTimer::singleShot(100, this, SLOT(initScreen()));
    return Qt::Key_Away;
}

void AppWindow::calc()
{
    int numb = 6;
    double a1 = 0;
    QVector<double> x(numb), y(numb);
    QVector<double> x1(numb/2), x2(numb/2);
    y[0] = 8800;
    y[1] = 8200;
    y[2] = 8100;
    y[3] = 8250;
    y[4] = 8839;
    y[5] = 8220;
    for (int i=0; i < numb; i++) {
        a1 += y[i];
    }
    for (int i=0; i < numb/2; i++) {
        double b1 = 0;
        double b2 = 0;
        for (int j = 0; j < numb/2; j++) {
            b1 += y.value((j + i) % (numb));
            b2 += y.value((j + i + numb/2) % (numb));
        }
        x1[i] = b1*b2/(b1+b2);
    }
    qDebug() << y << x1;
    for (int i=0; i < numb; i++) {
        double bb = 0;
        for (int j=0; j < numb; j++) {
            if (j != i)
                bb += y.value(j);
        }
        x[i] = y.value(i) * bb / a1;
    }

    double a2 = 0;
    for (int i=0; i < numb; i++) {
        a2 += x.value(i);
    }
    for (int i=0; i < numb/2; i++) {
        double b1 = 0;
        double b2 = 0;
        for (int j=0; j < numb/2; j++) {
            b1 += x.value((j + i) % numb);
            b2 += x.value((j + i + numb/2) % numb);
        }
        x2[i] = b1*b2/(b1+b2) * numb / (numb -1);
    }
    qDebug() << x << x2;

    //    for (int i=0; i < numb; i++) {
    //        double bb = 0;
    //        for (int j=0; j < numb; j++) {
    //            if (i != j)
    //                bb += x.value(j);
    //        }
    //        r[i] = x.value(i) * (numb) / (numb - 1) + 1;
    //        r[i] = (1 + (x.value(i) - a2/numb) * numb * numb / (numb-1) / (numb-1) / a2 ) * r[i];
    //    }
    //    qDebug() << x << x1;
    //    for (int i=0; i < numb/2; i++) {
    //        double b1 = 0;
    //        double b2 = 0;
    //        for (int j = 0; j < numb/2; j++) {
    //            b1 += r.value((j + i) % (numb));
    //            b2 += r.value((j + i + numb/2) % (numb));
    //        }
    //        x2[i] = b1*b2/(b1+b2);
    //    }
    //    qDebug() << r << x2;

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
    qDebug() << "app vern:" << verNumb;
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
    QString mark = tr("返回主页");
    initButton(mark, name);

    AppAuthor *app = new AppAuthor(this);
    app->setObjectName(name);
    connect(this, SIGNAL(sendAppMsg(QTmpMap)), app, SLOT(recvAppMsg(QTmpMap)));
    connect(app, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    stack->addWidget(app);

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
    scanner = new QTimer(this);
    connect(scanner, SIGNAL(timeout()), this, SLOT(showBarCode()));
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
    initMap[names.size()] = &AppWindow::readBackup;
    names << tr("正在读取后台数据库");
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

    for (int i=0; i < names.size(); i++)
        showBoxPop(names.at(i), i);

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
    QString mark = tr("返回测试");
    initButton(mark, name);

    AppTester *app = new AppTester(this);
    app->setObjectName("tester");
    connect(app, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    connect(this, SIGNAL(sendAppMsg(QTmpMap)), app, SLOT(recvAppMsg(QTmpMap)));
    stack->addWidget(app);

    return Qt::Key_Away;
}

int AppWindow::initSignin()
{
    tmpSet[DataSign] = 0;

    QString name = "signin";
    QString mark = tr("用户登录");
    initButton(mark, name);

    AppSignin *app = new AppSignin(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    connect(this, SIGNAL(sendAppMsg(QTmpMap)), app, SLOT(recvAppMsg(QTmpMap)));
    stack->addWidget(app);

    return Qt::Key_Away;
}

int AppWindow::initSystem()
{
    QString name = "system";
    QString mark = tr("系统设置");
    initButton(mark, name);

    AppSystem *app = new AppSystem(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    connect(this, SIGNAL(sendAppMsg(QTmpMap)), app, SLOT(recvAppMsg(QTmpMap)));
    stack->addWidget(app);

    return Qt::Key_Away;
}

int AppWindow::initOnline()
{
    //#ifndef __arm__
    //    QString name = "online";
    //    AppSystem *app = new AppSystem(this);
    //    app->setObjectName(name);
    //    connect(app, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    //    connect(this, SIGNAL(sendAppMsg(QTmpMap)), app, SLOT(recvAppMsg(QTmpMap)));
    //    stack->addWidget(app);

    //    initButton(tr("系统设置"), name);
    //#endif
    //#ifndef __arm__
    //    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL3", "mysql");
    //    db.setHostName("192.168.1.55");
    //    db.setUserName("root");
    //    db.setPassword("87973318");
    //    db.setDatabaseName("aip-server");
    //    if (!db.open()) {
    //        qDebug() << "open fail" << db.lastError();
    //    }
    //#endif
    return Qt::Key_Away;
}

int AppWindow::initMaster()
{
    QString name = "master";
    QString mark = tr("用户管理");
    initButton(mark, name);

    AppMaster *app = new AppMaster(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    connect(this, SIGNAL(sendAppMsg(QTmpMap)), app, SLOT(recvAppMsg(QTmpMap)));
    stack->addWidget(app);

    return Qt::Key_Away;
}

int AppWindow::initAction()
{
    QString name = "action";
    QString mark = tr("权限管理");
    initButton(mark, name);

    AppAction *app = new AppAction(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    connect(this, SIGNAL(sendAppMsg(QTmpMap)), app, SLOT(recvAppMsg(QTmpMap)));
    stack->addWidget(app);

    return Qt::Key_Away;
}

int AppWindow::initBackup()
{
    QString name = "backup";
    QString mark = tr("后台管理");
    initButton(mark, name);

    AppBackup *app = new AppBackup(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    connect(this, SIGNAL(sendAppMsg(QTmpMap)), app, SLOT(recvAppMsg(QTmpMap)));
    stack->addWidget(app);

    return Qt::Key_Away;
}

int AppWindow::initLogger()
{
    QString name = "logger";
    QString mark = tr("调试信息");
    initButton(mark, name);

    AppLogger *app = AppLogger::instance();
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    connect(this, SIGNAL(sendAppMsg(QTmpMap)), app, SLOT(recvAppMsg(QTmpMap)));
    stack->addWidget(app);

    return Qt::Key_Away;
}

int AppWindow::initConfig()
{
    QString name = "config";
    QString mark = tr("型号管理");
    initButton(mark, name);

    TypConfig *app = new TypConfig(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    connect(this, SIGNAL(sendAppMsg(QTmpMap)), app, SLOT(recvAppMsg(QTmpMap)));
    stack->addWidget(app);

    return Qt::Key_Away;
}

int AppWindow::initSetDcr()
{
    QString name = "setdcr";
    QString mark = tr("电阻配置");
    initButton(mark, name);

    TypSetDcr *app = new TypSetDcr(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    connect(this, SIGNAL(sendAppMsg(QTmpMap)), app, SLOT(recvAppMsg(QTmpMap)));
    stack->addWidget(app);

    return Qt::Key_Away;
}

int AppWindow::initSetAcw()
{
    QString name = "setacw";
    QString mark = tr("介电强度");
    initButton(mark, name);

    TypSetAcw *app = new TypSetAcw(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    connect(this, SIGNAL(sendAppMsg(QTmpMap)), app, SLOT(recvAppMsg(QTmpMap)));
    stack->addWidget(app);

    return Qt::Key_Away;
}

int AppWindow::initSetImp()
{
    QString name = "setimp";
    QString mark = tr("匝间配置");
    initButton(mark, name);

    TypSetImp *app = new TypSetImp(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    connect(this, SIGNAL(sendAppMsg(QTmpMap)), app, SLOT(recvAppMsg(QTmpMap)));
    stack->addWidget(app);

    return Qt::Key_Away;
}

int AppWindow::initOffDcr()
{
    QString name = "offdcr";
    QString mark = tr("电阻调试");
    initButton(mark, name);

    TypOffDcr *app = new TypOffDcr(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    connect(this, SIGNAL(sendAppMsg(QTmpMap)), app, SLOT(recvAppMsg(QTmpMap)));
    stack->addWidget(app);

    return Qt::Key_Away;
}

int AppWindow::initOffAcw()
{
    QString name = "offacw";
    QString mark = tr("绝缘调试");
    initButton(mark, name);

    TypOffAcw *app = new TypOffAcw(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    connect(this, SIGNAL(sendAppMsg(QTmpMap)), app, SLOT(recvAppMsg(QTmpMap)));
    stack->addWidget(app);

    return Qt::Key_Away;
}

int AppWindow::initOffImp()
{
    QString name = "offimp";
    QString mark = tr("匝间调试");
    initButton(mark, name);

    TypOffImp *app = new TypOffImp(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    connect(this, SIGNAL(sendAppMsg(QTmpMap)), app, SLOT(recvAppMsg(QTmpMap)));
    stack->addWidget(app);

    return Qt::Key_Away;
}

int AppWindow::initImport()
{
    sql = new QThread(this);
    sql->start();
    SqlImport *app = new SqlImport;
    connect(app, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    connect(this, SIGNAL(sendAppMsg(QTmpMap)), app, SLOT(recvAppMsg(QTmpMap)));
    app->moveToThread(sql);
    return Qt::Key_Away;
}

int AppWindow::initRecord()
{
    QString name = "record";
    QString mark = tr("数据管理");
    initButton(mark, name);

    SqlRecord *app = new SqlRecord(this);
    app->setObjectName(name);
    connect(this, SIGNAL(sendAppMsg(QTmpMap)), app, SLOT(recvAppMsg(QTmpMap)));
    stack->addWidget(app);

    return Qt::Key_Away;
}

int AppWindow::initUpload()
{
    QString name = "upload";
    QString mark = tr("数据上传");
    initButton(mark, name);

    SqlUpload *app = new SqlUpload(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    connect(this, SIGNAL(sendAppMsg(QTmpMap)), app, SLOT(recvAppMsg(QTmpMap)));
    stack->addWidget(app);

    return Qt::Key_Away;
}

int AppWindow::initSdcard()
{
#ifdef __arm__
    QString name = "sdcard";
    QString mark = tr("数据历史");
    initButton(mark, name);

    SqlSdcard *app = new SqlSdcard(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    stack->addWidget(app);
#endif
    return Qt::Key_Away;
}

int AppWindow::readBackup()
{
    QString name = "aip_backup";
    QSqlQuery query(QSqlDatabase::database("backup"));
    query.exec("select * from aip_backup");
    while (query.next()) {
        int uuid = query.value(0).toInt();
        tmpSet[uuid] = query.value(1).toString();
    }
    query.clear();
    qDebug() << "app read:" << name;
    return Qt::Key_Away;
}

int AppWindow::readSystem()
{
    QString name = "aip_system";
    QSqlQuery query(QSqlDatabase::database("system"));
    query.exec("select * from aip_system");
    while (query.next()) {
        int uuid = query.value(0).toInt();
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
    query.exec(QString("select * from '%1'").arg(name));
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
    taskMap["5"] = &AppWindow::taskStartWait;
    taskMap["6"] = &AppWindow::taskStartTest;
    taskMap["7"] = &AppWindow::taskStartSave;
    taskMap["8"] = &AppWindow::taskStartBeep;
    taskMap["9"] = &AppWindow::taskClearBeep;
    taskMap["10"] = &AppWindow::taskResetTest;

    testMap.clear();
    testMap["0"] = &AppWindow::testClearData;
    testMap["1"] = &AppWindow::testStartSend;
    testMap["2"] = &AppWindow::testStartTest;

    QTimer *tasks = new QTimer(this);
    connect(tasks, SIGNAL(timeout()), this, SLOT(taskThread()));
    tasks->start(10);

    currTask = 0;
    currTest = 0;
    station = 0x11;

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

void AppWindow::saveBackup()
{
    boxbar->setLabelText(tr("正在保存数据"));
    boxbar->show();
    wait(10);
    QString name = "backup";
    QSqlQuery query(QSqlDatabase::database(name));
    QList<int> uuids = tmpSet.keys();
    QSqlDatabase::database(name).transaction();
    for (int i=0; i < uuids.size(); i++) {
        int uuid = uuids.at(i);
        if (uuid < 20000 && uuid >= 10000) {
            query.prepare("replace into aip_backup values(?,?)");
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
        if (uuid < 30000 && uuid >= 20000) {
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

void AppWindow::saveConfig()
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
            query.prepare(QString("replace into '%1' values(?,?)").arg(type));
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
    testShift = Qt::Key_Away;
    qDebug() << "app save:" << tr("%1ms").arg(tt.elapsed(), 4, 10, QChar('0')) << type;
}

void AppWindow::saveBackup1(QTmpMap msg)
{

}

void AppWindow::saveSqlite1(QTmpMap msg)
{

}

void AppWindow::saveConfig1(QTmpMap msg)
{
    QElapsedTimer tt;
    tt.start();
    boxbar->setLabelText(tr("正在保存配置数据"));
    boxbar->show();
    wait(10);
    QString name = "config";
    QSqlQuery query(QSqlDatabase::database(name));
    QList<int> uuids = msg.keys();
    QSqlDatabase::database(name).transaction();
    QString type = tmpSet[tmpSet[DataFile].toInt()].toString();
    for (int i=0; i < uuids.size(); i++) {
        int uuid = uuids.at(i);
        if (uuid >= 40000) {
            tmpSet.insert(uuid, msg.value(uuid));
            query.prepare(QString("replace into '%1' values(?,?)").arg(type));
            query.addBindValue(uuid);
            query.addBindValue(msg.value(uuid));
            query.exec();
        }
        boxbar->setValue(i*99/uuids.size());
    }
    QSqlDatabase::database(name).commit();
    boxbar->setValue(99);
    wait(500);
    query.clear();
    boxbar->setValue(100);
    testShift = Qt::Key_Away;
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
    tmpMsg.insert(AddrEnum, Qt::Key_Time);
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
    tmpMsg.insert(AddrData, station);
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
    currItem = getNextItem();
    timeOut = t.elapsed();
    tmpSet[DataOKNG] = DataOK;
    qDebug() << "app view:" << tr("%1ms").arg(t.elapsed(), 4, 10, QChar('0'));
    return Qt::Key_Away;
}

int AppWindow::taskStartWait()
{
    int rr = tmpSet[AddrSyst].toInt();
    int tt = tmpSet[rr + 0x07].toDouble()*1000;
    return (t.elapsed() - timeOut > tt) ? Qt::Key_Away : Qt::Key_Meta;
}

int AppWindow::taskStartTest()
{
    int ret = testThread();
    return ret;
}

int AppWindow::taskStartSave()
{
    tmpSet.insert(AddrEnum, Qt::Key_Save);  // 存储数据
    tmpSet.insert(AddrText, "aip_record");
    emit sendAppMsg(tmpSet);
    qDebug() <<"app save:" << tr("%1ms").arg(t.elapsed(), 4, 10, QChar('0'));
    return Qt::Key_Away;
}

int AppWindow::taskStartBeep()
{
    int addr = tmpSet[AddrSyst].toInt();
    int beep = tmpSet[addr + SystBeep].toInt() * 10 + 9;
    tmpMsg.insert(AddrEnum, Qt::Key_Call);
    tmpMsg.insert(AddrBeep, beep);
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
    timeOut = t.elapsed();
    qDebug() <<"app beep:" << tr("%1ms").arg(t.elapsed(), 4, 10, QChar('0'));
    return Qt::Key_Away;
}

int AppWindow::taskClearBeep()
{
    int ret = Qt::Key_Meta;
    int rr = tmpSet[AddrSyst].toInt();
    int tt = tmpSet[rr + (tmpSet[DataOKNG].toInt() == DataOK ? 0x07 : 0x08)].toDouble()*1000;
    if (t.elapsed() - timeOut >= tt) {
        timeOut = t.elapsed();
        ret = Qt::Key_Away;
        tmpMsg.insert(AddrEnum, Qt::Key_Call);
        tmpMsg.insert(AddrText, tmpSet[DataOKNG].toInt() == DataOK ? "LEDG" : "LEDR");
        tmpMsg.insert(AddrBeep, 0);
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
        qDebug() <<"app wait:" << tr("%1ms").arg(t.elapsed(), 4, 10, QChar('0')) << tt;
    }
    return ret;
}

int AppWindow::taskResetTest()
{
    int addr = tmpSet[AddrBack].toInt();
    int test = tmpSet[addr + 7].toInt();
    int time = tmpSet[addr + 8].toInt();
    if (test >= 1) {
        if (t.elapsed() - timeOut >= time*100) {
            for (int i=0; i < taskMap.size(); i++) {
                if (taskMap[QString::number(i)] == &AppWindow::taskCheckPlay) {
                    currTask = i;
                    taskShift = Qt::Key_Play;
                    if (test == 1)
                        station = 0x11;
                    if (test == 2)
                        station = 0x14;
                    if (test == 3)
                        station = (station == 0x11) ? 0x14 : 0x11;
                    t.restart();
                }
            }
        }
    }
    return (test >= 1) ? Qt::Key_Meta : Qt::Key_Away;
}

int AppWindow::taskCheckStop()
{
    tmpMsg.insert(AddrEnum, Qt::Key_Call);
    tmpMsg.insert(AddrText, "LED1");
    tmpMsg.insert(AddrBeep, 0);
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
    taskClearData();
    return Qt::Key_Away;
}

int AppWindow::taskQuickConf()
{
    t.restart();
    screensShow("setdcr");
    wait(200);

    tmpMsg.insert(AddrEnum, Qt::Key_Zoom);
    tmpMsg.insert(AddrText, AddrDCRS1);
    emit sendAppMsg(tmpMsg);
    if (taskQuickWait() == Qt::Key_Meta)
        return Qt::Key_Meta;

    tmpMsg.insert(AddrEnum, Qt::Key_Zoom);
    tmpMsg.insert(AddrText, AddrDCRS3);
    emit sendAppMsg(tmpMsg);
    if (taskQuickWait() == Qt::Key_Meta)
        return Qt::Key_Meta;

    tmpMsg.insert(AddrEnum, Qt::Key_Zoom);
    tmpMsg.insert(AddrText, AddrDCRSW);
    emit sendAppMsg(tmpMsg);

    screensShow("setimp");
    wait(200);

    tmpMsg.insert(AddrEnum, Qt::Key_Zoom);
    tmpMsg.insert(AddrText, AddrIMPS1);
    emit sendAppMsg(tmpMsg);
    if (taskQuickWait() == Qt::Key_Meta)
        return Qt::Key_Meta;

    tmpMsg.insert(AddrEnum, Qt::Key_Zoom);
    tmpMsg.insert(AddrText, AddrIMPSW);
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();

    screensShow("config");
    wait(200);
    taskClearData();

    return Qt::Key_Meta;
}

int AppWindow::taskQuickWait()
{
    timeOut = t.elapsed();
    testShift = Qt::Key_Meta;
    while (1) {
        if (testShift == Qt::Key_Away)
            break;
        if (t.elapsed() - timeOut > 5000)
            break;
        wait(10);
    }
    wait(100);
    return testShift;
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
    tmpMsg.insert(AddrData, station);
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

void AppWindow::showBarCode()
{
    int pAddr = tmpSet[AddrSyst].toInt();
    int pCode = tmpSet[pAddr + SystCode].toInt();
    int pSize = tmpSet[pAddr + SystSize].toInt();
    barcode = barcode.mid(pCode, pSize);
    tmpSet[DataCode] = barcode;
    sendSqlite();
    scanner->stop();
    barcode.clear();
}

void AppWindow::keyReleaseEvent(QKeyEvent *e)
{
    barcode.append(e->text());
    scanner->start(200);
    e->accept();
}

void AppWindow::loopBoxbar()
{
    int t = 0;
    while (1) {
        if (boxbar->isHidden())
            break;
        boxbar->setValue(t);
        wait(100);
        t = (t + 1) % 100;
    }
}

void AppWindow::recvSqlMsg(QTmpMap msg)
{
    if (msg.value(AddrText).toString() == "copy") {  // 开始备份
        boxbar->setLabelText(tr("正在备份数据"));
        boxbar->show();
        QTimer::singleShot(10, this, SLOT(loopBoxbar()));
    }
    if (msg.value(AddrText).toString() == "over") {  // 备份完成
        boxbar->setValue(100);
    }
}

void AppWindow::recvNewMsg(QTmpMap msg)
{
#ifdef __arm__
    emit sendNetMsg(msg);
#endif
    int addr = msg.value(AddrText).toInt();
    int a = 0;
    QList <int> keys = msg.keys();
    int tt = 0;
    switch (addr) {
    case AddrModel:
        break;
    case AddrDCRS1:
    case AddrDCRS2:
    case AddrDCRS3:
        a = tmpSet.value(AddrDCRR1 + addr - AddrDCRS1).toInt();
        if (msg[a + STATDCRA].toInt() == 2) {
            testShift = Qt::Key_Away;
            for (int i=0; i < keys.size(); i++) {
                tt = keys.at(i);
                if (tt >= 30000 && tt < 40000)
                    tmpSet.insert(tt, msg[tt].toString());
            }
        }
        if (msg[a + OKNGDCRA].toInt() == 1) {
            tmpSet[DataOKNG] = DataNG;
        }
        break;
    case AddrACWS1:
    case AddrACWS2:
    case AddrACWS3:
    case AddrACWS4:
        if (msg[tmpSet[AddrACWR1 + addr - AddrACWS1].toInt() + STATACWA].toInt() == 0) {
            testShift = Qt::Key_Away;
            for (int i=0; i < keys.size(); i++) {
                tt = keys.at(i);
                if (tt >= 30000 && tt < 40000)
                    tmpSet.insert(tt, msg[tt].toString());
            }
        }
        if (msg[tmpSet[AddrACWR1 + addr - AddrACWS1].toInt() + OKNGACWA].toInt() == 1) {
            tmpSet[DataOKNG] = DataNG;
        }
        break;
    case AddrIMPS1:
        if (msg[tmpSet[AddrIMPR1].toInt() + STATIMPA].toInt() == 0) {
            testShift = Qt::Key_Away;
            for (int i=0; i < keys.size(); i++) {
                tt = keys.at(i);
                if (tt >= 30000 && tt < 40000)
                    tmpSet.insert(tt, msg[tt].toString());
            }
        }
        if (msg[tmpSet[AddrIMPR1].toInt() + OKNGIMPA].toInt() == 1) {
            tmpSet[DataOKNG] = DataNG;
        }
        break;
    default:
        break;
    }
    emit sendAppMsg(msg);
    if (tt != 0)
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
    int c = msg.value(AddrEnum).toInt();
#ifndef __arm__
    if (sender()->objectName() != "socket")
        emit sendNetMsg(msg);
#endif

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
    case Qt::Key_Copy:
        recvSqlMsg(msg);
        break;
    case Qt::Key_Save:
        tmpSet = msg;
        if (msg.value(AddrText).toString() == "aip_backup") {  // 后台参数保存
            saveBackup();
        }
        if (msg.value(AddrText).toString() == "aip_system") {  // 系统参数保存
            saveSqlite();
        }
        if (msg.value(AddrText).toString() == "aip_config") {  // 配置参数保存
            saveConfig();
        }
        if (msg.value(AddrText).toString() == "aip_reload") {  // 重新加载参数
            saveSqlite();
            readConfig();
        }
        sendSqlite();
        break;
    case Qt::Key_Book:
        if (msg.value(AddrText).toString() == "aip_backup") {  // 后台参数保存
            saveBackup1(msg);
        }
        if (msg.value(AddrText).toString() == "aip_system") {  // 系统参数保存
            saveSqlite1(msg);
        }
        if (msg.value(AddrText).toString() == "aip_config") {  // 配置参数保存
            saveConfig1(msg);
        }
        if (msg.value(AddrText).toString() == "aip_reload") {  // 重新加载参数
            saveSqlite1(msg);
            readConfig();
        }
        sendSqlite();
        break;
    case Qt::Key_Play:
        taskShift = Qt::Key_Play;
        if (msg.value(AddrText).toString() == "L")
            station = 0x11;
        if (msg.value(AddrText).toString() == "R")
            station = 0x14;
        break;
    case Qt::Key_Stop:  // 停止测试
        taskCheckStop();
        break;
    case Qt::Key_Time:
        emit sendAppMsg(msg);
        break;
    case Qt::Key_WLAN:
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
    case Qt::Key_Zoom:
        taskQuickConf();
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
