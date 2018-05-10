#include "mainwindow.h"
#include "windows.h"
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createDockWindow();
    createContentWindow();
    setWindowStyleSheet();
}

MainWindow::~MainWindow()
{

}

void MainWindow::createDockWindow()
{
    sidebarDock = new QDockWidget(this);
    addDockWidget(Qt::LeftDockWidgetArea, sidebarDock);

    //hide dock widget title bar
    QWidget *titleBarWidget = new QWidget(sidebarDock);
    sidebarDock->setTitleBarWidget(titleBarWidget);
    sidebarDock->titleBarWidget()->hide();

    dockWidget = new QWidget(sidebarDock);
    dockWidget->setObjectName("DockWidget");
    dockVLayout = new QVBoxLayout(dockWidget);
    dockVLayout->setMargin(0);

    overviewBtn = new QToolButton(dockWidget);
    overviewBtn->setAutoRaise(true);
    overviewBtn->setIcon(QIcon(":/Icons/home.png"));
    overviewBtn->setText("Overview");
    overviewBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    dashboardBtn = new QToolButton(dockWidget);
    dashboardBtn->setAutoRaise(true);
    dashboardBtn->setIcon(QIcon(":/Icons/dashboard.png"));
    dashboardBtn->setText("Dashboard");
    dashboardBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    usersBtn = new QToolButton(dockWidget);
    usersBtn->setAutoRaise(true);
    usersBtn->setIcon(QIcon(":/Icons/users.png"));
    usersBtn->setText("Users");
    usersBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    settingsBtn = new QToolButton(dockWidget);
    settingsBtn->setAutoRaise(true);
    settingsBtn->setIcon(QIcon(":/Icons/settings.png"));
    settingsBtn->setText("Settings");
    settingsBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    dockVLayout->addWidget(overviewBtn);
    dockVLayout->addWidget(dashboardBtn);
    dockVLayout->addWidget(usersBtn);
    dockVLayout->addWidget(settingsBtn);
    dockVLayout->addStretch();
    dockWidget->setLayout(dockVLayout);
    sidebarDock->setWidget(dockWidget);
}

void MainWindow::createContentWindow()
{
    mainWidget = new QWidget(this);
    mainHLayout = new QHBoxLayout(mainWidget);
    mainHLayout->setMargin(0);
    setCentralWidget(mainWidget);
    mainWidget->setLayout(mainHLayout);

    QLabel *lbl = new QLabel;
    lbl->setText("Content");
    lbl->setAlignment(Qt::AlignCenter);
    mainHLayout->addWidget(lbl);
}

void MainWindow::setWindowStyleSheet()
{
    setStyleSheet(
                "QMainWindow { background-color: #e6e6fa; }"
                "#DockWidget { background-color: #f0f8ff; }"
                "QLabel { color: #666666; }"
                "QToolButton { color: #333333; font: 15px; padding: 10px 0px; margin: 5px; }"
                "QToolButton:hover { background-color: #f5f5f5; }"

                );
}
