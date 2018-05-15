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

class Overview : public QWidget
{
    Q_OBJECT
public:
    explicit Overview(QWidget *parent = nullptr);

private:
    QHBoxLayout *timeSpanHLayout;
    QVBoxLayout *mainVLayout;
    QGridLayout *lblGLayout;
    QComboBox *timeSpanBox;
    TotalPressed *totalPressed;
    MostPressed *mostPressed;
    QTimer *timer;

    void setWindowLayout();
    void setWindowStyleSheet();
    void setTimeSpanBox();
    void setLbls();
    void setTimer();

protected:
    void paintEvent(QPaintEvent *);

signals:
    void updateDatabase() const;

public slots:

private slots:
    void updateTimer() const;
};

#endif // OVERVIEW_H
