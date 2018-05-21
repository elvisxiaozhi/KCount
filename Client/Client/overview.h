#ifndef OVERVIEW_H
#define OVERVIEW_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QComboBox>
#include <QLabel>
#include <QTimer>
#include "totalpressed.h"
#include "mostpressed.h"
#include "mouseclick.h"
#include "hook.h"

class Overview : public QWidget
{
    Q_OBJECT
public:
    explicit Overview(QWidget *parent = nullptr);
    void updateDatabase();

private:
    QHBoxLayout timeSpanHLayout;
    QVBoxLayout *mainVLayout;
    QGridLayout *lblGLayout;
    QComboBox *timeSpanBox;
    TotalPressed *totalPressed;
    MostPressed *mostPressed;
    MouseClick *mouseClick;
    QTimer *timer;
    Hook *hook;

    void setWindowLayout();
    void setWindowStyleSheet();
    void setTimeSpanBox();
    void setLbls();
    void setTimer();
    bool eventFilter(QObject *watched, QEvent *event);

protected:
    void paintEvent(QPaintEvent *);

signals:

public slots:

private slots:
    void timeout();
};

#endif // OVERVIEW_H
