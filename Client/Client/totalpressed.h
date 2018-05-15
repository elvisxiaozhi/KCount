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
    unsigned long int totalPressedTimes;

    void setWindowStyleSheet();
    void setContColor(const unsigned long int &);

protected:
    void paintEvent(QPaintEvent *);

signals:

public slots:
    void reloadData(int);

private slots:
    void keyPressed(QString);
};

#endif // TOTALPRESSED_H
