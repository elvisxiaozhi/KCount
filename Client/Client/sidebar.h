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
    QAction *checkedAct;
    QAction *hoveredAct;

    QAction *addAction(const QString &, const QIcon &);
    QAction *actionAt(const QPoint &);

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
};

#endif // SIDEBAR_H
