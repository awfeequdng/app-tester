/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       电枢测试仪主界面
*******************************************************************************/
#include "appwindow.h"

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

void AppWindow::initSql()
{  // 创建并打开数据库
    boxpop->setText(tr("正在初始化数据库..."));
    wait(10);
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
}

void AppWindow::initTitle()
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
}

void AppWindow::initLayout()
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
}

void AppWindow::initAuthor()
{
    QString name = "author";
    AppAuthor *app = new AppAuthor(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    connect(this, SIGNAL(sendAppMap(QVariantMap)), app, SLOT(recvAppMap(QVariantMap)));
    stack->addWidget(app);

    initButton(tr("返回主页"), name);
}

void AppWindow::initSocket()
{
    TcpSocket *tcp = new TcpSocket(this);
    connect(tcp, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    //    connect(this, SIGNAL(sendAppMap(QVariantMap)), tcp, SLOT(recvAppMap(QVariantMap)));

    tmpMap.insert("hostaddr", "s.aipuo.com");
    tmpMap.insert("hostport", "6000");
    tmpMap.insert("devNumb", config[QString::number(AddrNumber)].toString());
    tmpMap.insert("version", config[QString::number(AddrVerNub)].toString());
    tcp->connectToServer(tmpMap);
    tmpMap.clear();
}

void AppWindow::initDevice()
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
#endif
}

void AppWindow::initScreen()
{
    boxpop = new BoxDialog(this);
    boxpop->show();
    initSql();
    initTester();
    initSignin();
    initSystem();
    initMaster();
    initAction();
    initBackup();
    initLogger();
    initConfig();
    initSetDcr();
    initSetAcw();
    initSetImp();
    initOffDcr();
    initOffAcw();
    initOffImp();
    initRecord();
    initUpload();
    initSdcard();
    initUnqual();
    readSettings();
    readModelSet();
    initSettings();
    sendSettings();
    initSocket();
    boxpop->hide();
    //    recvAppShow("signin");
    btnLayout->addStretch();

    tmpMap.insert("enum", Qt::Key_Enter);
    emit sendAppMap(tmpMap);
    tmpMap.clear();

    if (isSignin) {
        QTimer::singleShot(500, this, SLOT(showTester()));
    } else {
        recvAppShow("signin");
    }
}

void AppWindow::initSignin()
{
    boxpop->setText(tr("正在初始化登录界面..."));
    wait(10);
    isSignin = false;
    QString name = "signin";
    AppSignin *app = new AppSignin(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    connect(this, SIGNAL(sendAppMap(QVariantMap)), app, SLOT(recvAppMap(QVariantMap)));
    stack->addWidget(app);

    initButton(tr("用户登录"), name);
}

void AppWindow::initSystem()
{
    boxpop->setText(tr("正在初始化系统设置..."));
    wait(10);
    QString name = "system";
    AppSystem *app = new AppSystem(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    connect(this, SIGNAL(sendAppMap(QVariantMap)), app, SLOT(recvAppMap(QVariantMap)));
    stack->addWidget(app);

    initButton(tr("系统设置"), name);
}

void AppWindow::initBackup()
{
    boxpop->setText(tr("正在初始化后台设置..."));
    wait(10);
    QString name = "backup";
    AppBackup *app = new AppBackup(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    connect(this, SIGNAL(sendAppMap(QVariantMap)), app, SLOT(recvAppMap(QVariantMap)));
    stack->addWidget(app);

    initButton(tr("后台管理"), name);
}

void AppWindow::initLogger()
{
    boxpop->setText(tr("正在初始化调试信息..."));
    wait(10);
    QString name = "logger";
    AppLogger *logger = AppLogger::instance();
    logger->setObjectName(name);
    connect(logger, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    connect(this, SIGNAL(sendAppMap(QVariantMap)), logger, SLOT(recvAppMap(QVariantMap)));
    stack->addWidget(logger);

    initButton(tr("调试信息"), name);
}

void AppWindow::initMaster()
{
    boxpop->setText(tr("正在初始化用户管理..."));
    wait(10);
    QString name = "master";
    AppMaster *app = new AppMaster(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    connect(this, SIGNAL(sendAppMap(QVariantMap)), app, SLOT(recvAppMap(QVariantMap)));
    stack->addWidget(app);

    initButton(tr("用户管理"), name);
}

void AppWindow::initAction()
{
    boxpop->setText(tr("正在初始化权限管理..."));
    wait(10);
    QString name = "action";
    AppAction *app = new AppAction(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    connect(this, SIGNAL(sendAppMap(QVariantMap)), app, SLOT(recvAppMap(QVariantMap)));
    stack->addWidget(app);

    initButton(tr("权限管理"), name);
}

void AppWindow::initConfig()
{
    boxpop->setText(tr("正在初始化型号管理..."));
    wait(10);
    QString name = "config";
    TypConfig *app = new TypConfig(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    connect(this, SIGNAL(sendAppMap(QVariantMap)), app, SLOT(recvAppMap(QVariantMap)));
    stack->addWidget(app);

    initButton(tr("型号管理"), name);
}

void AppWindow::initSetDcr()
{
    boxpop->setText(tr("正在初始化电阻配置..."));
    wait(10);
    QString name = "setdcr";
    TypSetDcr *app = new TypSetDcr(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    stack->addWidget(app);

    initButton(tr("电阻配置"), name);
}

void AppWindow::initSetAcw()
{
    boxpop->setText(tr("正在初始化介电强度..."));
    wait(10);
    QString name = "setacw";
    TypSetAcw *app = new TypSetAcw(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    connect(this, SIGNAL(sendAppMap(QVariantMap)), app, SLOT(recvAppMap(QVariantMap)));
    stack->addWidget(app);

    initButton(tr("介电强度"), name);
}

void AppWindow::initSetImp()
{
    boxpop->setText(tr("正在初始化匝间配置..."));
    wait(10);
    QString name = "setimp";
    TypSetImp *app = new TypSetImp(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    stack->addWidget(app);

    initButton(tr("匝间配置"), name);
}

void AppWindow::initOffDcr()
{
    boxpop->setText(tr("正在初始化电阻调试..."));
    wait(10);
    QString name = "offdcr";
    TypOffDcr *app = new TypOffDcr(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    stack->addWidget(app);

    initButton(tr("电阻调试"), name);
}

void AppWindow::initOffAcw()
{
    boxpop->setText(tr("正在初始化绝缘调试..."));
    wait(10);
    QString name = "offacw";
    TypOffAcw *app = new TypOffAcw(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    stack->addWidget(app);

    initButton(tr("绝缘调试"), name);
}

void AppWindow::initOffImp()
{
    boxpop->setText(tr("正在初始化匝间调试..."));
    wait(10);
    QString name = "offimp";
    TypOffImp *app = new TypOffImp(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    stack->addWidget(app);

    initButton(tr("匝间调试"), name);
}

void AppWindow::initRecord()
{
    boxpop->setText(tr("正在初始化数据管理..."));
    wait(10);
    QString name = "record";
    SqlRecord *app = new SqlRecord(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    stack->addWidget(app);

    initButton(tr("数据管理"), name);
}

void AppWindow::initUpload()
{
    boxpop->setText(tr("正在初始化数据上传..."));
    wait(10);
    QString name = "upload";
    SqlUpload *app = new SqlUpload(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    connect(this, SIGNAL(sendAppMap(QVariantMap)), app, SLOT(recvAppMap(QVariantMap)));
    stack->addWidget(app);

    initButton(tr("数据上传"), name);
}

void AppWindow::initSdcard()
{
    boxpop->setText(tr("正在初始化数据历史..."));
    wait(10);
    QString name = "sdcard";
    SqlSdcard *app = new SqlSdcard(this);
    app->setObjectName(name);
    connect(app, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    stack->addWidget(app);

    initButton(tr("数据历史"), name);
}

void AppWindow::initUnqual()
{
    boxpop->setText(tr("正在初始化数据分析..."));
    wait(10);
    QString name = "unqual";
    SqlUnqual *app = new SqlUnqual(this);
    app->setObjectName(name);
    //    connect(unqual, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    stack->addWidget(app);

    initButton(tr("数据分析"), name);
}

void AppWindow::initTester()
{
    boxpop->setText(tr("正在初始化测试界面..."));
    wait(10);
    QString name = "tester";
    AppTester *app = new AppTester(this);
    app->setObjectName("tester");
    connect(app, SIGNAL(sendAppMap(QVariantMap)), this, SLOT(recvAppMap(QVariantMap)));
    stack->addWidget(app);

    initButton(tr("返回测试"), name);
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
    boxpop->setText(text);
    QTimer::singleShot(t, boxpop, SLOT(hide()));
    boxpop->show();
}

void AppWindow::readSettings()
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
}

void AppWindow::readModelSet()
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
}

void AppWindow::initSettings()
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
}

void AppWindow::sendSettings()
{
    config["0"] = verNumb;
    config["1"] = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    config.insert("enum", Qt::Key_Option);
    emit sendAppMap(config);
    config.remove("enum");
}

void AppWindow::saveSettings()
{
    boxpop->setText(tr("正在保存参数..."));
    boxpop->show();
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
    }
    QSqlDatabase::database(name).commit();
    wait(500);
    query.clear();
    qDebug() << "app save:" << name;
    boxpop->hide();
}

void AppWindow::saveModelSet()
{
    boxpop->setText(tr("正在保存参数..."));
    boxpop->show();
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
    wait(500);
    query.clear();
    qDebug() << "app save:" << type;
    boxpop->hide();
}

void AppWindow::clickButtons()
{
    recvAppShow(sender()->objectName());
}

void AppWindow::showTester()
{
    recvAppShow("tester");
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

void AppWindow::recvAppMap(QVariantMap msg)
{
    switch (msg.value("enum").toInt()) {
    case Qt::Key_Display:
        recvAppShow(msg.value("text").toString());
        break;
    case Qt::Key_Enter:
        isSignin = true;
        QTimer::singleShot(500, this, SLOT(showTester()));
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
        saveSettings();
        initSettings();
        sendSettings();
        break;
    case Qt::Key_Option:
        msg.remove("enum");
        for (int i=0; i < msg.keys().size(); i++) {
            config[msg.keys().at(i)] = msg[msg.keys().at(i)];
        }
        saveModelSet();
        initSettings();
        sendSettings();
        break;
        break;
    case Qt::Key_Reload:
        config[QString::number(AddrConfig)] = msg[QString::number(AddrConfig)];
        saveSettings();
        readModelSet();
        initSettings();
        sendSettings();
        break;
    case Qt::Key_Stop:
        tmpMap.insert("enum", Qt::Key_View);
        tmpMap.insert("text", "LED1");
        tmpMap.insert("data", 0);
        emit sendAppMap(tmpMap);
        tmpMap.clear();
        tmpMap.insert("enum", Qt::Key_BrightnessAdjust);
        tmpMap.insert("data", 100);
        emit sendAppMap(tmpMap);
        tmpMap.clear();
        break;
    case Qt::Key_Play:
        tmpMap.insert("enum", Qt::Key_View);
        tmpMap.insert("text", "LEDY");
        emit sendAppMap(tmpMap);
        wait(1000);
        tmpMap.insert("text", "LEDG");
        tmpMap.insert("data", 99);
        emit sendAppMap(tmpMap);
        wait(1000);
        tmpMap.insert("text", "LEDR");
        tmpMap.insert("data", 0);
        emit sendAppMap(tmpMap);
        tmpMap.clear();

        tmpMap.insert("enum", Qt::Key_BrightnessAdjust);
        tmpMap.insert("data", 50);
        emit sendAppMap(tmpMap);
        tmpMap.clear();
        break;
    case Qt::Key_Time:
        emit sendAppMap(msg);
        break;
    case Qt::Key_Community:
        emit sendAppMap(msg);
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
