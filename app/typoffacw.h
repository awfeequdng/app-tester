#ifndef TYPOFFACW_H
#define TYPOFFACW_H

#include <QLabel>
#include <QLayout>
#include <QWidget>
#include <QVariant>
#include <QShowEvent>
#include <QMessageBox>
#include <QPushButton>

class TypOffAcw : public QWidget
{
    Q_OBJECT
public:
    explicit TypOffAcw(QWidget *parent = 0);
signals:
    void sendAppMap(QVariantMap msg);
};

#endif // TYPOFFACW_H
