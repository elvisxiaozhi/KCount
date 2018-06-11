#ifndef TOTALPRESSED_H
#define TOTALPRESSED_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include "label.h"
#include "customlabel.h"
#include <QHBoxLayout>

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
    CustomLabel *swicthLbl;

    void setWindowStyleSheet();

protected:
    void paintEvent(QPaintEvent *);

public slots:
    void reloadData(int);

private slots:
    void keyPressed(QString);
};

#endif // TOTALPRESSED_H
