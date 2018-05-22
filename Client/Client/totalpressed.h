#ifndef TOTALPRESSED_H
#define TOTALPRESSED_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include "label.h"

class TotalPressed : public QWidget
{
    Q_OBJECT
public:
    explicit TotalPressed(QWidget *parent = nullptr);

private:
    QVBoxLayout *mainVLayout;
    QLabel *title;
    Label *content;
    unsigned long int totalPressedTimes;

    void setWindowStyleSheet();

protected:
    void paintEvent(QPaintEvent *);

public slots:
    void reloadData(int);

private slots:
    void keyPressed(QString);
};

#endif // TOTALPRESSED_H
