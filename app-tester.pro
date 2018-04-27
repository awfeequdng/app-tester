#-------------------------------------------------
#
# Project created by QtCreator 2018-04-17T11:00:53
#
#-------------------------------------------------

QT       += core gui sql serialport

CONFIG   += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = aip
TEMPLATE = app

RC_FILE += qrc/appsource.rc

RESOURCES += \
    qrc/appsource.qrc

HEADERS += \
    app/appauthor.h \
    app/appwindow.h \
    app/apptester.h \
    lib/qcustomplot.h \
    app/sqlrecord.h \
    app/sqlsnowid.h \
    app/sqlcreate.h \
    app/appsystem.h \
    app/appbackup.h \
    app/applogger.h \
    app/appmaster.h \
    app/appaction.h \
    app/sqlupload.h \
    app/sqlsdcard.h \
    app/boxqcombo.h \
    app/boxqitems.h \
    app/appsignin.h \
    app/boxdialog.h \
    app/sqlunqual.h \
    app/sqlexport.h \
    app/sqlimport.h \
    app/typconfig.h \
    app/typsetdcr.h \
    app/typsetacw.h \
    app/typsetimp.h \
    app/typoffdcr.h \
    app/typoffacw.h \
    app/typoffimp.h \
    app/boxqinput.h \
    app/boxqchart.h \
    app/typsetall.h \
    app/devsetkey.h

SOURCES += \
    app/main.cpp \
    app/appauthor.cpp \
    app/appwindow.cpp \
    app/apptester.cpp \
    lib/qcustomplot.cpp \
    app/sqlrecord.cpp \
    app/sqlsnowid.cpp \
    app/sqlcreate.cpp \
    app/appsystem.cpp \
    app/appbackup.cpp \
    app/applogger.cpp \
    app/appmaster.cpp \
    app/appaction.cpp \
    app/sqlupload.cpp \
    app/sqlsdcard.cpp \
    app/boxqcombo.cpp \
    app/boxqitems.cpp \
    app/appsignin.cpp \
    app/boxdialog.cpp \
    app/sqlunqual.cpp \
    app/sqlexport.cpp \
    app/sqlimport.cpp \
    app/typconfig.cpp \
    app/typsetdcr.cpp \
    app/typsetacw.cpp \
    app/typsetimp.cpp \
    app/typoffdcr.cpp \
    app/typoffacw.cpp \
    app/typoffimp.cpp \
    app/boxqinput.cpp \
    app/boxqchart.cpp \
    app/typsetall.cpp \
    app/devsetkey.cpp
