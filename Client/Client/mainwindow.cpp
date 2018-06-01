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
    setMinimumSize(1200, 900);

    connect(sidebar, &Sidebar::actionChanged, this, &MainWindow::changeContent);
}

MainWindow::~MainWindow()
{
    dbThread.quit();
    dbThread.wait();
}

void MainWindow::createDBThread()
{
    database = new Database();
    database->moveToThread(&dbThread);
    connect(&dbThread, &QThread::finished, database, &QObject::deleteLater);
    dbThread.start();
}

void MainWindow::createSidebar()
{
    sidebar = new Sidebar(this);
    addDockWidget(Qt::LeftDockWidgetArea, sidebar);
}

void MainWindow::createContentWindow()
{
    contentWidget = new QWidget(this);
    contVLayout = new QVBoxLayout(contentWidget);
    contentWidget->setLayout(contVLayout);
    setCentralWidget(contentWidget);

    overview = new Overview(contentWidget);
    dashboard = new Dashboard(contentWidget);
    users = new Users(contentWidget);
    settings = new Settings(contentWidget);

    contentVec.push_back(overview);
    contentVec.push_back(dashboard);
    contentVec.push_back(users);
    contentVec.push_back(settings);
    contentVec.resize(4);
    dashboard->hide();
    users->hide();
    settings->hide();
    contVLayout->addWidget(overview);
    contVLayout->addWidget(dashboard);
    contVLayout->addWidget(users);
    contVLayout->addWidget(settings);
}

void MainWindow::createSystemTrayIcon()
{
    sysTrayIcon = new QSystemTrayIcon(QIcon(":/Resources/Icons/letter_k.png"), this);
    sysTrayIcon->show();

    QMenu *menu = new QMenu(this);

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
    for(auto content : contentVec) {
        if(content == contentVec[index]) {
            content->show();
        }
        else {
            content->hide();
        }
    }
    if(index == 1) {
        emit dashboard->loadingData();
    }
}
