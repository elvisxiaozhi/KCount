#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QDockWidget>
#include <QAction>
#include <QObject>

class Sidebar : public QDockWidget
{
    Q_OBJECT
public:
    Sidebar(QWidget *parent = nullptr);

private:
    QList<QAction *> actList;
    QAction *checkedAct;
    QAction *hoveredAct;

    QAction *addAction(const QString &, const QIcon &);
    QAction *actionAt(const QPoint &);

signals:
    void actionChanged(int);

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
};

#endif // SIDEBAR_H
