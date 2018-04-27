#ifndef TYPOFFDCR_H
#define TYPOFFDCR_H

#include <QLabel>
#include <QLayout>
#include <QWidget>
#include <QVariant>
#include <QShowEvent>
#include <QMessageBox>
#include <QPushButton>

class TypOffDcr : public QWidget
{
    Q_OBJECT
public:
    explicit TypOffDcr(QWidget *parent = 0);
signals:
    void sendAppMap(QVariantMap msg);
};

#endif // TYPOFFDCR_H
