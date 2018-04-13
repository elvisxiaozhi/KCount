#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QAction>
#include <QLabel>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QWidget *mainWidget;
    QVBoxLayout *mainVLayout;
    QLabel *keyPressedTimesLabel;
    unsigned long long int keyPressedTimes;
    QAction *startOnBootAction;
    void setLayout();
    void setTrayIcon();
    void keyPressEvent(QKeyEvent *);
    bool event(QEvent *);

signals:
    void keyPressed();

};

#endif // MAINWINDOW_H
