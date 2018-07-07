#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include "sidebar.h"
#include "overview/overview.h"
#include "core/database.h"
#include <QSystemTrayIcon>
#include "dashboard/dashboard.h"
#include "users/users.h"
#include "settings/settings.h"
#include <QThread>
#include "customtitlebar.h"
#include "notification.h"

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
    CustomTitleBar *titleBar;
    QVector<QWidget *> contentVec;
    Overview *overview;
    Dashboard *dashboard;
    Users *users;
    Settings *settings;
    QSystemTrayIcon *sysTrayIcon;
    QAction *startOnBootAct;
    Notification *notification;

    void createDBThread();
    void createSidebar();
    void createContentWindow();
    void createSystemTrayIcon();
    bool event(QEvent *);

private slots:
    void sysTrayIconActivated(QSystemTrayIcon::ActivationReason);
    void changeContent(int);
    void sidebarActChanged(int);
    void startOnBootActChanged();
    void showNotification(QString);
};

#endif // MAINWINDOW_H
