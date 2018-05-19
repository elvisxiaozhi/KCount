#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QWidget>

class Dashboard : public QWidget
{
    Q_OBJECT
public:
    explicit Dashboard(QWidget *parent = nullptr);

private:
    void setWindowStyleSheet();

protected:
    void paintEvent(QPaintEvent *);
signals:

public slots:
};

#endif // DASHBOARD_H
