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
    setDatabaseThread();

    connect(&setSettingsPage, &Settings::uncheckStartOnBootAct, [this](){ startOnBootAction->setChecked(false); }); //set startOnBootAction unchecked when reset the settings
    connect(&setSettingsPage, &Settings::databaseCleared, [this](){ //when database is cleared, clear map and vector and total pressed times, so the lbls can set to 0
        database->readDatabase(2);
        database->currentHourPressedKeyMap.clear();
        setLblTextAndColor(); //this line is used for refreshing lbls when the main window is activated
    });
}

MainWindow::~MainWindow()
{
    databaseThread.quit();
    databaseThread.wait();
}

void MainWindow::setLayout()
{
    this->setWindowTitle("KCount");

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

        //the two signals below are to change two objects checked action when one of them's action is changed
        connect(totalPressedTimesLabel, &Label::viewModeChanged, frequentlyPressedKeys[i], &Label::setActionChecked);
        connect(frequentlyPressedKeys[i], &Label::viewModeChanged, totalPressedTimesLabel, &Label::setActionChecked);
        connect(frequentlyPressedKeys[i], &Label::viewModeChanged, this, &MainWindow::changeViewMode);
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

    connect(totalPressedTimesLabel, &Label::viewModeChanged, this, &MainWindow::changeViewMode);
    connect(nextPageBtn, &QToolButton::clicked, this, &MainWindow::showNextPage);
    connect(previousPageBtn, &QToolButton::clicked, this, &MainWindow::showPreviousPage);
}

void MainWindow::setTrayIcon()
{
    trayIcon = new QSystemTrayIcon(QIcon(":/Icons/Icons/keyboard_tray_icon.png"), this);
    trayIcon->show();
    trayIcon->setToolTip("KCount 1.2 Beta");

    QMenu *trayIconMenu = new QMenu;
    trayIcon->setContextMenu(trayIconMenu);

    startOnBootAction = new QAction("Start on Boot", trayIcon);
    trayIconMenu->addAction(startOnBootAction);
    startOnBootAction->setCheckable(true);

    if(Settings::startOnBootSetting.value("KCount").isValid()) {
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
    connect(statisticsAction, &QAction::triggered, [this](){ statistics.show();  statistics.loadBarChartData(0); });
    connect(updateAction, &QAction::triggered, [this](){ QDesktopServices::openUrl(QUrl("https://github.com/elvisxiaozhi/Keyboard-Tracker/releases")); });
    connect(aboutAction, &QAction::triggered, [this](){ setAboutPage.show(); });
    //note the program can be only closed by clicking "Quit" action when the trayIcon is not visible
    connect(quitAction, &QAction::triggered, [this](){ database->updateDatabase(); trayIcon->setVisible(false); this->close(); }); //update and save data to database before closing the program, and set the trayIcon not visible so the program can be closed successfully
}

void MainWindow::setLblTextAndColor()
{
    totalPressedTimesLabel->setText(QString::number(database->totalPressedTimes)); //set total pressed times lbl
    totalPressedTimesLabel->setLblColor(database->totalPressedTimes); //set color for total pressed times lbl

    //set most frequently pressed times lbls
    //if mapVector is empty, which means there is no most frequently pressed key
    if(database->mapVector.isEmpty()) {
        for(int i = 0; i < 5; i++) {
            frequentlyPressedKeys[i]->setText(""); //then set the top five most frequently lbls text to blank
            frequentlyPressedKeys[i]->setLblColor(0); //then set the frequently pressed keys colors to the default color
        }
    }
    //if mapVector is more than 5 elements
    if(database->mapVector.size() > 5) {
        for(int i = 0; i < 5; i++) { //then set the top five most frequently pressed times keys to lbls
            frequentlyPressedKeys[i]->setText(database->mapVector[i].first + ": " + QString::number(database->mapVector[i].second));
            frequentlyPressedKeys[i]->setLblColor(database->mapVector[i].second);
        }
    }
    //if mapVector is less than or equal 5 elements
    else {
        //first set the top frequently pressed keys that the mapVector has
        for(int i = 0; i < database->mapVector.size(); i++) {
            frequentlyPressedKeys[i]->setText(database->mapVector[i].first + ": " + QString::number(database->mapVector[i].second));
            frequentlyPressedKeys[i]->setLblColor(database->mapVector[i].second); //set the top frequently pressed keys lbl with the colors that they should have
        }
        //then set the rest of the lbls texts to blank
        for(int i = 0; i < 5 - database->mapVector.size(); i++) {
            frequentlyPressedKeys[4 - i]->setText(""); //note the frequentlyPressedKeys[4 - i], the max index of frequentlyPressedKeys is 4
            frequentlyPressedKeys[4 - i]->setLblColor(0); //for the rest of the empty lbls, set the colors to default
        }
    }
}

void MainWindow::updateLabels()
{
    if(this->isVisible()) { //update texts and text colors to lbls only when the main window is visible, so it increase the preformance
        setLblTextAndColor();
    }

    //keyPressedDone will emit each time when a key pressed, and then will check if a sound alert should be made
    if(setSettingsPage.soundAlertCheckBox->isChecked()) { //if soundAlertCheckBox is checked
        if(database->totalPressedTimes % setSettingsPage.reachingNumEdit->text().toInt() == 0) { //then make a sound when the totalPressedTimes reaches some sepcific nums
            QSound::play(":/Sounds/Sounds/ding.wav");
        }
    }
}

void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == 2 || reason == 3) { //tray icon was double clicked or clicked
        this->showNormal(); //to show a normal size of the main window
        setLblTextAndColor();
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(trayIcon->isVisible()) { //if tray icon can be seen in the task bar
        event->ignore(); //then do not quit the program
        this->hide(); //instead of closing the program, just hide the main window
    }
}

void MainWindow::setDatabaseThread()
{
    database = new Database;
    database->moveToThread(&databaseThread);
    connect(&databaseThread, &QThread::finished, database, &QObject::deleteLater);
    connect(database, &Database::keyPressedDone, this, &MainWindow::updateLabels);
    connect(&statistics, &Statistics::loadBarChartData, database, &Database::loadBarChartData);
    connect(database, &Database::barChartDataLoaded, &statistics, &Statistics::updateBarChart, Qt::QueuedConnection);
    databaseThread.start();
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
        Settings::startOnBootSetting.setValue("KCount", QCoreApplication::applicationFilePath().replace('/', '\\'));
    }
    else {
        Settings::startOnBootSetting.remove("KCount");
    }
}

void MainWindow::changeViewMode(int viewMode) //when changing the view mode, re-read the database base on the view mode and then update lbls
{
    database->updateDatabase();
    database->readDatabase(viewMode);
    updateLabels(); //why call updateLabels() instead of setLblTextAndColor(), because this function has sound alert detection, when changing mode, the new nums may reach the sepcific nums
}
