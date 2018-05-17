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
    explicit MouseClick(QWidget *parent = nullptr);

private:
    QVBoxLayout *mainVLayout;
    QHBoxLayout *contHLayout;
    QLabel *title, *rightClickCont, *leftClickCont;
    unsigned long int leftClickedTimes, rightClickedTimes, tempLeftClickedTimes, tempRightClickedTimes;

    void setWindowStyleSheet();

protected:
    void paintEvent(QPaintEvent *);

signals:

public slots:
    void updateDatabase();
    void reloadData(int);

private slots:
    void leftClicked();
    void rightClicked();
};

#endif // MOUSECLICK_H
