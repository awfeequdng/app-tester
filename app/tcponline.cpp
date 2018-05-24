#include "tcponline.h"

TcpOnline::TcpOnline(QWidget *parent) : QWidget(parent)
{
    initUI();
    initSqlite();
    isClick = false;
}

void TcpOnline::initUI()
{
    initView();
    initTree();
    initFile();
    initText();
    initCtrl();
    initLayout();
}

void TcpOnline::initView()
{
    onlineView = new QTableView(this);

    onlineView->setItemDelegate(new BoxQItems());
    onlineView->setSortingEnabled(true);
    onlineView->horizontalHeader()->setHighlightSections(false);
    onlineView->setSelectionBehavior(QAbstractItemView::SelectRows);
    onlineView->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(onlineView, SIGNAL(clicked(QModelIndex)), this, SLOT(clickOnline(QModelIndex)));
}

void TcpOnline::initTree()
{
    onlineTree = new QTreeWidget(this);
    onlineTree->setHeaderHidden(true);
    connect(onlineTree, SIGNAL(clicked(QModelIndex)), this, SLOT(clickTree(QModelIndex)));
}

void TcpOnline::initFile()
{
    onlineFile = new QTreeView(this);
    updateFile();
    connect(onlineFile, SIGNAL(clicked(QModelIndex)), this, SLOT(clickFile(QModelIndex)));
}

void TcpOnline::initText()
{
    QStringList items;
    items << "设备编号" << "设备地址" << "出厂编号" << "设备型号"
          << "评审编号" << "出厂日期" << "客户名称" << "网络端口";
    QStringList headers;
    headers << "项目" << "参数";
    mOnlineText = new BoxQModel(this);
    mOnlineText->setColumnCount(items.size());
    mOnlineText->setRowCount(1);
    for (int i=0; i < mOnlineText->columnCount(); i++) {
        mOnlineText->setHeaderData(i, Qt::Horizontal, items.at(i));
    }
    for (int i=0; i < items.size(); i++) {
        mOnlineText->setItem(0, i, new QStandardItem());
    }
    onlineText = new QTableView(this);
    onlineText->setModel(mOnlineText);
    onlineText->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    onlineText->setColumnWidth(0, 65);
    onlineText->verticalHeader()->hide();
}

void TcpOnline::initCtrl()
{
    QPushButton *btnShellCmd = new QPushButton("发送命令", this);
    connect(btnShellCmd, SIGNAL(clicked(bool)), this, SLOT(sendShellCmd()));
    shellCmd = new QLineEdit(this);
    connect(shellCmd, SIGNAL(returnPressed()), this, SLOT(sendShellCmd()));

    QPushButton *btnSendFile = new QPushButton("发送文件", this);
    connect(btnSendFile, SIGNAL(clicked(bool)), this, SLOT(sendFileHead()));
    sendFile = new QLineEdit(this);
    connect(sendFile, SIGNAL(returnPressed()), this, SLOT(sendFileHead()));

    QPushButton *btnRecvFile = new QPushButton("接收文件", this);
    connect(btnRecvFile, SIGNAL(clicked(bool)), this, SLOT(recvFileHead()));
    recvFile = new QLineEdit(this);
    connect(recvFile, SIGNAL(returnPressed()), this, SLOT(recvFileHead()));

    text = new QTextBrowser(this);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(recvFile, 0, 0);
    layout->addWidget(btnRecvFile, 0, 1);

    layout->addWidget(sendFile, 1, 0);
    layout->addWidget(btnSendFile, 1, 1);

    layout->addWidget(shellCmd, 2, 0);
    layout->addWidget(btnShellCmd, 2, 1);

    layout->addWidget(text, 3, 0, 1, 2);
    layout->setColumnStretch(0, 1);

    onlineCtrl = new QGroupBox(this);
    onlineCtrl->setLayout(layout);
}

void TcpOnline::initLayout()
{
    QSplitter *hsplitter = new QSplitter(this);

    hsplitter->addWidget(onlineTree);
    hsplitter->addWidget(onlineFile);
    hsplitter->addWidget(onlineCtrl);
    hsplitter->setStretchFactor(0, 3);
    hsplitter->setStretchFactor(1, 3);
    hsplitter->setStretchFactor(2, 5);

    QSplitter *vsplitter = new QSplitter(Qt::Vertical, this);
    vsplitter->addWidget(onlineView);
    vsplitter->addWidget(onlineText);
    vsplitter->addWidget(hsplitter);
    vsplitter->setStretchFactor(0, 11);
    vsplitter->setStretchFactor(1, 2);
    vsplitter->setStretchFactor(2, 5);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(vsplitter);
    this->setLayout(layout);
}

