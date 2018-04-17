#include "mainwindow.h"
#include <QMenu>
#include <QKeyEvent>
#include <windows.h>
#include <QDebug>
#include "hook.h"
#include <QCloseEvent>
#include <QStatusBar>
#include <QSpacerItem>
#include <QSettings>
#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    Hook setHook;
    setLayout();
    setTrayIcon();

    statusBar()->showMessage("Total Pressed");
}

MainWindow::~MainWindow()
{

}

void MainWindow::setLayout()
{
    this->setWindowTitle("Keylogger");

    mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);

    mainVLayout = new QVBoxLayout;
    mainWidget->setLayout(mainVLayout);

    lblsVLayout = new QVBoxLayout;
    mainVLayout->addLayout(lblsVLayout);

    totalPressedTimesLabel = new Label;
    lblsVLayout->addWidget(totalPressedTimesLabel);
    totalPressedTimesLabel->setText(QString::number(setDataBase.keyPressedTimes));

    frequentlyPressedKeys.resize(5);
    for(int i = 0; i < frequentlyPressedKeys.size(); i++) {
        frequentlyPressedKeys[i] = new Label;
        lblsVLayout->addWidget(frequentlyPressedKeys[i]);
        frequentlyPressedKeys[i]->hide();
    }

    if(setDataBase.mapVector.size() > 5) {
        for(int i = 0; i < 5; i++) {
            frequentlyPressedKeys[i]->setText(setDataBase.mapVector[i].first + ": " + QString::number(setDataBase.mapVector[i].second));
        }
    }
    else {
        for(int i = 0; i < setDataBase.mapVector.size(); i++) {
            frequentlyPressedKeys[i]->setText(setDataBase.mapVector[i].first + ": " + QString::number(setDataBase.mapVector[i].second));
        }
    }

    btnHLayout = new QHBoxLayout;
    mainVLayout->addLayout(btnHLayout);

    QSpacerItem *leftBtnSpacer = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
    btnHLayout->addSpacerItem(leftBtnSpacer);

    nextPageBtn = new QToolButton(mainWidget);
    btnHLayout->addWidget(nextPageBtn);
//    nextPageBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    nextPageBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    nextPageBtn->setIcon(QIcon(":/next.png"));
    nextPageBtn->setText("Next");
    nextPageBtn->setLayoutDirection(Qt::RightToLeft);

    previousPageBtn = new QToolButton(mainWidget);
    btnHLayout->addWidget(previousPageBtn);
    previousPageBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    previousPageBtn->setIcon(QIcon(":/back.png"));
    previousPageBtn->setText("Back");
    previousPageBtn->hide();

    QSpacerItem *rightBtnSpacer = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum);
    btnHLayout->addSpacerItem(rightBtnSpacer);

    connect(nextPageBtn, &QToolButton::clicked, this, &MainWindow::showNextPage);
    connect(previousPageBtn, &QToolButton::clicked, this, &MainWindow::showPreviousPage);
    connect(&setDataBase, &DataBase::keyPressedDone, this, &MainWindow::updateLabels);
}

void MainWindow::setTrayIcon()
{
    trayIcon = new QSystemTrayIcon(QIcon(":/keyboard_tray_icon.png"), this);
    trayIcon->show();
    trayIcon->setToolTip("Keylogger Alpha Verion");

    QMenu *trayIconMenu = new QMenu;
    trayIcon->setContextMenu(trayIconMenu);

    startOnBootAction = new QAction("Start on Boot", trayIcon);
    trayIconMenu->addAction(startOnBootAction);
    startOnBootAction->setCheckable(true);

    startOnBootSettings = new QSettings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    if(startOnBootSettings->contains("Day-Counter")) {
        startOnBootAction->setChecked(true);
    }
    else {
        startOnBootAction->setChecked(false);
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

    connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::trayIconActivated);
    connect(startOnBootAction, &QAction::changed, this, &MainWindow::startOnBootActionChanged);
    connect(quitAction, &QAction::triggered, [this](){ trayIcon->setVisible(false); this->close(); }); //note the program can be only closed by clicking "Quit" action
}

void MainWindow::updateLabels()
{
    totalPressedTimesLabel->setText(QString::number(setDataBase.keyPressedTimes));
    if(setDataBase.mapVector.size() > 5) {
        for(int i = 0; i < 5; i++) {
            frequentlyPressedKeys[i]->setText(setDataBase.mapVector[i].first + ": " + QString::number(setDataBase.mapVector[i].second));
        }
    }
    else {
        for(int i = 0; i < setDataBase.mapVector.size(); i++) {
            frequentlyPressedKeys[i]->setText(setDataBase.mapVector[i].first + ": " + QString::number(setDataBase.mapVector[i].second));
        }
    }
}

void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == 2 || reason == 3) { //tray icon was double clicked or clicked
        this->showNormal(); //to show a normal size of the main window
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(trayIcon->isVisible()) { //if tray icon can be seen in the task bar
        event->ignore(); //then do not quit the program
        this->hide(); //instead of closing the program, just hide the main window
    }
}

void MainWindow::showNextPage()
{
    totalPressedTimesLabel->hide();
    nextPageBtn->hide();
    previousPageBtn->show();
    for(int i = 0; i < frequentlyPressedKeys.size(); i++) {
        frequentlyPressedKeys[i]->show();
    }

    statusBar()->showMessage("Frequently Pressed");

    setDataBase.updateDatabase();
}

void MainWindow::showPreviousPage()
{
    totalPressedTimesLabel->show();
    nextPageBtn->show();
    previousPageBtn->hide();
    for(int i = 0; i < frequentlyPressedKeys.size(); i++) {
        frequentlyPressedKeys[i]->hide();
    }

    statusBar()->showMessage("Total Pressed");

    setDataBase.updateDatabase();
}

void MainWindow::startOnBootActionChanged()
{
    if(startOnBootAction->isChecked()) {
        startOnBootSettings->setValue("Day-Counter", QCoreApplication::applicationFilePath().replace('/', '\\'));
    }
    else {
        startOnBootSettings->remove("Day-Counter");
    }
}
