#ifndef TYPCONFIG_H
#define TYPCONFIG_H

#include <QDebug>
#include <QLabel>
#include <QLayout>
#include <QWidget>
#include <QVariant>
#include <QLineEdit>
#include <QGroupBox>
#include <QShowEvent>
#include <QHeaderView>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>

#include <QSqlQuery>
#include <QSqlError>
#include <QStringList>
#include <QSqlDatabase>

#include "boxqitems.h"

#define C_ROW 16
#define C_COL 2

const QString ConfigAddr = QString::number(0x0050);

class TypConfig : public QWidget
{
    Q_OBJECT
public:
    explicit TypConfig(QWidget *parent = 0);
signals:
    void sendAppMap(QVariantMap msg);
private slots:
    void initUI();
    void initLayout();
    void initViewBar();
    void initConfigBar();
    void initButtonBar();
    void initSettings();
    void appendModelType();
    void selectModelType();
    void deleteModelType();
    void clickButtons();
    void clickViewBar();
    void recvAppMap(QVariantMap msg);
    virtual void showEvent(QShowEvent *e);
private:
    QTableWidget *view;
    QVariantMap tmpMap;
    QVariantMap config;
    QLineEdit *page;
    QLabel *curr;
    QLineEdit *numb;
    QLineEdit *type;
    QStringList groups;
    QVBoxLayout *boxLayout;
    QFrame *setFrame;
};

#endif // TYPCONFIG_H
