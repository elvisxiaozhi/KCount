#include "mainwindow.h"
#include <QLabel>
#include <QMenu>
#include <QCoreApplication>
#include <QDebug>
#include "initialisation.h"
#include "hook.h"
#include "settings.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createDBThread();
    createSidebar();
    createContentWindow();
    createSystemTrayIcon();
    setFixedSize(1250, 800);

    connect(sidebar, &Sidebar::actionChanged, this, &MainWindow::changeContent);
    connect(titleBar, &CustomTitleBar::actionChanged, this, &MainWindow::sidebarActChanged);
    connect(overview, &Overview::newDayComes, dashboard, &Dashboard::newDayComes);
    connect(overview, &Overview::limitAppAlert, this, &MainWindow::showNotification);
}

MainWindow::~MainWindow()
{
    dbThread.quit();
    dbThread.wait();
    delete notification;
//    delete database; //this line will cause the program to quit unsuccessfully
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
    setWindowTitle("Nana");
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint); //hide title bar

    contentWidget = new QWidget(this);
    setCentralWidget(contentWidget);

    contVLayout = new QVBoxLayout();
    contVLayout->setContentsMargins(0, 0, 0, 0);
    contentWidget->setLayout(contVLayout);

    titleBar = new CustomTitleBar(contentWidget);

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

    contVLayout->addWidget(titleBar);

    contVLayout->addWidget(overview);
    contVLayout->addWidget(dashboard);
    contVLayout->addWidget(users);
    contVLayout->addWidget(settings);

    //notification needs to be created after database
    //also note do not set parent for notification
    notification = new Notification();
}

void MainWindow::createSystemTrayIcon()
{
    sysTrayIcon = new QSystemTrayIcon(QIcon(":/icons/letter-n.png"), this);
    sysTrayIcon->show();
    sysTrayIcon->setToolTip(tr("Nana 2.0 Beta"));

    QMenu *menu = new QMenu(this);

    startOnBootAct = new QAction(tr("Start on Boot"), sysTrayIcon);
    startOnBootAct->setCheckable(true);

    QAction *quitAct = new QAction(tr("Quit"), menu);
    quitAct->setIcon(QIcon(":/icons/quit.png"));

    menu->addAction(startOnBootAct);
    menu->addAction(quitAct);
    sysTrayIcon->setContextMenu(menu);

    connect(sysTrayIcon, &QSystemTrayIcon::activated, this, &MainWindow::sysTrayIconActivated);
    connect(startOnBootAct, &QAction::changed, this, &MainWindow::startOnBootActChanged);
    connect(quitAct, &QAction::triggered, [this](){ Initialisation::quit = true; overview->updateDatabase(); qApp->quit(); });
}

bool MainWindow::event(QEvent *event)
{
    if(event->type() == QEvent::WindowActivate) {
        for(int i = 0; i < contentVec.size(); ++i) {
            if(!contentVec[i]->isHidden()) {
                if(i == 0) {
                    emit overview->loadingData();
                }
                if(i == 1) {
                    emit dashboard->loadingData();
                }
            }
        }
    }
    return QWidget::event(event);
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
    if(index == 0) {
        emit overview->loadingData();
    }
    if(index == 1) {
        emit dashboard->loadingData();
    }
}

void MainWindow::sidebarActChanged(int index)
{
    if(index == 0) {
        this->showMinimized();
    }
    if(index == 1) {
        this->close();
    }
}

void MainWindow::startOnBootActChanged()
{
    if(startOnBootAct->isChecked()) {
        Settings::startOnBootSettings.setValue("Nana", QCoreApplication::applicationFilePath().replace('/', '\\'));
    }
    else {
        Settings::startOnBootSettings.remove("Nana");
    }
}

void MainWindow::showNotification(QString appName)
{
    if(Initialisation::settings.value("InitSettings/isLimitsOn") == true) {
        notification->setLabelText(appName);
        notification->show();
    }
}
