#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>
#include <QAction>

class Sidebar : public QWidget
{
    Q_OBJECT
public:
    explicit Sidebar(QWidget *parent = nullptr);
    QAction *addAction(const QString &, const QIcon &);
    QSize minimumSizeHint() const;

private:
    QList<QAction *> actList;

    void paintEvent(QPaintEvent *);

signals:

public slots:
};

#endif // SIDEBAR_H
