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
    QMap<QString, unsigned long long int> pressedKeyMap;
    void setLayout();
    void setTrayIcon();
};

#endif // MAINWINDOW_H
