#ifndef STATISTICS_H
#define STATISTICS_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QTabWidget>

class Statistics : public QMainWindow
{
    Q_OBJECT
public:
    explicit Statistics(QWidget *parent = nullptr);
    void setBarChart();

private:
    QWidget *mainWidget;
    QVBoxLayout *mainVLayout;
    QTabWidget *tabWidget;
    void setLayout();
    void closeEvent(QCloseEvent *);

signals:

public slots:


};

#endif // STATISTICS_H
