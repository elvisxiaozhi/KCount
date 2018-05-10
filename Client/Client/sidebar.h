#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>
#include <QAction>

class SideBar : public QWidget
{
    Q_OBJECT
public:
    explicit SideBar(QWidget *parent = nullptr);

private:
    QAction *checkedAction;
    QAction *overAction;

signals:

public slots:
};

#endif // SIDEBAR_H
