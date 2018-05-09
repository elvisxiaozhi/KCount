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
    overviewBtn = new QPushButton(dockWidget);
    overviewBtn->setText("Overview");
    dockVLayout->addWidget(overviewBtn);
    dockWidget->setLayout(dockVLayout);
    sidebarDock->setWidget(dockWidget);
}

void MainWindow::createContentWindow()
{
    mainWidget = new QWidget(this);
    mainHLayout = new QHBoxLayout(mainWidget);
    mainHLayout->setContentsMargins(0, 0, 0, 0);
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
                "QMainWindow { background-color: #34495E; }"
                "#DockWidget { background-color: #212F3D; }"
                );
}
