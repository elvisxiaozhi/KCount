#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QSystemTrayIcon>
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
    QSystemTrayIcon *trayIcon;
    QLabel *keyPressedTimesLabel;
    unsigned long long int keyPressedTimes;
    QAction *startOnBootAction;
    QMap<QString, unsigned long long int> pressedKeyMap;
    void setLayout();
    void setTrayIcon();

private slots:
    void keyPressed();
    void trayIconActivated(QSystemTrayIcon::ActivationReason);
    void closeEvent(QCloseEvent *);
};

#endif // MAINWINDOW_H
