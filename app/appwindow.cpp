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
    QString verNumb = QString("V-%1").arg(t.toString("yyMMdd-hhmm"));
    tmpSet[DataSoft] = verNumb;
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

int AppWindow::initScreen()
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
    initMap[names.size()] = &AppWindow::initRecord;
    names << tr("正在初始化数据管理");
    initMap[names.size()] = &AppWindow::initUpload;
    names << tr("正在初始化上传管理");
    initMap[names.size()] = &AppWindow::initSdcard;
    names << tr("正在初始化历史数据");
    initMap[names.size()] = &AppWindow::initUnqual;
    names << tr("正在初始化数据分析");
    initMap[names.size()] = &AppWindow::initTmpDat;
    names << tr("正在初始化临时数据");
    initMap[names.size()] = &AppWindow::initSqlDir;
    names << tr("正在创建配置数据库");
    initMap[names.size()] = &AppWindow::readSqlite;
    names << tr("正在读取配置数据库");
    initMap[names.size()] = &AppWindow::readModels;
    names << tr("正在读取型号数据库");
    initMap[names.size()] = &AppWindow::sendSqlite;
    names << tr("正在分发数据库配置");
    initMap[names.size()] = &AppWindow::initSocket;
    names << tr("正在初始化网络连接");
    initMap[names.size()] = &AppWindow::sendSignin;
    names << tr("正在初始化登录验证");
    initMap[names.size()] = &AppWindow::initThread;
    names << tr("正在初始化系统线程");

    //    initMap[names.at(names.size() - 3)] = &AppWindow::initSqlDir;
    //    initMap[names.at(names.size() - 2)] = &AppWindow::initTester;
    //    initMap[names.at(names.size() - 1)] = &AppWindow::initSignin;
    //    names << tr("正在初始化系统设置") << tr("正在初始化用户管理") << tr("正在初始化权限管理");
    //    initMap[names.at(names.size() - 3)] = &AppWindow::initSystem;
    //    initMap[names.at(names.size() - 2)] = &AppWindow::initMaster;
    //    initMap[names.at(names.size() - 1)] = &AppWindow::initAction;
    //    names << tr("正在初始化后台设置") << tr("正在初始化调试信息") << tr("正在初始化型号管理");
    //    initMap[names.at(names.size() - 3)] = &AppWindow::initBackup;
    //    initMap[names.at(names.size() - 2)] = &AppWindow::initLogger;
    //    initMap[names.at(names.size() - 1)] = &AppWindow::initConfig;
    //    names << tr("正在初始化电阻配置") << tr("正在初始化高压配置") << tr("正在初始化匝间配置");
    //    initMap[names.at(names.size() - 3)] = &AppWindow::initSetDcr;
    //    initMap[names.at(names.size() - 2)] = &AppWindow::initSetAcw;
    //    initMap[names.at(names.size() - 1)] = &AppWindow::initSetImp;
    //    names << tr("正在初始化电阻调试") << tr("正在初始化高压调试") << tr("正在初始化匝间调试");
    //    initMap[names.at(names.size() - 3)] = &AppWindow::initOffDcr;
    //    initMap[names.at(names.size() - 2)] = &AppWindow::initOffAcw;
    //    initMap[names.at(names.size() - 1)] = &AppWindow::initOffImp;
    //    names << tr("正在初始化数据管理") << tr("正在初始化上传管理") << tr("正在初始化历史数据");
    //    initMap[names.at(names.size() - 3)] = &AppWindow::initRecord;
    //    initMap[names.at(names.size() - 2)] = &AppWindow::initUpload;
    //    initMap[names.at(names.size() - 1)] = &AppWindow::initSdcard;
    //    names << tr("正在初始化数据分析") << tr("正在读取配置数据库") << tr("正在初始化临时数据")
    //          << tr("正在读取型号数据库");
    //    initMap[names.at(names.size() - 4)] = &AppWindow::initUnqual;
    //    initMap[names.at(names.size() - 3)] = &AppWindow::initTmpDat;
    //    initMap[names.at(names.size() - 2)] = &AppWindow::readSqlite;
    //    initMap[names.at(names.size() - 1)] = &AppWindow::readModels;
    //    names << tr("正在分发数据库配置") << tr("正在初始化网络连接") << tr("正在初始化登录验证");
    //    initMap[names.at(names.size() - 3)] = &AppWindow::sendSqlite;
    //    initMap[names.at(names.size() - 2)] = &AppWindow::initSocket;
    //    initMap[names.at(names.size() - 1)] = &AppWindow::sendSignin;
    //    names << tr("正在初始化系统线程");
    //    initMap[names.at(names.size() - 1)] = &AppWindow::initThread;
    for (int i=0; i < names.size(); i++) {
        showBoxPop(names.at(i), i);
    }
    btnLayout->addStretch();
    return Qt::Key_Away;
}

