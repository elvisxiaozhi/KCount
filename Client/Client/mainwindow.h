#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include "sidebar.h"
#include "overview.h"
#include "database.h"
#include <QSystemTrayIcon>
#include <QThread>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Database *database;
    Sidebar *sidebar;
    Overview *overview;
    QSystemTrayIcon *sysTrayIcon;
    QThread dbThread;

    void createDBThread();
    void createSidebar();
    void createContentWindow();
    void createSystemTrayIcon();

private slots:
    void sysTrayIconActivated(QSystemTrayIcon::ActivationReason);
};

#endif // MAINWINDOW_H
