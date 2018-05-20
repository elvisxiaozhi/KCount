#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include "sidebar.h"
#include "overview.h"
#include "database.h"
#include <QSystemTrayIcon>
#include <QThread>
#include "dashboard.h"
#include "users.h"
#include "settings.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Database *database;
    Sidebar *sidebar;
    QWidget *contentWidget;
    QVBoxLayout *contVLayout;
    QVector<QWidget *> contentVec;
    Overview *overview;
    Dashboard *dashboard;
    Users *users;
    Settings *settings;
    QSystemTrayIcon *sysTrayIcon;
    QThread dbThread;

    void createDBThread();
    void createSidebar();
    void createContentWindow();
    void createSystemTrayIcon();

private slots:
    void sysTrayIconActivated(QSystemTrayIcon::ActivationReason);
    void changeContent(int);
};

#endif // MAINWINDOW_H
