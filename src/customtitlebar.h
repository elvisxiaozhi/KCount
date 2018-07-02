#ifndef CUSTOMTITLEBAR_H
#define CUSTOMTITLEBAR_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QAction>

class CustomTitleBar : public QWidget
{
    Q_OBJECT
public:
    explicit CustomTitleBar(QWidget *parent = nullptr);

private:
    QList<QAction *> actList;
    QAction *checkedAct;
    QAction *hoveredAct;
    bool hoveredOnNotify;

    QAction *addAction(const QString &, const QIcon &);
    QAction *actionAt(const QPoint &);

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);

signals:
    void actionChanged(int);
};

#endif // CUSTOMTITLEBAR_H
