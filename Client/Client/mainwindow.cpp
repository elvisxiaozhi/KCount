#include "mainwindow.h"
#include <QLabel>
#include <QMenu>
#include <QCoreApplication>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createDBThread();
    createSidebar();
    createContentWindow();
    createSystemTrayIcon();
    setMinimumSize(1100, 800);

    connect(sidebar, &Sidebar::actionChanged, this, &MainWindow::changeContent);
}

MainWindow::~MainWindow()
{
    dbThread.quit();
    dbThread.wait();
}

void MainWindow::createDBThread()
{
    database = new Database;
    database->moveToThread(&dbThread);
    connect(&dbThread, &QThread::finished, database, &QObject::deleteLater);
    dbThread.start();
}

void MainWindow::createSidebar()
{
    sidebar = new Sidebar();
    addDockWidget(Qt::LeftDockWidgetArea, sidebar);
}

void MainWindow::createContentWindow()
{
    contentWidget = new QWidget(this);
    contVLayout = new QVBoxLayout(contentWidget);
    contentWidget->setLayout(contVLayout);
    setCentralWidget(contentWidget);

    overview = new Overview;
    dashboard = new Dashboard;
    contentVec.push_back(overview);
    contentVec.push_back(dashboard);
    contentVec.resize(4);
    dashboard->hide();
    contVLayout->addWidget(overview);
    contVLayout->addWidget(dashboard);
}

void MainWindow::createSystemTrayIcon()
{
    sysTrayIcon = new QSystemTrayIcon(QIcon(":/Resources/Icons/letter_k.png"), this);
    sysTrayIcon->show();

    QMenu *menu = new QMenu;

    QAction *quitAct = new QAction(tr("Quit"), menu);

    menu->addAction(quitAct);
    sysTrayIcon->setContextMenu(menu);

    connect(sysTrayIcon, &QSystemTrayIcon::activated, this, &MainWindow::sysTrayIconActivated);
    connect(quitAct, &QAction::triggered, [this](){ overview->updateDatabase(); qApp->quit(); });
}

void MainWindow::sysTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == 2 || reason == 3) { //tray icon was double clicked or clicked
        this->showNormal(); //to show a normal size of the main window
    }
}

void MainWindow::changeContent(int index)
{
    if(index == 0) {
        contentVec[0]->show();
        contentVec[1]->hide();
    }
    if(index == 1) {
        contentVec[0]->hide();
        contentVec[1]->show();
    }
}
