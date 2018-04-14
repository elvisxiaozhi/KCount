#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSystemTrayIcon>
#include <QAction>
#include <QToolButton>
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
    QHBoxLayout *nextPageHLayout;
    QSystemTrayIcon *trayIcon;
    QLabel *keyPressedTimesLabel;
    QToolButton *nextArrowBtn;
    unsigned long long int keyPressedTimes;
    QAction *startOnBootAction;
    QMap<QString, unsigned long long int> pressedKeyMap;
    void setLayout();
    void setTrayIcon();

private slots:
    void keyPressed(QString);
    void trayIconActivated(QSystemTrayIcon::ActivationReason);
    void closeEvent(QCloseEvent *);
};

#endif // MAINWINDOW_H
