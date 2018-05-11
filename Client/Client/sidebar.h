#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QDockWidget>
#include <QAction>

class Sidebar : public QDockWidget
{
public:
    Sidebar();

private:
    QList<QAction *> actList;

    QAction *addAction(const QString &, const QIcon &);
protected:
    void paintEvent(QPaintEvent *);
};

#endif // SIDEBAR_H
