#ifndef OVERVIEW_H
#define OVERVIEW_H

#include <QWidget>

class Overview : public QWidget
{
    Q_OBJECT
public:
    explicit Overview(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *);

private:
    void setWindowStyleSheet();

signals:

public slots:
};

#endif // OVERVIEW_H
