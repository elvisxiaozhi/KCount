#include "mainwindow.h"
#include <QSystemTrayIcon>
#include <QMenu>
#include <QKeyEvent>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setLayout();
    setTrayIcon();
}

MainWindow::~MainWindow()
{

}

void MainWindow::setLayout()
{
    mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);

    mainVLayout = new QVBoxLayout;
    mainWidget->setLayout(mainVLayout);

    keyPressedTimes = 0; //initiate keyPressedTimes
    keyPressedTimesLabel = new QLabel(mainWidget);
    mainVLayout->addWidget(keyPressedTimesLabel);
    keyPressedTimesLabel->setAlignment(Qt::AlignCenter);
    keyPressedTimesLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    keyPressedTimesLabel->setStyleSheet("QLabel { background-color: #FAD7A0; color: #8E44AD; font-size: 50px; }");
    keyPressedTimesLabel->setText(QString::number(keyPressedTimes));
    connect(this, &MainWindow::keyPressed, [this](){ keyPressedTimesLabel->setText(QString::number(keyPressedTimes)); });
}

void MainWindow::setTrayIcon()
{
    QSystemTrayIcon *trayIcon = new QSystemTrayIcon(QIcon(":/keyboard_tray_icon.png"), this);
    trayIcon->show();

    QMenu *trayIconMenu = new QMenu;
    trayIcon->setContextMenu(trayIconMenu);

    startOnBootAction = new QAction("Start on Boot", trayIcon);
    trayIconMenu->addAction(startOnBootAction);
    startOnBootAction->setCheckable(true);
    if(startOnBootAction->isChecked()) {
        startOnBootAction->setChecked(true);
    }
    else {
        startOnBootAction->setCheckable(false);
    }
    QAction *settingsAction = new QAction("Settings", trayIconMenu);
    trayIconMenu->addAction(settingsAction);

    QMenu *helpMenu = new QMenu("Help");
    trayIconMenu->addMenu(helpMenu);
    QAction *feedbackAction = new QAction("Feedback", helpMenu);
    helpMenu->addAction(feedbackAction);
    QAction *aboutAction = new QAction("About", helpMenu);
    helpMenu->addAction(aboutAction);
    QAction *donateAction = new QAction("Donate", helpMenu);
    helpMenu->addAction(donateAction);

    QAction *quitAction = new QAction("Quit", trayIconMenu);
    trayIconMenu->addAction(quitAction);

//    connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::trayIconActivated);
//    connect(startOnBootAction, &QAction::changed, this, &MainWindow::startOnBootActionChanged);
//    connect(settingsAction, &QAction::triggered, [this](){ this->activateWindow(); });
    connect(quitAction, &QAction::triggered, [this, trayIcon](){ trayIcon->setVisible(false); this->close(); }); //note the program can be only closed by clicking "Quit" action
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    keyPressedTimes++;
    qDebug() << event->text();
    emit keyPressed();
}

bool MainWindow::event(QEvent *event)
{
    if(event->type() == QEvent::WindowDeactivate) {
        qDebug() << "yes_1";
        QKeyEvent *testingE = new QKeyEvent(QEvent::KeyPress, Qt::Key_Tab, Qt::NoModifier);
        QCoreApplication::postEvent(this, testingE);
    }
    return QWidget::event(event);
}
