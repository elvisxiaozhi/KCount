#ifndef OVERVIEW_H
#define OVERVIEW_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QComboBox>
#include <QLabel>
#include "totalpressed.h"

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

    void setWindowLayout();
    void setWindowStyleSheet();
    void setTimeSpanBox();
    void setLbls();

protected:
    void paintEvent(QPaintEvent *);

signals:

public slots:
};

#endif // OVERVIEW_H