void TcpOnline::initSqlite()
{
    sqlName = "mysql";
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL3", sqlName);
    db.setHostName("192.168.1.55");
    db.setUserName("root");
    db.setPassword("87973318");
    db.setDatabaseName("aip-server");
    if (!db.open()) {
        qDebug() << "open fail" << db.lastError();
    } else {
        updateSqlite();
    }
}

void TcpOnline::clickTree(QModelIndex index)
{
    if (index.row() < 0)
        return;
    QString temp = onlineTree->currentItem()->text(1);
    QString name = onlineTree->currentItem()->text(0);
    if (temp == "文件夹") {
        int t = currentPath.lastIndexOf("/");
        if (name == ".." && t > 0) {
            currentPath = currentPath.mid(0, t);
        } else if (name == ".." && t == 0) {
            currentPath = "/";
        } else {
            if (currentPath != "/")
                currentPath.append("/");
            currentPath.append(name);
        }
        QString text = shellCmd->text();
        shellCmd->setText(QString("ls -aF %1").arg(currentPath));
        onlineTree->clear();
        sendShellCmd();
        shellCmd->setText(text);
    }
    if (temp == "文件") {
        QString path = currentPath;
        path.append("/");
        path.append(name);
        recvFile->setText(path);
    }
}

void TcpOnline::clickFile(QModelIndex index)
{
    QString path = mOnlineFile->filePath(index);
#ifdef __linux__
    path.remove("/home/link/smbtmp/");
#else
    path.remove("//192.168.1.55/share/二代升级文件/");
#endif
    sendFile->setText(path);
}

void TcpOnline::clickOnline(QModelIndex index)
{
    int row = index.row();
    QString uuid = mOnlineView->index(row, ONLINE_DEV_UUID).data().toString();
    QString filter = QString("devUuid = %1").arg(uuid);
    mDeviceView->setFilter(filter);
    mDeviceView->select();
    for (int i=0; i < mDeviceView->columnCount(); i++) {
        QString item = mDeviceView->index(0, i).data().toString();
        mOnlineText->setData(mOnlineText->index(0, i), item, Qt::DisplayRole);
    }
    currentPort = mOnlineView->index(row, ONLINE_NET_PORT).data().toString();
    int pRow = mOnlineText->columnCount()-1;
    mOnlineText->setData(mOnlineText->index(0, pRow), currentPort, Qt::DisplayRole);

    onlineTree->clear();
    shellCmd->setText("ls -aF /");
    currentPath = "/";
    recvFile->setText(currentPath);
    sendShellCmd();
    shellCmd->setText("mv network/userinfo.txt nandflash");
}

void TcpOnline::updateFile()
{
    mOnlineFile = new QFileSystemModel(this);
    mOnlineFile->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    onlineFile->clearSelection();
    onlineFile->setModel(mOnlineFile);
#ifdef __linux__
    onlineFile->setRootIndex(mOnlineFile->setRootPath("/home/link/smbtmp/"));
#else
    onlineFile->setRootIndex(mOnlineFile->setRootPath("\\\\192.168.1.55\\share\\二代升级文件"));
#endif
    onlineFile->setHeaderHidden(true);
    onlineFile->hideColumn(3);
    onlineFile->hideColumn(2);
    onlineFile->hideColumn(1);
}

void TcpOnline::recvFileHead()
{
    qDebug() << "recv file" << recvFile->text();
    if (currentPort.isEmpty()) {
        qDebug() << "empty";
    } else {
        tmpMsg.insert(AddrEnum, Qt::Key_Zoom);
        tmpMsg.insert(AddrRecv, recvFile->text());
        tmpMsg.insert(AddrPort, currentPort.toInt());
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
    }
}

void TcpOnline::sendShellCmd()
{
    qDebug() << "shell cmd" << shellCmd->text();
    if (currentPort.isEmpty()) {
        qDebug() << "empty";
    } else {
        tmpMsg.insert(AddrEnum, Qt::Key_Zoom);
        tmpMsg.insert(AddrData, shellCmd->text());
        tmpMsg.insert(AddrPort, currentPort.toInt());
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
    }
}

