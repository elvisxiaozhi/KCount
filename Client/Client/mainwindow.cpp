#include "mainwindow.h"
#include "windows.h"
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createMainWindow();
    createDockWindow();
    createContentWindow();
    setWindowStyleSheet();
}

MainWindow::~MainWindow()
{

}

void MainWindow::createMainWindow()
{
    mainWidget = new QWidget(this);
    mainHLayout = new QHBoxLayout(mainWidget);
    mainHLayout->setMargin(0);
    sidebarHLayout = new QHBoxLayout(); //do not set parent
    contentWidget = new QWidget(mainWidget);
    contentHLayout = new QHBoxLayout(contentWidget);

    contentWidget->setLayout(contentHLayout);
    mainHLayout->addLayout(sidebarHLayout);
    mainHLayout->addWidget(contentWidget);
    mainWidget->setLayout(mainHLayout);
    setCentralWidget(mainWidget);
}

void MainWindow::createDockWindow()
{
    sidebar = new Sidebar();
    sidebar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    sidebar->addAction(tr("Overview"), QIcon(":/Icons/home.png"));
    sidebar->addAction(tr("Dashboard"), QIcon(":/Icons/dashboard.png"));
    sidebar->addAction(tr("Users"), QIcon(":/Icons/users.png"));
    sidebar->addAction(tr("Settings"), QIcon(":/Icons/settings.png"));

    sidebarHLayout->setObjectName("Sidebar");
    sidebarHLayout->addWidget(sidebar);
}

void MainWindow::createContentWindow()
{
    contentHLayout->setMargin(0);

    QLabel *lbl = new QLabel;
    lbl->setText("Content");
    lbl->setAlignment(Qt::AlignCenter);
    contentHLayout->addWidget(lbl);
}

void MainWindow::setWindowStyleSheet()
{
    setStyleSheet(
                "QMainWindow { background-color: #e6e6fa; }"
                );
}
