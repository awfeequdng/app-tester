#ifndef TYPSETDCR_H
#define TYPSETDCR_H

#include <QLabel>
#include <QLayout>
#include <QWidget>
#include <QVariant>
#include <QShowEvent>
#include <QMessageBox>
#include <QPushButton>

class TypSetDcr : public QWidget
{
    Q_OBJECT
public:
    explicit TypSetDcr(QWidget *parent = 0);
signals:
    void sendAppMap(QVariantMap msg);
};

#endif // TYPSETDCR_H
