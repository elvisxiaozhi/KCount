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
    overviewBtn->setIcon(QIcon(":/Icons/overview.png"));
    overviewBtn->setText("Overview");
    overviewBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    dockVLayout->addWidget(overviewBtn);
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
                "QMainWindow { background-color: #333333; }"
                "#DockWidget { background-color: #17202A; }"
                "QLabel { color: white; }"
                "QToolButton { color: white; }"
                );
}
