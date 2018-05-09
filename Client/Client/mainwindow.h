#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDockWidget>
#include <QPushButton>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QWidget *mainWidget, *dockWidget;
    QHBoxLayout *mainHLayout;
    QVBoxLayout *dockVLayout;
    QDockWidget *sidebarDock;
    QPushButton *overviewBtn;

    void createDockWindow();
    void createContentWindow();
    void setWindowStyleSheet();
};

#endif // MAINWINDOW_H
