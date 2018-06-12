#ifndef MOSTPRESSED_H
#define MOSTPRESSED_H

#include <QWidget>
#include <QLabel>
#include <QMap>
#include <QVBoxLayout>
#include <label.h>
#include <QHBoxLayout>
#include "custombutton.h"

class MostPressed : public QWidget
{
    Q_OBJECT
public:
    explicit MostPressed(QWidget *parent = nullptr);

private:
    QVBoxLayout *mainVLayout, *contVLayout;
    QHBoxLayout *lblHLayout;
    QLabel *title;
    QVector<Label *> contents;
    QVector<std::pair<QString, unsigned long int>> mostPressed;
    QMap<QString, unsigned long int> tempKeyMap;
    int currentHour;
    CustomButton *switchBtn, *showMoreBtn, *showLessBtn;

    void setWindowStyleSheet();
    void setContents();

protected:
    void paintEvent(QPaintEvent *);

signals:
    void switchBtnClicked();

public slots:
    void updateDatabase();
    void reloadData(int);

private slots:
    void keyPressed(QString);
};

#endif // MOSTPRESSED_H
