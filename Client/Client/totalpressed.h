#ifndef TOTALPRESSED_H
#define TOTALPRESSED_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class TotalPressed : public QWidget
{
    Q_OBJECT
public:
    explicit TotalPressed(QWidget *parent = nullptr);

private:
    QVBoxLayout *mainVLayout;
    QLabel *title, *content;

    void setWindowStyleSheet();

protected:
    void paintEvent(QPaintEvent *);

signals:

public slots:

private slots:
    void keyPressed(QString);
};

#endif // TOTALPRESSED_H
