#ifndef MOUSECLICK_H
#define MOUSECLICK_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

class MouseClick : public QWidget
{
    Q_OBJECT
public:
    explicit MouseClick(QWidget *parent = nullptr, int mode = 1);

private:
    QVBoxLayout *mainVLayout;
    QHBoxLayout *contHLayout;
    QLabel *title, *rightClickCont, *leftClickCont;
    unsigned long int leftClickedTimes, rightClickedTimes, tempLeftClickedTimes, tempRightClickedTimes;
    int currentHour;

    void setWindowStyleSheet();

protected:
    void paintEvent(QPaintEvent *);

public slots:
    void updateDatabase();

private slots:
    void leftClicked();
    void rightClicked();
};

#endif // MOUSECLICK_H
