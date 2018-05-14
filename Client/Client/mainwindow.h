#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include "sidebar.h"
#include "overview.h"
#include "database.h"
#include <QThread>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Database database;
    QThread dbThread;
    Sidebar *sidebar;
    Overview *overview;

    void setDatabaseThread();
    void createSidebar();
    void createContentWindow();
};

#endif // MAINWINDOW_H
