#ifndef MOSTUSED_H
#define MOSTUSED_H

#include <QWidget>
#include <QVBoxLayout>

class MostUsed : public QWidget
{
    Q_OBJECT
public:
    explicit MostUsed(QWidget *parent = nullptr);

private:
    QVBoxLayout *mainVLayout, *contVLayout;

    void setWindowStyleSheet();

protected:
    void paintEvent(QPaintEvent *);

signals:

private slots:
    void appChanged(QString);
};

#endif // MOSTUSED_H
