#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include "sidebar.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Sidebar *sidebar;
    QWidget *contWidget;
    QHBoxLayout *contHLayout;

    void createContentWindow();
};

#endif // MAINWINDOW_H
