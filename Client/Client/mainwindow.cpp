#include "mainwindow.h"
#include <QLabel>
#include "hook.h"
#include <QMenu>
#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    Hook hook;
    createSidebar();
    createContentWindow();
    createSystemTrayIcon();
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
//        setLblTextAndColor();
    }
}
