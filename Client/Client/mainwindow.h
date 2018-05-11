#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDockWidget>
#include <QToolButton>
#include "sidebar.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QWidget *mainWidget, *dockWidget, *contentWidget;
    QHBoxLayout *sidebarHLayout;
    QHBoxLayout *mainHLayout, *contentHLayout;
    Sidebar *sidebar;
    QDockWidget *sidebarDock;
    QToolButton *overviewBtn, *dashboardBtn, *usersBtn, *settingsBtn;

    void createMainWindow();
    void createDockWindow();
    void createContentWindow();
    void setWindowStyleSheet();
};

#endif // MAINWINDOW_H
