#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include "sidebar.h"
#include "overview.h"
#include "database.h"
#include <QSystemTrayIcon>
#include "dashboard.h"
#include "users.h"
#include "settings.h"
#include <QThread>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Database *database;
    QThread dbThread;
    Sidebar *sidebar;
    QWidget *contentWidget;
    QVBoxLayout *contVLayout;
    QVector<QWidget *> contentVec;
    Overview *overview;
    Dashboard *dashboard;
    Users *users;
    Settings *settings;
    QSystemTrayIcon *sysTrayIcon;
    QAction *startOnBootAct;

    void createDBThread();
    void createSidebar();
    void createContentWindow();
    void createSystemTrayIcon();

private slots:
    void sysTrayIconActivated(QSystemTrayIcon::ActivationReason);
    void changeContent(int);
    void startOnBootActChanged();
};

#endif // MAINWINDOW_H