int AppWindow::initSqlDir()
{  // 创建并打开数据库
    QDir dir;
    if (!dir.exists("nandflash")) {
        dir.mkdir("nandflash");
    }
    bool s = false;
    bool r = false;
    if (!dir.exists("./nandflash/sqlite.db")) {
        QFile file("./nandflash/sqlite.db");
        file.open(QIODevice::ReadWrite | QIODevice::Text);
        file.close();
        s = true;
    }
    if (!dir.exists("nandflash/record.db")) {
        QFile file("nandflash/record.db");
        file.open(QIODevice::ReadWrite | QIODevice::Text);
        file.close();
        r = true;
    }
    if (s || r) {
        SqlCreate c;
        if (s)
            c.createSqlite();
        if (r)
            c.createRecord();
        c.deleteLater();
    }
    wait(100);
    QSqlDatabase sqlite = QSqlDatabase::addDatabase("QSQLITE", "sqlite");
    sqlite.setDatabaseName("./nandflash/sqlite.db");
    if (sqlite.open()) {
    } else {
        qDebug() << "sqlite:" << sqlite.lastError();
    }
    QSqlDatabase record = QSqlDatabase::addDatabase("QSQLITE", "record");
    record.setDatabaseName("./nandflash/record.db");
    if (record.open()) {
    } else {
        qDebug() << "record:" << record.lastError();
    }
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
    //    connect(app, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    //    connect(this, SIGNAL(sendAppMsg(QTmpMap)), app, SLOT(recvAppMsg(QTmpMap)));
    stack->addWidget(app);

    initButton(tr("匝间调试"), name);
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

int AppWindow::initTmpDat()
{
    tmpSet.insert(AddrDevA, 1100);
    tmpSet.insert(AddrINRA, 1150);  // 绝缘结果存放在150
    tmpSet.insert(AddrACWL, 1155);  // 轴铁结果存放在160
    tmpSet.insert(AddrACWC, 1160);  // 轴线结果存放在170
    tmpSet.insert(AddrACWA, 1165);  // 铁线结果存放在180
    tmpSet.insert(AddrWeld, 1200);  // 电阻结果存放在200
    tmpSet.insert(AddrChip, 1300);
    tmpSet.insert(AddrDiag, 1350);
    tmpSet.insert(AddrIMPA, 1400);  // 匝间结果存放在400
    tmpSet.insert(AddrIMPW, 1600);  // 匝间波形存放在600
    tmpSet.insert(AddrBack, 2020);
    tmpSet.insert(AddrDCRB, 2040);
    tmpSet.insert(AddrINRB, 2060);
    tmpSet.insert(AddrACWB, 2080);
    tmpSet.insert(AddrIMPB, 2100);
    tmpSet.insert(AddrSyst, 2120);
    tmpSet.insert(AddrInfo, 2140);
    tmpSet.insert(AddrLoad, 2160);
    tmpSet.insert(AddrType, 2500);
    tmpSet.insert(AddrShow, 2200);  // 界面信息存入在2200
    tmpSet.insert(AddrUser, 2300);  // 用户信息存放在2300

    tmpSet.insert(AddrModel, 3000);
    tmpSet.insert(AddrDCRS1, 3020);
    tmpSet.insert(AddrDCRS2, 3030);
    tmpSet.insert(AddrDCRS3, 3040);
    tmpSet.insert(AddrACWS1, 3050);
    tmpSet.insert(AddrACWS2, 3060);
    tmpSet.insert(AddrACWS3, 3070);
    tmpSet.insert(AddrACWS4, 3080);
    tmpSet.insert(AddrIMPS1, 3090);
    tmpSet.insert(AddrDCRSW, 3200);
    tmpSet.insert(AddrIMPSW, 3600);
    return Qt::Key_Away;
}

int AppWindow::readSqlite()
{
    QString name = "sqlite";
    qDebug() << "app read:" << name;
    QSqlQuery query(QSqlDatabase::database(name));
    query.exec("select * from G_DEFAULT");
    while (query.next()) {
        int uuid = query.value(0).toInt();
        if (uuid >= 2000 && uuid < 3000)  // 系统设置区
            tmpSet[uuid] = query.value(1).toString();
    }
    query.clear();
    return Qt::Key_Away;
}

int AppWindow::readModels()
{
    QSqlQuery query(QSqlDatabase::database("sqlite"));
    int r = tmpSet[DataFile].toInt();
    QString type = tmpSet[r].toString();
    qDebug() << "app read:" << type;
    query.exec(QString("select * from M_%1").arg(type));
    while (query.next()) {
        int uuid = query.value(0).toInt();
        if (uuid >= 3000 && uuid < 4000)  // 型号参数区
            tmpSet[uuid] = query.value(1).toString();
    }
    query.clear();
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
    //    connect(tcp, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));

    //    tmpMap.insert("hostaddr", "s.aipuo.com");
    //    tmpMap.insert("hostport", "6000");
    //    tmpMap.insert("devNumb", tmpSet.value(AddrSoft).toString());
    //    tmpMap.insert("version", tmpSet.value(AddrSoft).toString());
    //    tcp->connectToServer(tmpMap);
    //    tmpMap.clear();

    UdpSocket *udp = new UdpSocket(this);
    udp->setObjectName("socket");
    connect(udp, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    connect(this, SIGNAL(sendUdpMsg(QTmpMap)), udp, SLOT(recvAppMsg(QTmpMap)));
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
    QString name = "sqlite";
    QSqlQuery query(QSqlDatabase::database(name));
    QList<int> uuids = tmpSet.keys();
    QSqlDatabase::database(name).transaction();
    for (int i=0; i < uuids.size(); i++) {
        int uuid = uuids.at(i);
        if (uuid < 3000 && uuid >= 2000) {
            query.prepare("replace into G_DEFAULT values(?,?)");
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
    boxbar->setLabelText(tr("正在保存数据"));
    boxbar->show();
    wait(10);
    QString name = "sqlite";
    QSqlQuery query(QSqlDatabase::database(name));
    QList<int> uuids = tmpSet.keys();
    QSqlDatabase::database(name).transaction();
    QString type = tmpSet[tmpSet[DataFile].toInt()].toString();
    for (int i=0; i < uuids.size(); i++) {
        int uuid = uuids.at(i);
        if (uuid < 4000 && uuid >= 3000) {
            query.prepare(QString("replace into M_%1 values(?,?)").arg(type));
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
    qDebug() << "app save:" << type;
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
        qDebug() << "app task:" << currTask;
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
    return Qt::Key_Away;
}

int AppWindow::taskStartTest()
{
    int ret = testThread();
    return ret;
}

int AppWindow::taskStartSave()
{
    qDebug() <<"app time:" << t.elapsed() << "ms";
    return Qt::Key_Away;
}

int AppWindow::taskStartBeep()
{
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
    timeOut++;
    int r = tmpSet[AddrSyst].toInt();
    int t = tmpSet[r + (tmpSet[DataOKNG].toInt() == DataOK ? 0x07 : 0x08)].toDouble()*100;
    if (timeOut > t) {
        timeOut = 0;
        ret = Qt::Key_Away;
        tmpMsg.insert(AddrEnum, Qt::Key_Call);
        tmpMsg.insert(AddrText, tmpSet[DataOKNG].toInt() == DataOK ? "LEDG" : "LEDR");
        tmpMsg.insert(AddrBeep, 0);
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
    }
    return ret;
}

int AppWindow::testThread()
{
    int ret = Qt::Key_Meta;
    if (testMap.keys().contains(QString::number(currTest))) {
        if ((this->*testMap[QString::number(currTest)])() == Qt::Key_Away) {
            qDebug() << "app test:" << currTest;
            currTest++;
            currTest = (currTest >= testMap.size()) ? 0 : currTest;  // 测试流程
            if (currTest == 0) {
                currItem = getNextItem();
                if (currItem == 0)
                    ret = Qt::Key_Away;
                qDebug() << "app next:" << currItem;
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
    t.restart();
#ifdef __arm__
    emit sendUdpMsg(msg);
#endif
    qDebug() << "udp time:" << t.elapsed();
    t.restart();
    int addr = msg.value(AddrText).toInt();
    QList <int> keys = msg.keys();
    int s = tmpSet[tmpSet[AddrIMPW].toInt()].toInt();
    switch (addr) {
    case AddrModel:
        break;
    case AddrDCRS1:
    case AddrDCRS2:
    case AddrDCRS3:
        if (msg[DataDCRS].toInt() == 2) {
            testShift = Qt::Key_Away;
            for (int i=0; i < keys.size(); i++) {
                if (keys.at(i) < s)
                    tmpSet[keys.at(i)] = msg[keys.at(i)];
            }
        }
        break;
    case AddrACWS1:
    case AddrACWS2:
    case AddrACWS3:
    case AddrACWS4:
        if (msg[DataACWS].toInt() == 0) {
            testShift = Qt::Key_Away;
            for (int i=0; i < keys.size(); i++) {
                if (keys.at(i) < s)
                    tmpSet[keys.at(i)] = msg[keys.at(i)];
            }
        }
        break;
    case AddrIMPS1:
        if (msg[DataIMPS].toInt() == 0) {
            testShift = Qt::Key_Away;
            for (int i=0; i < keys.size(); i++) {
                if (keys.at(i) < s)
                    tmpSet[keys.at(i)] = msg[keys.at(i)];
            }
        }
        break;
    default:
        break;
    }
    if (!msg.value(DataOKNG).isNull()) {
        tmpSet[DataOKNG] = msg.value(DataOKNG);
    }
    emit sendAppMsg(msg);
    qDebug() << "tst time:" << t.elapsed();
}

void AppWindow::recvTmpMsg(QTmpMap msg)
{
#ifdef __arm__
    emit sendUdpMsg(msg);
#endif
    emit sendAppMsg(msg);
}

void AppWindow::recvAppMsg(QTmpMap msg)
{
#ifndef __arm__
    if (sender()->objectName() != "socket")
        emit sendUdpMsg(msg);
#endif
    int c = msg.value(0).toInt();
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
        qDebug() << "app play:";
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
        readModels();
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
