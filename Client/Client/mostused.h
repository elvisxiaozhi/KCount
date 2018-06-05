#ifndef MOSTUSED_H
#define MOSTUSED_H

#include <QWidget>
#include <QVBoxLayout>
#include <QElapsedTimer>

class MostUsed : public QWidget
{
    Q_OBJECT
public:
    explicit MostUsed(QWidget *parent = nullptr);

private:
    QVBoxLayout *mainVLayout, *contVLayout;
    QElapsedTimer timer;
    QVector<std::pair<QString, float> >mostUsedVec;

    void setWindowStyleSheet();

protected:
    void paintEvent(QPaintEvent *);

signals:

private slots:
    void appChanged(QString);
};

#endif // MOSTUSED_H
