/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       电枢测试仪
*******************************************************************************/
#include <QApplication>
#include <QTranslator>
#include <QTextCodec>
#include <QFile>

#include "appwindow.h"
#include "boxqinput.h"

int main(int argc, char *argv[])
{
#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
#endif

    QApplication a(argc, argv);

    QFile file;
    QString qss;
    file.setFileName(":/qss_black.css");
    file.open(QFile::ReadOnly);
    qss = QLatin1String(file.readAll());
    qApp->setStyleSheet(qss);

    QTranslator qtTran;
    qtTran.load(":/qt_zh_CN.qm");
    a.installTranslator(&qtTran);

    QTranslator qtBase;
    qtBase.load(":/qtbase_zh_CN.qm");
    a.installTranslator(&qtBase);

    AppWindow w;
    w.show();



#ifdef __arm__
    BoxQInput::instance()->hide();
#endif

//    BoxQInput::instance()->show();

    return a.exec();
}
