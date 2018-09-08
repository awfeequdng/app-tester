/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       WIFI设置
*******************************************************************************/
#include "apprt3070.h"

AppRT3070::AppRT3070(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void AppRT3070::initUI()
{
    initLayout();
    initViewBar();
    initTextBar();
}

void AppRT3070::initLayout()
{
    layout = new QVBoxLayout(this);
}

void AppRT3070::initViewBar()
{
    view = new QTableWidget(this);
    view->setColumnCount(1);
    view->setRowCount(MAXNET);
    for (int i=0; i < MAXNET; i++) {
        initViewItem(i);
    }
    view->verticalHeader()->setVisible(false);
    view->horizontalHeader()->setVisible(false);
#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    view->verticalHeader()->setResizeMode(QHeaderView::Stretch);
    view->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif

    layout->addWidget(view);
}

void AppRT3070::initTextBar()
{
    QHBoxLayout *wifi = new QHBoxLayout;
    layout->addLayout(wifi);
    wifi->addWidget(new QLabel(tr("网络名称:"), this));
    ssid = new QLineEdit(this);
    wifi->addWidget(ssid);
    wifi->addWidget(new QLabel(tr("密码:"), this));
    pswd = new QLineEdit(this);
    wifi->addWidget(pswd);
    pswd->setEchoMode(QLineEdit::Password);

    btnWifi = new QPushButton(tr("连接"), this);
    btnWifi->setFixedSize(97, 35);
    wifi->addWidget(btnWifi);
    connect(btnWifi, SIGNAL(clicked(bool)), this, SLOT(saveWireLess()));

    btnShow = new QPushButton(tr("刷新"), this);
    btnShow->setFixedSize(97, 35);
    wifi->addWidget(btnShow);
    connect(btnShow, SIGNAL(clicked(bool)), this, SLOT(initWireLess()));

    text = new QTextBrowser(this);
    layout->addWidget(text);

    layout->setStretch(0, 2);
    layout->setStretch(2, 1);
}

void AppRT3070::initWireLess()
{
    btnWifi->setEnabled(false);
    btnShow->setEnabled(false);
#ifdef __arm__
    QProcess w;
    w.start("ifconfig ra0 up");
    w.waitForFinished();
    w.start("iwlist ra0 scanning");
    w.waitForFinished();
    QByteArray msg = w.readAll();
    QStringList wires = QString(msg).split("Cell");
    wires.removeFirst();
    wifis.clear();
    for (int i=0; i < qMin(MAXNET, wires.size()); i++) {
        QStringList less = wires.at(i).split("\n");
        for (int t=0; t < less.size(); t++) {
            QString text = less.at(t);
            text = text.remove("\t");
            if (text.contains("ESSID:")) {
                names.at(i)->setText(wifiSG.arg(text));
                int t = text.indexOf(":");
                QString str = text.mid(t+1, 200);
                str.remove("\"");
                wifis.insert(QString::number(i), str);
            }
            if (text.contains("Mode:")) {
                texts.at(i)->setText(strWifi.arg(i).arg(text.remove(" ")));
            }
            if (text.contains("Quality=")) {
                int index = text.indexOf("/");
                int p = (text.mid(0, index)).remove("Quality=").toInt();
                if (p / 20 == 0)
                    icons.at(i)->setPixmap(QPixmap(":/wifi1.png"));
                if (p / 20 == 1)
                    icons.at(i)->setPixmap(QPixmap(":/wifi2.png"));
                if (p / 20 == 2)
                    icons.at(i)->setPixmap(QPixmap(":/wifi3.png"));
                if (p / 20 >= 3)
                    icons.at(i)->setPixmap(QPixmap(":/wifi4.png"));
            }
        }
    }
#endif
    btnWifi->setEnabled(true);
    btnShow->setEnabled(true);
}

void AppRT3070::initViewItem(int row)
{
    QLabel *name = new QLabel(this);
    names.append(name);
    QLabel *text = new QLabel(this);
    texts.append(text);
    connect(text, SIGNAL(linkActivated(QString)), this, SLOT(clickLink(QString)));
    QLabel *icon = new QLabel(this);
    icons.append(icon);
    icon->setFixedSize(64, 64);
    icon->setScaledContents(true);

    QHBoxLayout *wlayout = new QHBoxLayout;
    wlayout->addWidget(name);
    wlayout->addWidget(text);
    wlayout->addWidget(icon);
    wlayout->setStretch(0, 1);
    wlayout->setStretch(1, 1);
    wlayout->setStretch(2, 1);
    QFrame *frm = new QFrame(this);
    frm->setLayout(wlayout);
    view->setCellWidget(row, 0, frm);
}

void AppRT3070::initSettings()
{
    QString path = "wpa_supplicant.conf";
    path = "/etc/wpa_supplicant.conf";
    QSettings *set = new QSettings(path, QSettings::NativeFormat);
    ssid->setText(set->value("ssid").toString());
    pswd->setText(set->value("psk").toString());
    QString ssid = set->value("ssid").toString();
    QString psk = set->value("psk").toString();
    qDebug() << ssid << psk;
}

void AppRT3070::saveWireLess()
{
    btnWifi->setEnabled(false);
    btnShow->setEnabled(false);
    QString path = "./wpa_supplicant.conf";
#ifdef __arm__
    path = "/etc/wpa_supplicant.conf";
#endif
    QFile file(path);
    file.open(QFile::ReadWrite | QFile::Truncate);
    file.write("# WPA-PSK/TKIP\n\n");
    file.write("ctrl_interface=/var/run/wpa_supplicant\n\n");
    file.write("network={\n");
    file.write(tr("\tssid=\"%1\"\n").arg(ssid->text()).toUtf8());
    file.write("\tscan_ssid=1\n");
    file.write("\tkey_mgmt=WPA-EAP WPA-PSK IEEE8021X NONE\n");
    file.write("\tpairwise=TKIP CCMP\n");
    file.write("\tgroup=CCMP TKIP WEP104 WEP40\n");
    file.write(tr("\tpsk=\"%1\"\n").arg(pswd->text()).toUtf8());
    file.write("}\n");
    file.close();

    text->clear();
    text->insertPlainText("linking...\n");
    wait(10);
    QProcess w;
    w.start("ifconfig ra0 down");
    w.waitForFinished();
    w.start("ifconfig ra0 up");
    w.waitForFinished();
    w.start("wpa_supplicant -B -D wext -i ra0 -c /etc/wpa_supplicant.conf");
    w.waitForFinished();
    w.start("udhcpc -i ra0");
    w.waitForFinished();
    text->insertPlainText(w.readAll());
    btnWifi->setEnabled(true);
    btnShow->setEnabled(true);
}

void AppRT3070::clickLink(QString str)
{
    int row = str.toInt();
    ssid->setText(wifis.value(QString::number(row)).toString());
}

void AppRT3070::showEvent(QShowEvent *e)
{
    QTimer::singleShot(1000, this, SLOT(initWireLess()));
    initSettings();
    e->accept();
}

void AppRT3070::wait(int ms)
{
    QElapsedTimer t;
    t.start();
    while (t.elapsed() < ms)
        QCoreApplication::processEvents();
}

