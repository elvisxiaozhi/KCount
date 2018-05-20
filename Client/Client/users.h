#ifndef USERS_H
#define USERS_H

#include <QWidget>

class Users : public QWidget
{
    Q_OBJECT
public:
    explicit Users(QWidget *parent = nullptr);

private:
    void setWindowStyleSheet();

protected:
    void paintEvent(QPaintEvent *);

signals:

public slots:
};

#endif // USERS_H
