#ifndef MOSTPRESSED_H
#define MOSTPRESSED_H

#include <QWidget>
#include <QLabel>
#include <QMap>
#include <QVBoxLayout>
#include <label.h>

class MostPressed : public QWidget
{
    Q_OBJECT
public:
    explicit MostPressed(QWidget *parent = nullptr);

private:
    QVBoxLayout *mainVLayout, *contVLayout;
    QLabel *title;
    QVector<Label *> contents;
    QVector<std::pair<QString, unsigned long int>> mostPressed;
    QMap<QString, unsigned long int> tempKeyMap;
    int currentHour;

    void setWindowStyleSheet();
    void setContents();

protected:
    void paintEvent(QPaintEvent *);

public slots:
    void updateDatabase();
    void reloadData(int);

private slots:
    void keyPressed(QString);
};

#endif // MOSTPRESSED_H
