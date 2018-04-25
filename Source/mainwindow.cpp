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
#include <QDesktopServices>
#include <QMessageBox>
#include <QSound>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    Hook setHook;
    setLayout();
    setTrayIcon();

    //initiate the label color, so to let it show the right color when the program starts
    setLblColor();

    connect(&setSettingsPage, &Settings::uncheckStartOnBootAct, [this](){ startOnBootAction->setChecked(false); }); //set startOnBootAction unchecked when reset the settings
    connect(&setSettingsPage, &Settings::databaseCleared, [this](){ //when database is cleared, clear map and vector and total pressed times, so the lbls can set to 0
        setDataBase.pressedKeyMap.clear();
        setDataBase.mapVector.clear();
        setDataBase.keyPressedTimes = 0;
        setLblText(); //this line is used for refreshing lbls when the main window is activated
    });
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

    frequentlyPressedKeys.resize(5);
    for(int i = 0; i < frequentlyPressedKeys.size(); i++) {
        frequentlyPressedKeys[i] = new Label;
        lblsVLayout->addWidget(frequentlyPressedKeys[i]);
        frequentlyPressedKeys[i]->hide();

        connect(frequentlyPressedKeys[i], &Label::viewNodeChanged, this, &MainWindow::changeViewMode);
    }

    btnHLayout = new QHBoxLayout;
    mainVLayout->addLayout(btnHLayout);

    QSpacerItem *leftBtnSpacer = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
    btnHLayout->addSpacerItem(leftBtnSpacer);

    nextPageBtn = new QToolButton(mainWidget);
    btnHLayout->addWidget(nextPageBtn);
    nextPageBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    nextPageBtn->setIcon(QIcon(":/Icons/Icons/next.png"));
    nextPageBtn->setText("Next");
    nextPageBtn->setLayoutDirection(Qt::RightToLeft);
    nextPageBtn->setStyleSheet("QToolButton { background-color: #FF5A5F; font-size: 20px; }"); //Rausch color for button background

    previousPageBtn = new QToolButton(mainWidget);
    btnHLayout->addWidget(previousPageBtn);
    previousPageBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    previousPageBtn->setIcon(QIcon(":/Icons/Icons/back.png"));
    previousPageBtn->setText("Back");
    previousPageBtn->hide();
    previousPageBtn->setStyleSheet("QToolButton { background-color: #FF5A5F; font-size: 20px; }");

    QSpacerItem *rightBtnSpacer = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum);
    btnHLayout->addSpacerItem(rightBtnSpacer);

    statusBar()->showMessage("Total Pressed");

    connect(totalPressedTimesLabel, &Label::viewNodeChanged, this, &MainWindow::changeViewMode);
    connect(nextPageBtn, &QToolButton::clicked, this, &MainWindow::showNextPage);
    connect(previousPageBtn, &QToolButton::clicked, this, &MainWindow::showPreviousPage);
    connect(&setDataBase, &DataBase::keyPressedDone, this, &MainWindow::updateLabels);
}

void MainWindow::setTrayIcon()
{
    trayIcon = new QSystemTrayIcon(QIcon(":/Icons/Icons/keyboard_tray_icon.png"), this);
    trayIcon->show();
    trayIcon->setToolTip("Keylogger Alpha Verion");

    QMenu *trayIconMenu = new QMenu;
    trayIcon->setContextMenu(trayIconMenu);

    startOnBootAction = new QAction("Start on Boot", trayIcon);
    trayIconMenu->addAction(startOnBootAction);
    startOnBootAction->setCheckable(true);

    if(Settings::startOnBootSetting.value("Keylogger").isValid()) {
        startOnBootAction->setChecked(true);
    }

    QAction *settingsAction = new QAction("Settings", trayIconMenu);
    trayIconMenu->addAction(settingsAction);

    QAction *statisticsAction = new QAction("Statistics", trayIconMenu);
    trayIconMenu->addAction(statisticsAction);

    QMenu *helpMenu = new QMenu("Help");
    trayIconMenu->addMenu(helpMenu);
    QAction *feedbackAction = new QAction("Feedback", helpMenu);
    helpMenu->addAction(feedbackAction);
    QAction *updateAction = new QAction("Update", helpMenu);
    helpMenu->addAction(updateAction);
    QAction *aboutAction = new QAction("About", helpMenu);
    helpMenu->addAction(aboutAction);
    QAction *donateAction = new QAction("Donate", helpMenu);
    helpMenu->addAction(donateAction);

    QAction *quitAction = new QAction("Quit", trayIconMenu);
    trayIconMenu->addAction(quitAction);

    connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::trayIconActivated);
    connect(startOnBootAction, &QAction::changed, this, &MainWindow::startOnBootActionChanged);
    connect(settingsAction, &QAction::triggered, [this](){ setSettingsPage.resetChanges(); setSettingsPage.show(); });
    connect(statisticsAction, &QAction::triggered, [this](){ setStatistics.show(); });
    connect(updateAction, &QAction::triggered, [this](){ QDesktopServices::openUrl(QUrl("https://github.com/elvisxiaozhi/Keyboard-Tracker/releases")); });
    connect(aboutAction, &QAction::triggered, [this](){ setAboutPage.show(); });
    connect(quitAction, &QAction::triggered, [this](){ setDataBase.updateDatabase(); trayIcon->setVisible(false); this->close(); }); //note the program can be only closed by clicking "Quit" action
}

void MainWindow::setLblText()
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

void MainWindow::setLblColor()
{
    totalPressedTimesLabel->setLblColor(setDataBase.keyPressedTimes);
    if(setDataBase.mapVector.size() > 5) {
        for(int i = 0; i < 5; i++) {
            frequentlyPressedKeys[i]->setLblColor(setDataBase.mapVector[i].second);
        }
    }
    else {
        for(int i = 0; i < setDataBase.mapVector.size(); i++) {
            frequentlyPressedKeys[i]->setLblColor(setDataBase.mapVector[i].second);
        }
    }
}

void MainWindow::updateLabels()
{
    if(this->isHidden() == false) {
        setLblText();
    }

    //keyPressedDone will emit each time when a key pressed, and the will call this function
    if(setSettingsPage.soundAlertCheckBox->isChecked()) {
        if(setDataBase.keyPressedTimes % setSettingsPage.reachingNumEdit->text().toInt() == 0) {
            QSound::play(":/Sounds/Sounds/ding.wav");
        }
    }

    setLblColor();
}

void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == 2 || reason == 3) { //tray icon was double clicked or clicked
        this->showNormal(); //to show a normal size of the main window

        setLblText();
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
    QSound::play(":/Sounds/Sounds/click.wav"); //sound after everything
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
    QSound::play(":/Sounds/Sounds/click.wav");
}

void MainWindow::startOnBootActionChanged()
{
    if(startOnBootAction->isChecked()) {
        Settings::startOnBootSetting.setValue("Keylogger", QCoreApplication::applicationFilePath().replace('/', '\\'));
    }
    else {
        Settings::startOnBootSetting.remove("Keylogger");
    }
}

void MainWindow::changeViewMode(int viewMode) //when changing the view mode, re-read the database base on the view mode and then update lbls
{
    setDataBase.updateDatabase();
    setDataBase.readDatabase(viewMode);
    updateLabels();
    setDataBase.currentHourPressedKeyMap.clear(); //must update and read first, and then clear this map
}
