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
QMap<QString, pClass> initMap;
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

    this->setWindowTitle(tr("电枢测试仪%1").arg(verNumb));
    return Qt::Key_Enter;
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
    return Qt::Key_Enter;
}

int AppWindow::initAuthor()
{
    QString name = "author";
    AppAuthor *app = new AppAuthor(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    connect(this, SIGNAL(sendAppMap(QVariantMap)), app, SLOT(recvAppMap(QVariantMap)));
    stack->addWidget(app);

    initButton(tr("返回主页"), name);
    return Qt::Key_Enter;
}

int AppWindow::initDevice()
{
#ifdef __arm__
    DevSerial *key = new DevSerial(this);
    connect(key, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    connect(this, SIGNAL(sendAppMap(QVariantMap)), key, SLOT(recvAppMap(QVariantMap)));

    DevSetRtc *rtc = new DevSetRtc(this);
    connect(rtc, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    connect(this, SIGNAL(sendAppMap(QVariantMap)), rtc, SLOT(recvAppMap(QVariantMap)));

    DevBuzzer *pwm = new DevBuzzer(this);
    connect(pwm, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    connect(this, SIGNAL(sendAppMap(QVariantMap)), pwm, SLOT(recvAppMap(QVariantMap)));

    DevScreen *lcd = new DevScreen(this);
    connect(lcd, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    connect(this, SIGNAL(sendAppMap(QVariantMap)), lcd, SLOT(recvAppMap(QVariantMap)));

    DevSetCan *can = new DevSetCan(this);
    connect(can, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    connect(this, SIGNAL(sendAppMap(QVariantMap)), can, SLOT(recvAppMap(QVariantMap)));
#endif
    return Qt::Key_Enter;
}

int AppWindow::initScreen()
{
    boxbar = new BoxDialog(this);
    boxbar->setValue(0);
    boxbar->show();

    QStringList names;
    names << tr("正在初始化数据记录") << tr("正在初始化测试界面") << tr("正在初始化登录界面");
    initMap[names.at(names.size() - 3)] = &AppWindow::initSqlDir;
    initMap[names.at(names.size() - 2)] = &AppWindow::initTester;
    initMap[names.at(names.size() - 1)] = &AppWindow::initSignin;
    names << tr("正在初始化系统设置") << tr("正在初始化用户管理") << tr("正在初始化权限管理");
    initMap[names.at(names.size() - 3)] = &AppWindow::initSystem;
    initMap[names.at(names.size() - 2)] = &AppWindow::initMaster;
    initMap[names.at(names.size() - 1)] = &AppWindow::initAction;
    names << tr("正在初始化后台设置") << tr("正在初始化调试信息") << tr("正在初始化型号管理");
    initMap[names.at(names.size() - 3)] = &AppWindow::initBackup;
    initMap[names.at(names.size() - 2)] = &AppWindow::initLogger;
    initMap[names.at(names.size() - 1)] = &AppWindow::initConfig;
    names << tr("正在初始化电阻配置") << tr("正在初始化高压配置") << tr("正在初始化匝间配置");
    initMap[names.at(names.size() - 3)] = &AppWindow::initSetDcr;
    initMap[names.at(names.size() - 2)] = &AppWindow::initSetAcw;
    initMap[names.at(names.size() - 1)] = &AppWindow::initSetImp;
    names << tr("正在初始化电阻调试") << tr("正在初始化高压调试") << tr("正在初始化匝间调试");
    initMap[names.at(names.size() - 3)] = &AppWindow::initOffDcr;
    initMap[names.at(names.size() - 2)] = &AppWindow::initOffAcw;
    initMap[names.at(names.size() - 1)] = &AppWindow::initOffImp;
    names << tr("正在初始化数据管理") << tr("正在初始化上传管理") << tr("正在初始化历史数据");
    initMap[names.at(names.size() - 3)] = &AppWindow::initRecord;
    initMap[names.at(names.size() - 2)] = &AppWindow::initUpload;
    initMap[names.at(names.size() - 1)] = &AppWindow::initSdcard;
    names << tr("正在初始化数据分析") << tr("正在读取配置数据库") << tr("正在读取型号数据库");
    initMap[names.at(names.size() - 3)] = &AppWindow::initUnqual;
    initMap[names.at(names.size() - 2)] = &AppWindow::readSqlite;
    initMap[names.at(names.size() - 1)] = &AppWindow::readModels;
    names << tr("正在分发数据库配置") << tr("正在初始化网络连接") << tr("正在初始化登录验证");
    initMap[names.at(names.size() - 3)] = &AppWindow::sendSqlite;
    initMap[names.at(names.size() - 2)] = &AppWindow::initSocket;
    initMap[names.at(names.size() - 1)] = &AppWindow::sendSignin;
    names << tr("正在初始化系统线程");
    initMap[names.at(names.size() - 1)] = &AppWindow::initThread;
    for (int i=0; i < names.size(); i++) {
        showBoxPop(names.at(i), i);
    }
    btnLayout->addStretch();
    return Qt::Key_Enter;
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
    return Qt::Key_Enter;
}

int AppWindow::initTester()
{
    QString name = "tester";
    AppTester *app = new AppTester(this);
    app->setObjectName("tester");
    connect(app, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    connect(this, SIGNAL(sendAppMap(QVariantMap)), app, SLOT(recvAppMap(QVariantMap)));
    stack->addWidget(app);

    initButton(tr("返回测试"), name);
    return Qt::Key_Enter;
}

int AppWindow::initSignin()
{
    isSignin = false;
    QString name = "signin";
    AppSignin *app = new AppSignin(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    connect(this, SIGNAL(sendAppMap(QVariantMap)), app, SLOT(recvAppMap(QVariantMap)));
    stack->addWidget(app);

    initButton(tr("用户登录"), name);
    return Qt::Key_Enter;
}

int AppWindow::initSystem()
{
    QString name = "system";
    AppSystem *app = new AppSystem(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    connect(this, SIGNAL(sendAppMap(QVariantMap)), app, SLOT(recvAppMap(QVariantMap)));
    stack->addWidget(app);

    initButton(tr("系统设置"), name);
    return Qt::Key_Enter;
}

int AppWindow::initMaster()
{
    QString name = "master";
    AppMaster *app = new AppMaster(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    connect(this, SIGNAL(sendAppMap(QVariantMap)), app, SLOT(recvAppMap(QVariantMap)));
    stack->addWidget(app);

    initButton(tr("用户管理"), name);
    return Qt::Key_Enter;
}

int AppWindow::initAction()
{
    QString name = "action";
    AppAction *app = new AppAction(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    connect(this, SIGNAL(sendAppMap(QVariantMap)), app, SLOT(recvAppMap(QVariantMap)));
    stack->addWidget(app);

    initButton(tr("权限管理"), name);
    return Qt::Key_Enter;
}

int AppWindow::initBackup()
{
    QString name = "backup";
    AppBackup *app = new AppBackup(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    connect(this, SIGNAL(sendAppMap(QVariantMap)), app, SLOT(recvAppMap(QVariantMap)));
    stack->addWidget(app);

    initButton(tr("后台管理"), name);
    return Qt::Key_Enter;
}

int AppWindow::initLogger()
{
    QString name = "logger";
    AppLogger *logger = AppLogger::instance();
    logger->setObjectName(name);
    connect(logger, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    connect(this, SIGNAL(sendAppMap(QVariantMap)), logger, SLOT(recvAppMap(QVariantMap)));
    stack->addWidget(logger);

    initButton(tr("调试信息"), name);
    return Qt::Key_Enter;
}

int AppWindow::initConfig()
{
    QString name = "config";
    TypConfig *app = new TypConfig(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    connect(this, SIGNAL(sendAppMap(QVariantMap)), app, SLOT(recvAppMap(QVariantMap)));
    stack->addWidget(app);

    initButton(tr("型号管理"), name);
    return Qt::Key_Enter;
}

int AppWindow::initSetDcr()
{
    QString name = "setdcr";
    TypSetDcr *app = new TypSetDcr(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    stack->addWidget(app);

    initButton(tr("电阻配置"), name);
    return Qt::Key_Enter;
}

int AppWindow::initSetAcw()
{
    QString name = "setacw";
    TypSetAcw *app = new TypSetAcw(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    connect(this, SIGNAL(sendAppMap(QVariantMap)), app, SLOT(recvAppMap(QVariantMap)));
    stack->addWidget(app);

    initButton(tr("介电强度"), name);
    return Qt::Key_Enter;
}

int AppWindow::initSetImp()
{
    QString name = "setimp";
    TypSetImp *app = new TypSetImp(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    stack->addWidget(app);

    initButton(tr("匝间配置"), name);
    return Qt::Key_Enter;
}

int AppWindow::initOffDcr()
{
    QString name = "offdcr";
    TypOffDcr *app = new TypOffDcr(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    stack->addWidget(app);

    initButton(tr("电阻调试"), name);
    return Qt::Key_Enter;
}

int AppWindow::initOffAcw()
{
    QString name = "offacw";
    TypOffAcw *app = new TypOffAcw(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    stack->addWidget(app);

    initButton(tr("绝缘调试"), name);
    return Qt::Key_Enter;
}

int AppWindow::initOffImp()
{
    QString name = "offimp";
    TypOffImp *app = new TypOffImp(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    stack->addWidget(app);

    initButton(tr("匝间调试"), name);
    return Qt::Key_Enter;
}

int AppWindow::initRecord()
{
    QString name = "record";
    SqlRecord *app = new SqlRecord(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    stack->addWidget(app);

    initButton(tr("数据管理"), name);
    return Qt::Key_Enter;
}

int AppWindow::initUpload()
{
    QString name = "upload";
    SqlUpload *app = new SqlUpload(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    connect(this, SIGNAL(sendAppMap(QVariantMap)), app, SLOT(recvAppMap(QVariantMap)));
    stack->addWidget(app);

    initButton(tr("数据上传"), name);
    return Qt::Key_Enter;
}

int AppWindow::initSdcard()
{
    QString name = "sdcard";
    SqlSdcard *app = new SqlSdcard(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    stack->addWidget(app);

    initButton(tr("数据历史"), name);
    return Qt::Key_Enter;
}

int AppWindow::initUnqual()
{
    QString name = "unqual";
    SqlUnqual *app = new SqlUnqual(this);
    app->setObjectName(name);
    //    connect(unqual, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    stack->addWidget(app);

    initButton(tr("数据分析"), name);
    return Qt::Key_Enter;
}

int AppWindow::readSqlite()
{
    QString name = "sqlite";
    qDebug() << "app read:" << name;
    QSqlQuery query(QSqlDatabase::database(name));
    query.exec("select * from G_DEFAULT");
    while (query.next()) {
        QString uuid = query.value(0).toString();
        QString parm = query.value(1).toString();
        config[uuid] = parm;
    }
    query.clear();
    config[QString::number(AddrVerNub)] = verNumb;
    return Qt::Key_Enter;
}

int AppWindow::readModels()
{
    QSqlQuery query(QSqlDatabase::database("sqlite"));
    QString type = config[QString::number(AddrConfig)].toString();
    qDebug() << "app read:" << type;
    query.exec(QString("select * from M_%1").arg(type));
    while (query.next()) {
        QString uuid = query.value(0).toString();
        QString parm = query.value(1).toString();
        config[uuid] = parm;
    }
    query.clear();
    return Qt::Key_Enter;
}

int AppWindow::sendSqlite()
{
    sources.clear();  // 界面名称
    sgroups.clear();  // 界面组别
    saction.clear();  // 界面权限
    masters.clear();  // 用户名称
    maction.clear();  // 用户权限
    for (int i=0; i < 0x0100; i+=4) {  // 查询界面组别与权限
        masters.append(config[QString::number(i+AddrMaster+0x00)].toString());
        maction.append(config[QString::number(i+AddrMaster+0x02)].toInt());
        sources.append(config[QString::number(i+AddrAction+0x00)].toString());
        sgroups.append(config[QString::number(i+AddrAction+0x02)].toInt());
        saction.append(config[QString::number(i+AddrAction+0x03)].toInt());
    }

    config["0"] = verNumb;
    config["1"] = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    config.insert("enum", Qt::Key_Option);
    emit sendAppMap(config);
    config.remove("enum");
    return Qt::Key_Enter;
}

int AppWindow::sendSignin()
{
    tmpMap.insert("enum", Qt::Key_Enter);
    emit sendAppMap(tmpMap);
    tmpMap.clear();

    if (isSignin) {
        QTimer::singleShot(500, this, SLOT(showTester()));
    } else {
        tmpMap.insert("enum", Qt::Key_Display);
        tmpMap.insert("text", "signin");
        recvAppMap(tmpMap);
        tmpMap.clear();
    }
    return Qt::Key_Enter;
}

int AppWindow::initSocket()
{
    TcpSocket *tcp = new TcpSocket(this);
    connect(tcp, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));

    tmpMap.insert("hostaddr", "s.aipuo.com");
    tmpMap.insert("hostport", "6000");
    tmpMap.insert("devNumb", config[QString::number(AddrNumber)].toString());
    tmpMap.insert("version", config[QString::number(AddrVerNub)].toString());
    tcp->connectToServer(tmpMap);
    tmpMap.clear();

    UdpSocket *udp = new UdpSocket(this);
    connect(udp, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    connect(this, SIGNAL(sendUdpMap(QVariantMap)), udp, SLOT(recvAppMap(QVariantMap)));
    return Qt::Key_Enter;
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

    addrList << AddrAG << AddrAC;

    QTimer *tasks = new QTimer(this);
    connect(tasks, SIGNAL(timeout()), this, SLOT(taskThread()));
    tasks->start(10);

    currTask = 0;
    currTest = 0;

    return Qt::Key_Enter;
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
    (this->*initMap[text])();
    boxbar->setValue((t+1)*100/initMap.size());
}

void AppWindow::saveSqlite()
{
    boxbar->setLabelText(tr("正在保存数据"));
    boxbar->show();
    wait(10);
    QString name = "sqlite";
    QSqlQuery query(QSqlDatabase::database(name));
    QStringList uuids = config.keys();
    QSqlDatabase::database(name).transaction();
    for (int i=0; i < uuids.size(); i++) {
        int uuid = uuids.at(i).toInt();
        if (uuid < 0x0400 && uuid >= 0x0010) {
            query.prepare("replace into G_DEFAULT values(?,?)");
            query.addBindValue(uuid);
            query.addBindValue(config[QString::number(uuid)].toString());
            query.exec();
        }
        boxbar->setValue(i*100/uuids.size());
    }
    QSqlDatabase::database(name).commit();
    boxbar->setValue(99);
    wait(500);
    query.clear();
    boxbar->setValue(100);
    qDebug() << "app save:" << name;
}

void AppWindow::saveModels()
{
    boxbar->setLabelText(tr("正在保存数据"));
    boxbar->show();
    wait(10);
    QString name = "sqlite";
    QSqlQuery query(QSqlDatabase::database(name));
    QSqlDatabase::database(name).transaction();
    QStringList uuids = config.keys();
    QString type = config[QString::number(AddrConfig)].toString();
    for (int i=0; i < uuids.size(); i++) {
        int uuid = uuids.at(i).toInt();
        if (uuid >= 0x0400) {
            query.prepare(QString("replace into M_%1 values(?,?)").arg(type));
            query.addBindValue(uuid);
            query.addBindValue(config[QString::number(uuid)].toString());
            query.exec();
        }
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
    tmpMap.insert("enum", Qt::Key_Display);
    tmpMap.insert("text", sender()->objectName());
    recvAppMap(tmpMap);
    tmpMap.clear();
}

void AppWindow::showTester()
{
    tmpMap.insert("enum", Qt::Key_Display);
    tmpMap.insert("text", "tester");
    recvAppMap(tmpMap);
    tmpMap.clear();
}

bool AppWindow::checkAction(QString msg)
{
    QString currMaster = config[QString::number(AddrSignIn)].toString();  // 当前用户
    int currAction = (isSignin) ? maction.at(masters.indexOf(currMaster)) : 4;  // 当前权限
    int currGroups = sgroups.at(sources.indexOf(msg));  // 当前界面组别
    int showAction = saction.at(sources.indexOf(msg));  // 当前界面权限

    if (currGroups > 0) {
        for (int i=0; i < buttons.size(); i++) {  // 显示相同组别按钮
            int a = saction.at(sources.indexOf(buttons.at(i)->objectName()));
            int g = sgroups.at(sources.indexOf(buttons.at(i)->objectName()));
            if ((currAction <= a && currGroups == g) || g == 0) {
                buttons.at(i)->show();
            } else {
                buttons.at(i)->hide();
            }
            if (buttons.at(i)->objectName() == msg) {
                buttons.at(i)->setChecked(true);
            } else {
                buttons.at(i)->setChecked(false);
            }
        }
        btnFrame->show();
    } else {
        btnFrame->hide();
    }
    if (currAction > showAction)
        return false;
    tmpMap.insert("enum", currAction < showAction ? Qt::Key_Less : Qt::Key_Equal);
    emit sendAppMap(tmpMap);
    tmpMap.clear();
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
        tmpMap.insert("enum", Qt::Key_Play);
        tmpMap.insert("text", AddrSC);
        emit sendAppMap(tmpMap);
        tmpMap.clear();
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
    int ret = Qt::Key_Return;
    if (taskMap.keys().contains(QString::number(currTask)))
        ret = (this->*taskMap[QString::number(currTask)])();
    if (ret == Qt::Key_Enter) {
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
    testIndex = addrList.at(0);
    taskShift = Qt::Key_Sleep;
    testShift = Qt::Key_Sleep;
    return Qt::Key_Enter;
}

int AppWindow::taskCheckPlay()
{
    int ret = Qt::Key_Return;
    if (taskShift == Qt::Key_Play) {
        if (stack->currentWidget()->objectName() != "tester") {
            taskClearData();
        } else {
            ret = Qt::Key_Enter;
        }
    }
    return ret;
}

int AppWindow::taskCheckSafe()
{
    return Qt::Key_Enter;
}

int AppWindow::taskCheckCode()
{
    return Qt::Key_Enter;
}

int AppWindow::taskStartView()
{
    tmpMap.insert("enum", Qt::Key_View);
    tmpMap.insert("text", "LEDY");
    emit sendAppMap(tmpMap);
    tmpMap.insert("enum", Qt::Key_WakeUp);
    emit sendAppMap(tmpMap);
    tmpMap.clear();
    return Qt::Key_Enter;
}

int AppWindow::taskStartTest()
{
    int ret = testThread();
    return ret;
}

int AppWindow::taskStartSave()
{
    return Qt::Key_Enter;
}

int AppWindow::taskStartBeep()
{
    tmpMap.insert("enum", Qt::Key_View);
    tmpMap.insert("text", "LEDG");
    tmpMap.insert("data", 100);
    emit sendAppMap(tmpMap);
    return Qt::Key_Enter;
}

int AppWindow::taskClearBeep()
{
    int ret = Qt::Key_Return;
    timeOut++;
    int t = config[QString::number(AddrTimeOK)].toDouble()*100;
    if (timeOut > t) {
        timeOut = 0;
        ret = Qt::Key_Enter;
        tmpMap.insert("enum", Qt::Key_View);
        tmpMap.insert("text", "LEDG");
        tmpMap.insert("data", 0);
        emit sendAppMap(tmpMap);
        tmpMap.insert("enum", Qt::Key_Sleep);
        emit sendAppMap(tmpMap);
        tmpMap.clear();
    }
    return ret;
}

int AppWindow::testThread()
{
    int ret = Qt::Key_Return;
    if (testMap.keys().contains(QString::number(currTest))) {
        if ((this->*testMap[QString::number(currTest)])() == Qt::Key_Enter) {
            qDebug() << "app test:" << currTest;
            currTest++;
            currTest = (currTest >= testMap.size()) ? 0 : currTest;
            if (currTest == 0) {
                int r = addrList.indexOf(testIndex);
                if (r + 1 == addrList.size()) {
                    ret = Qt::Key_Enter;
                } else {
                    testIndex = addrList.at(r+1);
                }
            }
        }
    }
    return ret;
}

int AppWindow::testClearData()
{
    testShift = Qt::Key_Sleep;
    timeTst = 0;
    return Qt::Key_Enter;
}

int AppWindow::testStartSend()
{
    tmpMap.insert("enum", Qt::Key_Play);
    tmpMap.insert("text", testIndex);
    emit sendAppMap(tmpMap);
    tmpMap.clear();
    return Qt::Key_Enter;
}

int AppWindow::testStartTest()
{
    //    return Qt::Key_Enter;
    int ret = Qt::Key_Return;
    if (testShift == Qt::Key_Enter) {
        ret = Qt::Key_Enter;
    }
    return ret;
}

void AppWindow::testUpdate(QVariantMap msg)
{
#ifdef __arm__
    emit sendUdpMap(msg);
#endif
    emit sendAppMap(msg);
    if (msg[QString::number(msg.value("text").toInt() + AddrHS)].toInt() == 0)
        testShift = Qt::Key_Enter;
    QStringList keys = msg.keys();
    keys.removeOne("enum");
    keys.removeOne("text");
    for (int i=0; i < keys.size(); i++) {
        config[keys.at(i)] = msg[keys.at(i)];
    }
}

void AppWindow::recvAppMap(QVariantMap msg)
{
#ifndef __arm__
    switch (msg.value("enum").toInt()) {
    case Qt::Key_Refresh:
        break;
    default:
        emit sendUdpMap(msg);
        break;
    }
#endif
    switch (msg.value("enum").toInt()) {
    case Qt::Key_Display:
        recvAppShow(msg.value("text").toString());
        break;
    case Qt::Key_Enter:
        isSignin = true;
        break;
    case Qt::Key_LogOff:
        isSignin = false;
        recvAppShow("signin");
        break;
    case Qt::Key_Save:
        msg.remove("enum");
        for (int i=0; i < msg.keys().size(); i++) {
            config[msg.keys().at(i)] = msg[msg.keys().at(i)];
        }
        saveSqlite();
        sendSqlite();
        break;
    case Qt::Key_Option:
        msg.remove("enum");
        for (int i=0; i < msg.keys().size(); i++) {
            config[msg.keys().at(i)] = msg[msg.keys().at(i)];
        }
        saveModels();
        sendSqlite();
        break;
        break;
    case Qt::Key_Reload:
        config[QString::number(AddrConfig)] = msg[QString::number(AddrConfig)];
        saveSqlite();
        readModels();
        sendSqlite();
        break;
    case Qt::Key_Stop:
        taskClearData();
        tmpMap.insert("enum", Qt::Key_View);
        tmpMap.insert("text", "LED1");
        tmpMap.insert("data", 0);
        emit sendAppMap(tmpMap);
        tmpMap.clear();
        //        tmpMap.insert("enum", Qt::Key_BrightnessAdjust);
        //        tmpMap.insert("data", 100);
        //        emit sendAppMap(tmpMap);
        //        tmpMap.clear();
        break;
    case Qt::Key_Play:
        taskShift = Qt::Key_Play;
        break;
    case Qt::Key_Time:
        emit sendAppMap(msg);
        break;
    case Qt::Key_Community:
        emit sendAppMap(msg);
        break;
    case Qt::Key_Refresh:
        testUpdate(msg);
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
