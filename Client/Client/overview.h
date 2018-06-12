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
#include "mostused.h"
#include <QScrollArea>

class Overview : public QWidget
{
    Q_OBJECT
public:
    explicit Overview(QWidget *parent = nullptr);
    void updateDatabase();

private:
    QHBoxLayout *timeSpanHLayout;
    QVBoxLayout *mainVLayout;
    QGridLayout *lblGLayout;
    QScrollArea *scrollArea;
    QWidget *scrollWidget;
    QComboBox *timeSpanBox;
    QTimer *timer;
    MostUsed *mostUsedArr[5];
    TotalPressed *totalPressedArr[5];
    MostPressed *mostPressedArr[5];
    MouseClick *mouseClickArr[5];

    void setWindowLayout();
    void setWindowStyleSheet();
    void setTimeSpanBox();
    void setLbls();
    void setTimer();
    bool eventFilter(QObject *watched, QEvent *event);

protected:
    void paintEvent(QPaintEvent *);

signals:
    void loadingData();

private slots:
    void timeout();
    void loadData();
    void comboBoxChanged(int);
};

#endif // OVERVIEW_H