void TcpOnline::sendFileHead()
{
    qDebug() << "send file" << sendFile->text();
    if (currentPort.isEmpty()) {
        qDebug() << "empty";
    } else {
        tmpMsg.insert(AddrEnum, Qt::Key_Zoom);
        tmpMsg.insert(AddrSend, sendFile->text());
        tmpMsg.insert(AddrPort, currentPort.toInt());
        emit sendAppMsg(tmpMsg);
        tmpMsg.clear();
    }
}

void TcpOnline::updateSqlite()
{
    mOnlineView = new QSqlTableModel(this, QSqlDatabase::database(sqlName));
    mOnlineView->setTable("aip_online");
    QStringList headers;
    headers << "记录编号" << "设备编号" << "设备地址" << "出厂编号"
            << "网络端口" << "网络地址" << "记录时间" << "设备状态" << "软件版本";
    if (mOnlineView->columnCount() >= headers.size()) {
        for (int i=0; i < qMin(headers.size(), mOnlineView->columnCount()); i++) {
            mOnlineView->setHeaderData(i, Qt::Horizontal, headers.at(i));
        }
        onlineView->setModel(mOnlineView);
        onlineView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        onlineView->hideColumn(0);
        onlineView->hideColumn(1);
        onlineView->hideColumn(4);
        onlineView->hideColumn(7);
        mOnlineView->select();
    }

    mDeviceView = new QSqlTableModel(this, QSqlDatabase::database(sqlName));
    mDeviceView->setTable("aip_device");
}

void TcpOnline::updateFilter(int col, QString msg)
{
    QSqlRecord record = mOnlineView->record();
    QString modelFilter;
    filters[col] = msg;
    for (int i=0; i < record.count(); ++i)  {
        if (i != 0)  {
            modelFilter += " and ";
        }
        QString field = record.fieldName(i);
        QString subFilter = QString("%1 LIKE '%%%2%%'").arg(field).arg(filters[i]);
        modelFilter += subFilter;
    }
    mOnlineView->setFilter(modelFilter);
    mOnlineView->select();
}

void TcpOnline::recvDisplay(QString msg)
{
    qDebug() << msg;
    if (msg.startsWith("./")) {
        QString temp = msg;
        QStringList FileList = temp.split("\n", QString::SkipEmptyParts);
        for (int i=0; i < FileList.size(); i++) {
            QString t = FileList.at(i);
            if (t.endsWith("@") || t.endsWith("|") || t.endsWith("=") || t == "./") {
                continue;
            }
            QTreeWidgetItem *item = new QTreeWidgetItem;
            if (t.endsWith("/")) {
                item->setText(0, t.remove("/"));
                QPixmap pixmap("./image/dir.png");
                item->setIcon(0, pixmap);
                item->setText(1, "文件夹");
            } else if (t.endsWith("*")) {
                item->setText(0, t.remove("*"));
                QPixmap pixmap("./image/link.png");
                item->setIcon(0, pixmap);
                item->setText(1, "文件");
            } else {
                item->setText(0, t.remove("*"));
                QPixmap pixmap("./image/file.png");
                item->setIcon(0, pixmap);
                item->setText(1, "文件");
            }
            onlineTree->addTopLevelItem(item);
        }
    } else {
        text->setPlainText(msg);
    }
}

void TcpOnline::recvTarget()
{
    for (int i=0; i < mOnlineView->rowCount(); i++) {
        if (mOnlineView->index(i, 2).data().toString() == "F4:5E:AB:2C:72:DD") {
            clickOnline(mOnlineView->index(i, 2));
            isClick = true;
            break;
        }
    }
}

void TcpOnline::recvAppMsg(QTmpMap msg)
{
    int c = msg.value(AddrEnum).toInt();
    switch (c) {
    case Qt::Key_Copy:
        tmpSet = msg;
        if (!isClick)
            QTimer::singleShot(500, this, SLOT(recvTarget()));
        break;
    default:
        break;
    }
}

void TcpOnline::showEvent(QShowEvent *e)
{
    this->setFocus();
    mOnlineView->select();
    updateFile();
    e->accept();
}

