/*******************************************************************************
 * Copyright [2018] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       虚拟键盘
*******************************************************************************/
#ifndef BOXQINPUT_H
#define BOXQINPUT_H

#include <QTimer>
#include <QTime>
#include <QLayout>
#include <QWidget>
#include <QMutex>
#include <QDebug>
#include <QLineEdit>
#include <QComboBox>
#include <QShowEvent>
#include <QPushButton>
#include <QMouseEvent>
#include <QButtonGroup>
#include <QApplication>

#ifdef __arm__
#include <QWSServer>
#endif

class BoxQInput : public QWidget
{
    Q_OBJECT
public:
    explicit BoxQInput(QWidget *parent = 0);

public:
    static BoxQInput *instance() {
        static QMutex mutex;
        if (!_instance) {
            QMutexLocker locker(&mutex);
            if (!_instance) {
                _instance = new BoxQInput;
            }
        }
        return _instance;
    }
private slots:
    void initUI();
    void initSkin();
    void initFilter();
    void initLayout();
    void initButton();
    void clickButton(int id);
    void focusChanged(QWidget *oldWidget, QWidget *nowWidget);
    void showEvent(QShowEvent *);
private:
    static BoxQInput *_instance;
    bool upper;
    QWidget *currentWidget;
    QVBoxLayout *btnLayout;
    QButtonGroup *btns;
};

#endif // BOXQINPUT_H
