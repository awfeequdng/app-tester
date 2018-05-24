#-------------------------------------------------
#
# Project created by QtCreator 2018-04-17T11:00:53
#
#-------------------------------------------------

QT       += core gui sql serialport network xml

CONFIG   += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = aip
TEMPLATE = app

RC_FILE += qrc/appsource.rc

RESOURCES += \
    qrc/appsource.qrc

HEADERS += \
    app/appaction.h \
    app/appauthor.h \
    app/appbackup.h \
    app/appdefine.h \
    app/applogger.h \
    app/appmaster.h \
    app/appsignin.h \
    app/appsystem.h \
    app/apptester.h \
    app/appwindow.h \
    app/boxdialog.h \
    app/boxdouble.h \
    app/boxqchart.h \
    app/boxqcombo.h \
    app/boxqinput.h \
    app/boxqitems.h \
    app/boxqmodel.h \
    app/devbuzzer.h \
    app/devscreen.h \
    app/devserial.h \
    app/devsetcan.h \
    app/devsetrtc.h \
    app/devsystem.h \
    app/sqlcreate.h \
    app/sqlexport.h \
    app/sqlimport.h \
    app/sqlrecord.h \
    app/sqlsdcard.h \
    app/sqlsnowid.h \
    app/sqlunqual.h \
    app/sqlupload.h \
    app/tcpsocket.h \
    app/typconfig.h \
    app/typoffacw.h \
    app/typoffdcr.h \
    app/typoffimp.h \
    app/typsetacw.h \
    app/typsetdcr.h \
    app/typsetimp.h \
    app/udpsocket.h \
    lib/qcustomplot.h \
    app/tcpserver.h \
    app/tcponline.h

SOURCES += \
    app/appaction.cpp \
    app/appauthor.cpp \
    app/appbackup.cpp \
    app/applogger.cpp \
    app/appmaster.cpp \
    app/appsignin.cpp \
    app/appsystem.cpp \
    app/apptester.cpp \
    app/appwindow.cpp \
    app/boxdialog.cpp \
    app/boxdouble.cpp \
    app/boxqchart.cpp \
    app/boxqcombo.cpp \
    app/boxqinput.cpp \
    app/boxqitems.cpp \
    app/boxqmodel.cpp \
    app/devbuzzer.cpp \
    app/devscreen.cpp \
    app/devserial.cpp \
    app/devsetcan.cpp \
    app/devsetrtc.cpp \
    app/devsystem.cpp \
    app/main.cpp \
    app/sqlcreate.cpp \
    app/sqlexport.cpp \
    app/sqlimport.cpp \
    app/sqlrecord.cpp \
    app/sqlsdcard.cpp \
    app/sqlsnowid.cpp \
    app/sqlunqual.cpp \
    app/sqlupload.cpp \
    app/tcpsocket.cpp \
    app/typconfig.cpp \
    app/typoffacw.cpp \
    app/typoffdcr.cpp \
    app/typoffimp.cpp \
    app/typsetacw.cpp \
    app/typsetdcr.cpp \
    app/typsetimp.cpp \
    app/udpsocket.cpp \
    lib/qcustomplot.cpp \
    app/tcpserver.cpp \
    app/tcponline.cpp

