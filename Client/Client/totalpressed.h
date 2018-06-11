#ifndef TOTALPRESSED_H
#define TOTALPRESSED_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include "label.h"
#include <QHBoxLayout>
#include "custombutton.h"

class TotalPressed : public QWidget
{
    Q_OBJECT
public:
    explicit TotalPressed(QWidget *parent = nullptr);

private:
    QVBoxLayout *mainVLayout;
    QHBoxLayout *lblHLayout;
    QLabel *title;
    Label *content;
    unsigned long int totalPressedTimes;
    CustomButton *switchBtn;

    void setWindowStyleSheet();

protected:
    void paintEvent(QPaintEvent *);

public slots:
    void reloadData(int);

private slots:
    void keyPressed(QString);
};

#endif // TOTALPRESSED_H
