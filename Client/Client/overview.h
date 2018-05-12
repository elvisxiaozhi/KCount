#ifndef OVERVIEW_H
#define OVERVIEW_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QLabel>

class Overview : public QWidget
{
    Q_OBJECT
public:
    explicit Overview(QWidget *parent = nullptr);

private:
    QHBoxLayout *timeSpanHLayout;
    QVBoxLayout *mainVLayout;
    QComboBox *timeSpanBox;

    void setWindowLayout();
    void setWindowStyleSheet();
    void setTimeSpanBox();

protected:
    void paintEvent(QPaintEvent *);

signals:

public slots:
};

#endif // OVERVIEW_H
