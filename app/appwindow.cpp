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
QMap<int, pClass> taskMap;
QMap<int, pClass> testMap;

AppWindow::AppWindow(QWidget *parent) : QMainWindow(parent)
{
    t.start();
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

    QDate makeDate;
    makeDate.setDate(year, month, day);
    static const QTime makeTime = QTime::fromString(__TIME__, "hh:mm:ss");

    QDateTime mDateTime(makeDate, makeTime);
    verNumb = QString("V-2.2.%1").arg(mDateTime.toString("yyMMdd-hhmm"));
    QString ttt = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    qWarning() << "app vern:" << verNumb << ttt;
    this->setWindowTitle(tr("电枢转子测试仪%1").arg(verNumb));
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
    btnFrame->setFixedWidth(BTN_WIDTH);
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
    AppAuthor *app = new AppAuthor(this);
    initWidget(0x00, 0, "author", tr("返回主页"), app);
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

    DevDS1302 *iic = new DevDS1302(this);
    connect(this, SIGNAL(sendAppMsg(QTmpMap)), iic, SLOT(recvAppMsg(QTmpMap)));
    connect(iic, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
#endif
    scanner = new QTimer(this);
    connect(scanner, SIGNAL(timeout()), this, SLOT(showBarCode()));
    return Qt::Key_Away;
}

int AppWindow::  initScreen()
{
    isChange = false;
    boxbar = new BoxDialog(this);
    boxbar->setValue(0);
    boxbar->show();

    boxSta = new BoxStatus(this);

    QStringList names;
    initMap[names.size()] = &AppWindow::initTester;
    names << tr("正在初始化测试界面");
    initMap[names.size()] = &AppWindow::initSignin;
    names << tr("正在初始化登录界面");
    initMap[names.size()] = &AppWindow::initSystem;
    names << tr("正在初始化系统设置");
    initMap[names.size()] = &AppWindow::initMaster;
    names << tr("正在初始化用户管理");
    initMap[names.size()] = &AppWindow::initPermit;
    names << tr("正在初始化权限管理");
    initMap[names.size()] = &AppWindow::initRT3070;
    names << tr("正在初始化无线网络");
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
    initMap[names.size()] = &AppWindow::initExport;
    names << tr("正在初始化数据导出");
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
    AppTester *app = new AppTester(this);
    initWidget(0x01, 0, "tester", tr("测试界面"), app);
    return Qt::Key_Away;
}

int AppWindow::initSignin()
{
    AppSignin *app = new AppSignin(this);
    initWidget(0x00, 1, "signin", tr("用户登录"), app);
    return Qt::Key_Away;
}

int AppWindow::initSystem()
{
    AppSystem *app = new AppSystem(this);
    initWidget(0x01, 1, "system", tr("系统设置"), app);
    return Qt::Key_Away;
}

int AppWindow::initMaster()
{
    AppMaster *app = new AppMaster(this);
    initWidget(0x03, 1, "master", tr("用户管理"), app);
    return Qt::Key_Away;
}

int AppWindow::initPermit()
{
    AppPermit *app = new AppPermit(this);
    initWidget(0x04, 1, "permit", tr("权限管理"), app);
    return Qt::Key_Away;
}

int AppWindow::initRT3070()
{
#ifdef __arm__
    AppRT3070 *app = new AppRT3070(this);
    initWidget(0x07, 1, "rt3070", tr("无线网络"), app);
#endif
    return Qt::Key_Away;
}

int AppWindow::initBackup()
{
    AppBackup *app = new AppBackup(this);
    initWidget(0x05, 1, "backup", tr("后台管理"), app);
    return Qt::Key_Away;
}

int AppWindow::initLogger()
{
    AppLogger *app = AppLogger::instance();
    initWidget(0x06, 1, "logger", tr("调试信息"), app);
    return Qt::Key_Away;
}

int AppWindow::initConfig()
{
    TypConfig *app = new TypConfig(this);
    initWidget(0x00, 2, "config", tr("型号管理"), app);
    return Qt::Key_Away;
}

int AppWindow::initSetDcr()
{
    TypSetDcr *app = new TypSetDcr(this);
    initWidget(0x01, 2, "setdcr", tr("电阻配置"), app);
    return Qt::Key_Away;
}

int AppWindow::initSetAcw()
{
    TypSetAcw *app = new TypSetAcw(this);
    initWidget(0x02, 2, "setacw", tr("耐压配置"), app);
    return Qt::Key_Away;
}

int AppWindow::initSetImp()
{
    TypSetImp *app = new TypSetImp(this);
    initWidget(0x03, 2, "setimp", tr("匝间配置"), app);
    return Qt::Key_Away;
}

int AppWindow::initOffDcr()
{
    TypOffDcr *app = new TypOffDcr(this);
    initWidget(0x04, 2, "offdcr", tr("电阻调试"), app);
    return Qt::Key_Away;
}

int AppWindow::initOffAcw()
{
    TypOffAcw *app = new TypOffAcw(this);
    initWidget(0x05, 2, "offacw", tr("耐压调试"), app);
    return Qt::Key_Away;
}

int AppWindow::initOffImp()
{
    TypOffImp *app = new TypOffImp(this);
    initWidget(0x06, 2, "offimp", tr("匝间调试"), app);
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

int AppWindow::initExport()
{
    SqlExport *app = new SqlExport;
    connect(app, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    connect(this, SIGNAL(sendAppMsg(QTmpMap)), app, SLOT(recvAppMsg(QTmpMap)));
    app->moveToThread(sql);

    return Qt::Key_Away;
}

int AppWindow::initRecord()
{
    SqlRecord *app = new SqlRecord(this);
    initWidget(0x00, 3, "record", tr("数据管理"), app);
    return Qt::Key_Away;
}

int AppWindow::initUpload()
{
    SqlUpload *app = new SqlUpload(this);
    initWidget(0x01, 3, "upload", tr("数据上传"), app);
    return Qt::Key_Away;
}

int AppWindow::initSdcard()
{
    SqlSdcard *app = new SqlSdcard(this);
    initWidget(0x02, 3, "sdcard", tr("数据历史"), app);
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
    query.exec("select name from sqlite_master where type='table' order by name");
    QTmpMap tmptyp;
    while (query.next()) {
        QString t = query.value(0).toString();
        QString numb = t.mid(1, 4);
        tmptyp.insert(numb.toInt(), t.mid(6, NAME_SIZE));
    }
    int r = tmpSet.value(DataFile).toInt();
    QString name = tmptyp.value(r).toString();
    tmpSet.insert(DataType, name);
    query.exec(QString("select * from '%1'").arg(tr("T%1_%2").arg(r, 4, 10, QChar('0')).arg(name)));
    while (query.next()) {
        int uuid = query.value(0).toInt();
        tmpSet.insert(uuid, query.value(1).toString());
    }
    query.clear();
    qDebug() << "app read:" << name;
    return Qt::Key_Away;
}

int AppWindow::sendSqlite()
{
    tmpSet.insert(Qt::Key_0, Qt::Key_Copy);
    tmpSet.insert(DataSoft, verNumb);
    emit sendAppMsg(tmpSet);

    for (int i=0; i < bufwin.size(); i++) {
        tmpMsg.insert(i, bufwin.at(i));
    }
    tmpMsg.insert(Qt::Key_0, Qt::Key_Memo);
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();

    return Qt::Key_Away;
}

int AppWindow::sendSignin()
{
    tmpMsg.insert(Qt::Key_0, Qt::Key_Game);
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();

    int temp = tmpSet.value((3000 + Qt::Key_0)).toInt();  // 临时数据地址
    if (tmpSet.value(temp + TEMPSIGN).toInt() == 1) {
        QPushButton *btn = NULL;
        for (int i=0; i < buttons.size(); i++) {
            if (buttons.at(i)->objectName() == "tester") {
                btn = buttons.at(i);
                QTimer::singleShot(500, btn, SLOT(click()));
                break;
            }
        }
    }
    return Qt::Key_Away;
}

int AppWindow::initSocket()
{
    TcpSocket *tcp = new TcpSocket;
    tcp->setObjectName("socket");
    connect(tcp, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    connect(this, SIGNAL(sendNetMsg(QTmpMap)), tcp, SLOT(recvAppMsg(QTmpMap)));
    tcp->connectToServer(tmpSet);
    tcp->moveToThread(sql);

    return Qt::Key_Away;
}

int AppWindow::initThread()
{
    taskMap.clear();
    taskMap.insert(0x00, &AppWindow::taskClearData);
    taskMap.insert(0x01, &AppWindow::taskCheckPlay);
    taskMap.insert(0x02, &AppWindow::taskCheckSafe);
    taskMap.insert(0x03, &AppWindow::taskCheckCode);
    taskMap.insert(0x04, &AppWindow::taskStartView);
    taskMap.insert(0x05, &AppWindow::taskStartWait);
    taskMap.insert(0x06, &AppWindow::taskStartTest);
    taskMap.insert(0x07, &AppWindow::taskStartSave);
    taskMap.insert(0x08, &AppWindow::taskStartBeep);
    taskMap.insert(0x09, &AppWindow::taskResetTest);

    testMap.clear();
    testMap.insert(0x00, &AppWindow::testClearData);
    testMap.insert(0x01, &AppWindow::testStartSend);
    testMap.insert(0x02, &AppWindow::testStartTest);

    QTimer *tasks = new QTimer(this);
    connect(tasks, SIGNAL(timeout()), this, SLOT(taskThread()));
    tasks->start(10);

    currTask = 0;
    currTest = 0;
    station = 0x11;

    return Qt::Key_Away;
}

int AppWindow::initWidget(int numb, int form, QString name, QString mark, QWidget *app)
{
    tmpMap.insert("numb", numb);
    tmpMap.insert("form", form);
    tmpMap.insert("name", name);
    tmpMap.insert("mark", mark);
    bufwin.append(tmpMap);
    tmpMap.clear();

    app->setObjectName(name);
    stack->addWidget(app);
    connect(app, SIGNAL(sendAppMsg(QTmpMap)), this, SLOT(recvAppMsg(QTmpMap)));
    connect(this, SIGNAL(sendAppMsg(QTmpMap)), app, SLOT(recvAppMsg(QTmpMap)));

    QPushButton *btnTester = new QPushButton(mark, this);
    btnTester->setFlat(true);
    btnTester->setCheckable(true);
    btnTester->setFixedSize(BTN_WIDTH, BTN_LENTH);
    btnTester->setFocusPolicy(Qt::NoFocus);
    buttons.append(btnTester);
    btnLayout->addWidget(btnTester);
    btnTester->setObjectName(name);
    connect(btnTester, SIGNAL(clicked(bool)), this, SLOT(clickButtons()));
    return Qt::Key_Away;
}

void AppWindow::showBoxPop(QString text, int t)
{
    boxbar->setLabelText(text);
    wait(10);
    (this->*initMap[t])();
    boxbar->setValue((t+1)*100/initMap.size());
}

void AppWindow::saveBackup(QTmpMap msg)
{
    boxbar->setLabelText(tr("正在保存后台数据"));
    boxbar->show();
    wait(10);
    QString name = "backup";
    QSqlQuery query(QSqlDatabase::database(name));
    QList<int> uuids = msg.keys();
    QSqlDatabase::database(name).transaction();
    for (int i=0; i < uuids.size(); i++) {
        int uuid = uuids.at(i);
        tmpSet.insert(uuid, msg.value(uuid));
        if (uuid >= 10000 && uuid < 20000) {
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

void AppWindow::saveSqlite(QTmpMap msg)
{
    boxbar->setLabelText(tr("正在保存系统数据"));
    boxbar->show();
    wait(10);
    QString name = "system";
    QSqlQuery query(QSqlDatabase::database(name));
    QList<int> uuids = msg.keys();
    QSqlDatabase::database(name).transaction();
    for (int i=0; i < uuids.size(); i++) {
        int uuid = uuids.at(i);
        tmpSet.insert(uuid, msg.value(uuid));
        if (uuid >= 20000 && uuid < 30000) {
            query.prepare("replace into aip_system values(?,?)");
            query.addBindValue(uuid);
            query.addBindValue(msg[uuid]);
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

void AppWindow::saveConfig(QTmpMap msg)
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
    QString c_numb = tr("%1").arg(tmpSet.value(DataFile).toInt(), 4, 10, QChar('0'));
    QString c_name = tmpSet.value(DataType).toString();
    c_name = tr("T%1_%2").arg(c_numb).arg(c_name);
    for (int i=0; i < uuids.size(); i++) {
        int uuid = uuids.at(i);
        if (uuid >= 40000) {
            tmpSet.insert(uuid, msg.value(uuid));
            query.prepare(QString("replace into '%1' values(?,?)").arg(c_name));
            query.addBindValue(uuid);
            query.addBindValue(msg.value(uuid));
            if (!query.exec()) {
                qDebug() << query.lastError();
            }
        }
        boxbar->setValue(i*99/uuids.size());
    }
    QSqlDatabase::database(name).commit();
    boxbar->setValue(99);
    wait(500);
    query.clear();
    boxbar->setValue(100);
    testShift = Qt::Key_Away;
    qDebug() << "app save:" << tr("%1ms").arg(tt.elapsed(), 4, 10, QChar('0')) << c_name;
}

void AppWindow::clickButtons()
{
    tmpMsg.insert(Qt::Key_0, Qt::Key_View);
    tmpMsg.insert(Qt::Key_1, sender()->objectName());
    recvAppMsg(tmpMsg);
    tmpMsg.clear();
}

bool AppWindow::checkAction(QString msg)
{
    int temp = tmpSet.value(3000 + Qt::Key_0).toInt();  // 临时数据地址
    int sign = tmpSet.value(temp + TEMPSIGN).toInt();  // 是否已登录
    int real = tmpSet.value(DataUser).toInt();  // 当前用户地址
    int gear = (sign == 1) ? tmpSet.value(real + mRole).toInt() : 4;  // 当前权限等级
    int addr = tmpSet.value(2000 + Qt::Key_6).toInt();  // 界面权限地址
    int role = 0;
    QVariantMap win;
    for (int i=0; i < bufwin.size(); i++) {
        QVariantMap tmp = bufwin.at(i);
        if (tmp.value("name") == msg) {
            win = tmp;
            int g = tmp.value("form").toInt();
            int n = tmp.value("numb").toInt();
            role = tmpSet.value(addr + g*STEP + n).toInt();
            break;
        }
    }
    if (win.isEmpty())
        return false;

    QStringList gs;
    for (int i=0; i < bufwin.size(); i++) {
        QVariantMap tmp = bufwin.at(i);
        int p = win.value("form").toInt();
        int g = tmp.value("form").toInt();
        int n = tmp.value("numb").toInt();
        int r = tmpSet.value(addr + g*STEP + n).toInt();  // 界面权限
        int m = 1;
        if ((p == g && r >= gear && m != 0) || g == 0)
            gs.append(tmp.value("name").toString());
    }
    for (int i=0; i < buttons.size(); i++) {
        buttons.at(i)->setVisible(gs.contains(buttons.at(i)->objectName()));
        buttons.at(i)->setChecked(buttons.at(i)->objectName() == msg);
    }
    btnFrame->setVisible(win.value("form").toInt() > 0);
    if (gear > role)
        return false;
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
    return true;
}

bool AppWindow::screensSave(QString msg)
{
    if (isChange) {
        QString w = tr("数据未保存,确定退出吗?");
        int ret = QMessageBox::warning(this, tr("警告"), w, QMessageBox::Close | QMessageBox::Save);
        if (ret == QMessageBox::Save) {
            tmpMsg.insert(Qt::Key_0, Qt::Key_Save);
            tmpMsg.insert(Qt::Key_1, msg);
            emit sendAppMsg(tmpMsg);
            tmpMsg.clear();
        }
    }
    isChange = false;
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
        tmpMsg.insert(Qt::Key_0, Qt::Key_Send);
        tmpMsg.insert(Qt::Key_1, Qt::Key_0);
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
    }
}

void AppWindow::recvAppShow(QString msg)
{
    if (boxbar->isHidden()) {
        screensSave(msg);
        if (!checkAction(msg)) {
            QString w = tr("您无权限进行此操作,请重新登录");
            QMessageBox::warning(this, tr("警告"), w, QMessageBox::Ok);
            screensShow("signin");
        } else {
            screensShow(msg);
        }
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
    tmpMsg.insert(Qt::Key_0, Qt::Key_Time);
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();

    int ret = Qt::Key_Meta;
    if (taskMap.keys().contains(currTask))
        ret = (this->*taskMap[currTask])();
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
    currItem = Qt::Key_0;
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
            int addr = tmpSet.value((3000 + Qt::Key_0)).toInt();
            tmpSet.insert(addr + TEMPPLAY, QTime::currentTime());
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
    tmpMsg.insert(Qt::Key_0, Qt::Key_Call);
    tmpMsg.insert(Qt::Key_2, DATAON);
    tmpMsg.insert(Qt::Key_1, "LEDY");
    tmpMsg.insert(Qt::Key_4, station);
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();

    currItem = getNextItem();
    timeOut = t.elapsed();
    int real = tmpSet.value((3000 + Qt::Key_0)).toInt();
    tmpSet.insert(real + TEMPISOK, DATAOK);
    qDebug() << "app view:" << tr("%1ms").arg(t.elapsed(), 4, 10, QChar('0'));
    return Qt::Key_Away;
}

int AppWindow::taskStartWait()
{
    int addr = tmpSet.value((2000 + Qt::Key_1)).toInt();  // 系统设置地址
    int wait = tmpSet.value(addr + SystWait).toDouble() * 1000;
    return (t.elapsed() - timeOut > wait) ? Qt::Key_Away : Qt::Key_Meta;
}

int AppWindow::taskStartTest()
{
    int ret = testThread();
    return ret;
}

int AppWindow::taskStartSave()
{
    int addr = tmpSet.value((3000 + Qt::Key_0)).toInt();
    tmpSet.insert(Qt::Key_0, Qt::Key_Save);  // 存储数据
    tmpSet.insert(Qt::Key_1, "aip_record");
    tmpSet.insert(addr + TEMPSTOP, QTime::currentTime());
    tmpSet.insert(addr + TEMPWORK, station);
    emit sendAppMsg(tmpSet);
    qDebug() <<"app save:" << tr("%1ms").arg(t.elapsed(), 4, 10, QChar('0'));
    return Qt::Key_Away;
}

int AppWindow::taskStartBeep()
{
    int real = tmpSet.value(3000 + Qt::Key_0).toInt();  // 零散参数地址
    int isok = tmpSet.value(real + TEMPISOK).toInt();
    int addr = tmpSet[(2000 + Qt::Key_1)].toInt();
    int beep = tmpSet.value(addr + SystBeep).toInt() * 10 + 9;
    int tt = tmpSet.value(addr + (isok == DATAOK ? SystTime : SystWarn)).toDouble()*1000;
    tmpMsg.insert(Qt::Key_0, Qt::Key_Call);
    tmpMsg.insert(Qt::Key_1, isok == DATAOK ? "LEDG" : "LEDR");
    tmpMsg.insert(Qt::Key_2, (taskShift == Qt::Key_Stop) ? DATADC : isok);
    tmpMsg.insert(Qt::Key_3, beep);
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
    timeOut = t.elapsed();
    QTimer::singleShot(tt, this, SLOT(taskClearBeep()));
    qDebug() <<"app beep:" << tr("%1ms").arg(t.elapsed(), 4, 10, QChar('0'));
    return Qt::Key_Away;
}

int AppWindow::taskClearBeep()
{
    tmpMsg.insert(Qt::Key_0, Qt::Key_Call);
    tmpMsg.insert(Qt::Key_3, 0);
    emit sendAppMsg(tmpMsg);
    tmpMsg.clear();
    qDebug() << "app wait:" << tr("%1ms").arg(t.elapsed(), 4, 10, QChar('0'));

    return Qt::Key_Away;
}

int AppWindow::taskResetTest()
{
    if (taskShift == Qt::Key_Stop)
        return Qt::Key_Away;
    int addr = tmpSet.value(1000 + Qt::Key_0).toInt();  // 后台配置地址
    int test = tmpSet.value(addr + backAuto).toInt();  // 自动测试
    int time = tmpSet.value(addr + backWait).toInt();  // 测试延时
    if (test >= 1 && (t.elapsed() - timeOut >= time*1000)) {
        taskClearData();
        currTask = taskMap.values().indexOf(&AppWindow::taskCheckPlay);
        taskShift = Qt::Key_Play;
        station = (test == 1) ? WORKL : station;
        station = (test == 2) ? WORKR : station;
        station = (test == 3) ? ((station == WORKL) ? WORKR : WORKL) : station;
        t.restart();
    }
    return (test >= 1) ? Qt::Key_Meta : Qt::Key_Away;
}

int AppWindow::taskCheckStop()
{
    if (stack->currentWidget()->objectName() == "author")
        return Qt::Key_Away;
    int addr = tmpSet.value(3000 + Qt::Key_0).toInt();  // 综合测试结果
    int play = taskMap.values().indexOf(&AppWindow::taskCheckPlay);
    int save = taskMap.values().indexOf(&AppWindow::taskStartTest) + 1;
    int stop = taskMap.values().indexOf(&AppWindow::taskResetTest);
    int item = getNextItem();
    bool isStop = currTask < save && currTask > play;
    isStop = (testShift == Qt::Key_Away && item == Qt::Key_0) ? false : isStop;
    if (currTask <= play) {
        tmpMsg.insert(Qt::Key_0, Qt::Key_Call);
        tmpMsg.insert(Qt::Key_1, "LED1");
        tmpMsg.insert(Qt::Key_2, DATADD);
        tmpMsg.insert(Qt::Key_3, 0);
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
        taskClearData();
    }
    if (isStop) {
        taskShift = Qt::Key_Stop;
        currTask = taskMap.values().indexOf(&AppWindow::taskStartSave);
        tmpSet.insert(addr + TEMPISOK, DATANG);
    }
    if (currTask == stop) {
        taskShift = Qt::Key_Stop;
    }
    boxSta->hide();
    return Qt::Key_Away;
}

int AppWindow::testThread()
{
    int ret = Qt::Key_Meta;
    if (testMap.keys().contains(currTest)) {
        if ((this->*testMap[currTest])() == Qt::Key_Away) {
            currTest++;
            currTest = (currTest >= testMap.size()) ? 0 : currTest;  // 测试流程
            if (currTest == 0) {
                currItem = getNextItem();
                if (currItem == Qt::Key_0)
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
    tmpMsg.insert(Qt::Key_0, Qt::Key_Play);
    tmpMsg.insert(Qt::Key_1, currItem);
    tmpMsg.insert(Qt::Key_4, station);
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
        if (item > Qt::Key_8) {
            item = Qt::Key_0;
            break;
        }
        if (tmpSet.value(tmpSet.value(4000 + item).toInt()).toInt() == 1) {
            break;
        }
    }
    return item;
}

void AppWindow::showBarCode()
{
    int pAddr = tmpSet[(2000 + Qt::Key_1)].toInt();
    int pCode = tmpSet[pAddr + SystCode].toInt();
    int pSize = tmpSet[pAddr + SystSize].toInt();
    barcode = barcode.mid(pCode, pSize);
    tmpMsg.insert(tmpSet.value((3000 + Qt::Key_0)).toInt() + TEMPCODE, barcode);
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

void AppWindow::recvPlay(QTmpMap msg)
{
    if (stack->currentWidget()->objectName() == "setdcr")
        emit sendAppMsg(msg);
    if (stack->currentWidget()->objectName() == "setimp")
        emit sendAppMsg(msg);
    if (stack->currentWidget()->objectName() == "tester") {
        int addr = tmpSet.value(2000 + Qt::Key_1).toInt();
        int mode = tmpSet.value(addr + SystMode).toInt();
        if (mode == msg.value(Qt::Key_2).toInt()) {
            taskShift = Qt::Key_Play;
            station = msg.value(Qt::Key_1).toInt();
            boxSta->hide();
        } else {
            boxSta->setLabelText(tr("启动方式错误"));
            boxSta->show();
        }
    }
}

void AppWindow::recvNewMsg(QTmpMap msg)
{
#ifdef __arm__
    emit sendNetMsg(msg);
#endif
    int curr = msg.value(Qt::Key_1).toInt();
    int real = tmpSet.value(3000 + curr).toInt();  // 测试结果存储地址
    int addr = tmpSet.value(3000 + Qt::Key_0).toInt();  // 综合测试结果
    QList <int> keys = msg.keys();
    int tt = 0;
    switch (curr) {
    case Qt::Key_0:
#ifdef __arm__
        emit sendNetMsg(msg);
#endif
        emit sendAppMsg(msg);
        tmpSet.insert(addr + TEMPTEMP, msg.value(addr + TEMPTEMP).toDouble() / 10.0);
        break;
    case Qt::Key_1:
    case Qt::Key_2:
    case Qt::Key_3:
        if (msg.value(real + STATDCRA).toInt() == 2) {
            testShift = Qt::Key_Away;
            for (int i=0; i < keys.size(); i++) {
                tt = keys.at(i);
                tmpSet.insert(tt, msg.value(tt).toString());
            }
        }
        if (msg.value(real + OKNGDCRA).toInt() == DATANG) {
            tmpSet.insert(addr + TEMPISOK, DATANG);
        }
        break;
    case Qt::Key_4:
    case Qt::Key_5:
    case Qt::Key_6:
    case Qt::Key_7:
        if (msg.value(real + STATACWA).toInt() == 0) {
            testShift = Qt::Key_Away;
            for (int i=0; i < keys.size(); i++) {
                tt = keys.at(i);
                tmpSet.insert(tt, msg[tt].toString());
            }
        }
        if (msg.value(real + OKNGACWA).toInt() == DATANG) {
            tmpSet.insert(addr + TEMPISOK, DATANG);
        }
        break;
    case (Qt::Key_8):
        if (msg.value(real + STATIMPA).toInt() == 0) {
            testShift = Qt::Key_Away;
            for (int i=0; i < keys.size(); i++) {
                tt = keys.at(i);
                tmpSet.insert(tt, msg[tt].toString());
            }
        }
        if (msg.value(real + OKNGIMPA).toInt() == DATANG) {
            tmpSet.insert(addr + TEMPISOK, DATANG);
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
    int addr = tmpSet.value((3000 + Qt::Key_0)).toInt();
    tmpSet.insert(addr + TEMPTEMP, msg.value(addr + TEMPTEMP).toDouble() / 10.0);
}

void AppWindow::recvStaMsg(QTmpMap msg)
{
    QString str;
    quint32 temp = tmpSet.value(3000 + Qt::Key_0).toInt();
    if (msg.value(temp + TEMPDCRS).isNull()) {
        str.append(tr("电阻板异常"));
        qDebug() << "app stop:" << tr("电阻板异常");
    }
    if (msg.value(temp + TEMPACWS).isNull() || msg.value(temp + TEMPACWS).toInt() != 0) {
        int err = msg.value(temp + TEMPACWS).toInt();
        str.append((str.isEmpty() ? "" : "\n") + tr("耐压板异常"));
        QStringList errs;
        errs << "READY" << "TESTING"
             << "FLASH_ERROR" << "AC_CUR_ERROR" << "DC_CUR_ERROR"
             << "ACW_VOLT_ERROR" << "IR_VOLT_ERROR" << "DCW_VOLT_ERROR"
             << "PWR_CUR_ERROR" << "DCPT_ERROR" << "ARC_ERROR"
             << "LEAKPT_ERROR" << "IR_PT_ERROR" << "IR_MAG_ERROR"
             << "IR_VOLTMEAS_ERROR" << "IR_CURMEAS_ERROR" << "ACW_PT_ERROR"
             << "ACW_MAG_ERROR" << "ACW_VOLTMEAS_ERROR" << "ACW_CURMEAS_ERROR"
             << "DCW_ERROR";
        if (err >= 2 && err < errs.size()) {
            str.append(errs.at(err));
        }
        if (err >= errs.size())
            str.append(" 未知错误");
        qDebug() << "app stop:" << tr("耐压板异常");
    }
    if (msg.value(temp + TEMPIMPS).isNull() || msg.value(temp + TEMPIMPS).toInt() != 0) {
        int err = msg.value(temp + TEMPIMPS).toInt();
        str.append((str.isEmpty() ? "" : "\n") + tr("匝间板异常"));
        QStringList errs;
        errs << "READY" << "TESTING"
             << "FLASH_ERROR" << "HV_ERROR" << "WAVE_ERROR";
        if (err >= 2 && err < errs.size()) {
            str.append(errs.at(err));
        }
        if (err >= errs.size())
            str.append(" 未知错误");
        qDebug() << "app stop:" << tr("匝间板异常");
    }
    if (!str.isEmpty()) {
        boxSta->setLabelText(str);
        boxSta->show();
    }
}

void AppWindow::recvAppMsg(QTmpMap msg)
{
#ifndef __arm__
    if (sender()->objectName() != "socket")
        emit sendNetMsg(msg);
#endif
    switch (msg.value(Qt::Key_0).toInt()) {
    case Qt::Key_View:
        recvAppShow(msg.value(Qt::Key_1).toString());
        break;
    case Qt::Key_Back:
        tmpSet[tmpSet.value((3000 + Qt::Key_0)).toInt() + TEMPSIGN] = 0;
        recvAppShow("signin");
        break;
    case Qt::Key_Copy:
        tmpSet = msg;
        break;
    case Qt::Key_Save:
        if (msg.value(Qt::Key_1).toString() == "aip_backup") {  // 后台参数保存
            saveBackup(msg);
        }
        if (msg.value(Qt::Key_1).toString() == "aip_system") {  // 系统参数保存
            saveSqlite(msg);
        }
        if (msg.value(Qt::Key_1).toString() == "aip_config") {  // 配置参数保存
            saveConfig(msg);
        }
        if (msg.value(Qt::Key_1).toString() == "aip_reload") {  // 重新加载参数
            saveSqlite(msg);
            readConfig();
        }
        sendSqlite();
        isChange = false;
        break;
    case Qt::Key_Play:
        recvPlay(msg);
        break;
    case Qt::Key_Stop:  // 停止测试
        taskCheckStop();
        break;
    case Qt::Key_Xfer:
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
    case Qt::Key_Flip:
#ifdef __arm__
        emit sendNetMsg(msg);
#else
        qDebug() << "tcp recv:" << msg.value(Qt::Key_1).toString();
#endif
        break;
    case Qt::Key_Book:
        boxbar->show();
        boxbar->setLabelText(tr("正在查询数据"));
        boxbar->setValue(1);
        wait(1);
        emit sendAppMsg(msg);
        break;
    case Qt::Key_Word:
        if (!msg.value(Qt::Key_1).isNull())
            boxbar->setLabelText(msg.value(Qt::Key_1).toString());
        if (!msg.value(Qt::Key_2).isNull()) {
            if (msg.value(Qt::Key_2).toInt() >= 100)
                wait(1000);
            boxbar->setValue(msg.value(Qt::Key_2).toInt());
        }
        break;
    case Qt::Key_Menu:
        recvStaMsg(msg);
        break;
    case Qt::Key_Plus:  // 数据修改未保存
        isChange = true;
        break;
    case Qt::Key_Down:
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
