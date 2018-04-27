#ifndef TYPSETACW_H
#define TYPSETACW_H

#include <QLabel>
#include <QDebug>
#include <QLayout>
#include <QWidget>
#include <QVariant>
#include <QCheckBox>
#include <QGroupBox>
#include <QComboBox>
#include <QLineEdit>
#include <QShowEvent>
#include <QMessageBox>
#include <QPushButton>

class TypSetAcw : public QWidget
{
    Q_OBJECT
public:
    explicit TypSetAcw(QWidget *parent = 0);
signals:
    void sendAppMap(QVariantMap msg);
private slots:
    void initUI();
    void initLayout();
    void initBoxInrAG();
    void initBoxAcwAC();
    void initBoxAcwAL();
    void initBoxAcwLC();
    void initButtons();
    void initSettings();
    void saveSettings();
    void recvAppMap(QVariantMap msg);
    virtual void showEvent(QShowEvent *e);
private:
    QVBoxLayout *boxLayout;
    QList<QCheckBox*> boxCheck;
    QVariantMap config;
};

#endif // TYPSETACW_H
