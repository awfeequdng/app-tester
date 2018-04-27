#ifndef TYPSETIMP_H
#define TYPSETIMP_H

#include <QLabel>
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

#include "lib/qcustomplot.h"

#define PI 3.1415926

class TypSetImp : public QWidget
{
    Q_OBJECT
public:
    explicit TypSetImp(QWidget *parent = 0);
signals:
    void sendAppMap(QVariantMap msg);
private slots:
    void initUI();
    void initLayout();
    void initContent();
    void initWaveBar();
    void initButtons();
    void drawImpWave();
private:
    QVBoxLayout *boxLayout;
    QCustomPlot *impView;
};

#endif // TYPSETIMP_H
