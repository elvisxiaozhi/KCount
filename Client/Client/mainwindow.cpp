#include "mainwindow.h"
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    sidebar = new Sidebar();
    addDockWidget(Qt::LeftDockWidgetArea, sidebar);
    createContentWindow();
}

MainWindow::~MainWindow()
{

}

void MainWindow::createContentWindow()
{
    contWidget = new QWidget(this);
    contHLayout = new QHBoxLayout(contWidget);
    contHLayout->setMargin(0);

    QLabel *lbl = new QLabel;
    lbl->setText("Content");
    lbl->setAlignment(Qt::AlignCenter);
    contHLayout->addWidget(lbl);

    setCentralWidget(contWidget);
    contWidget->setLayout(contHLayout);
}
