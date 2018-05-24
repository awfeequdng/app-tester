#ifndef TCPONLINE_H
#define TCPONLINE_H

#include <QDebug>
#include <QTimer>
#include <QLayout>
#include <QWidget>
#include <QLineEdit>
#include <QTreeView>
#include <QTableView>
#include <QHeaderView>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QFileSystemModel>
#include <QStandardItem>
#include <QTextBrowser>
#include <QTreeWidgetItem>

#include <QLabel>
#include <QSplitter>
#include <QPushButton>
#include <QGroupBox>
#include <QShowEvent>

#include <QJsonObject>
#include <QJsonDocument>

#include "appdefine.h"
#include "boxqmodel.h"
#include "boxqitems.h"

#define ONLINE_REC_UUID 0x00
#define ONLINE_DEV_UUID 0x01
#define ONLINE_DEV_ADDR 0x02
#define ONLINE_DEV_NUMB 0x03
#define ONLINE_NET_PORT 0x04
#define ONLINE_NET_ADDR 0x05
#define ONLINE_REC_TIME 0x06
#define ONLINE_NET_STAT 0x07
#define ONLINE_VER_NUMB 0x08

#define ONLINE_SHELL_CMD 101
#define ONLINE_SEND_HAED 102
#define ONLINE_READ_HAED 103

class TcpOnline : public QWidget
{
    Q_OBJECT
public:
    explicit TcpOnline(QWidget *parent = 0);

signals:
    void sendAppMsg(QTmpMap msg);
    void sendNetMsg(QJsonObject dat);
private slots:
    void initUI();
    void initView();
    void initTree();
    void initFile();
    void initText();
    void initCtrl();
    void initLayout();
    void initSqlite();
    void clickTree(QModelIndex index);
    void clickFile(QModelIndex index);
    void clickOnline(QModelIndex index);
    void updateFile();
    void recvFileHead();
    void sendShellCmd();
    void sendFileHead();
    void updateSqlite();
    void updateFilter(int col, QString msg);
    void recvDisplay(QString msg);
    void recvTarget();
    void recvAppMsg(QTmpMap msg);
    virtual void showEvent(QShowEvent *e);
private:
    QTableView *onlineView;
    QSqlTableModel *mOnlineView;
    QSqlTableModel *mDeviceView;

    QTreeWidget *onlineTree;

    QTreeView *onlineFile;
    QFileSystemModel *mOnlineFile;

    QTableView *onlineText;
    BoxQModel *mOnlineText;

    QGroupBox *onlineCtrl;
    QLineEdit *sendFile;
    QLabel *currentRate;
    QLineEdit *shellCmd;
    QTextBrowser *text;
    QLineEdit *recvFile;

    QString sqlName;
    QString currentPort;
    QString currentPath;
    QMap<int, QString> filters;

    QTmpMap tmpSet;
    QTmpMap tmpMsg;
    bool isClick;
};

#endif // TCPONLINE_H
