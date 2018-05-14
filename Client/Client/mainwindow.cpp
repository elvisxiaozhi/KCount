#include "mainwindow.h"
#include <QLabel>
#include "hook.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    Hook hook;
//    setDatabaseThread();
    createSidebar();
    createContentWindow();
    setMinimumSize(1100, 800);
}

MainWindow::~MainWindow()
{
    dbThread.quit();
    dbThread.wait();
}

//void MainWindow::setDatabaseThread()
//{
//    database = new Database;
//    database->moveToThread(&dbThread);

//    connect(&dbThread, &QThread::finished, database, &QObject::deleteLater);

//    dbThread.start();
//}

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
