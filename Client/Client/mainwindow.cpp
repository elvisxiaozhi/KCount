#include "mainwindow.h"
#include <QLabel>
#include "hook.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    Hook hook;
    createSidebar();
    createContentWindow();
    setMinimumSize(1100, 800);
}

MainWindow::~MainWindow()
{

}

void MainWindow::createSidebar()
{
    sidebar = new Sidebar();
    addDockWidget(Qt::LeftDockWidgetArea, sidebar);
}

void MainWindow::createContentWindow()
{
    overview = new Overview();
    setCentralWidget(overview);
}
