#ifndef TYPOFFIMP_H
#define TYPOFFIMP_H

#include <QLabel>
#include <QLayout>
#include <QWidget>
#include <QVariant>
#include <QShowEvent>
#include <QMessageBox>
#include <QPushButton>

class TypOffImp : public QWidget
{
    Q_OBJECT
public:
    explicit TypOffImp(QWidget *parent = 0);
signals:
    void sendAppMap(QVariantMap msg);
};

#endif // TYPOFFIMP_H
