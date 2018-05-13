#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include "sidebar.h"
#include "overview.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Sidebar *sidebar;
    Overview *overview;

    void createSidebar();
    void createContentWindow();
};

#endif // MAINWINDOW_H
