#include "mainwindow.h"
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
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
