#include "mainwindow.h"
#include <QMenu>
#include <QKeyEvent>
#include <windows.h>
#include <QDebug>
#include "signalemitter.h"
#include <QCloseEvent>
//#pragma comment(lib, "user32.lib")

HHOOK hHook = NULL;

void UpdateKeyState(BYTE *keystate, int keycode)
{
    keystate[keycode] = GetKeyState(keycode);
}

LRESULT CALLBACK MyLowLevelKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    //WPARAM is WM_KEYDOWn, WM_KEYUP, WM_SYSKEYDOWN, or WM_SYSKEYUP
    //LPARAM is the key information

    if (wParam == WM_KEYDOWN) {
        qDebug() << "Key Pressed!";
        Emitter::Instance()->keyPressed();

        //Get the key information
        KBDLLHOOKSTRUCT cKey = *((KBDLLHOOKSTRUCT*)lParam);

        wchar_t buffer[5];

        //get the keyboard state
        BYTE keyboard_state[256];
        GetKeyboardState(keyboard_state);
        UpdateKeyState(keyboard_state, VK_SHIFT);
        UpdateKeyState(keyboard_state, VK_CAPITAL);
        UpdateKeyState(keyboard_state, VK_CONTROL);
        UpdateKeyState(keyboard_state, VK_MENU);

        //Get keyboard layout
        HKL keyboard_layout = GetKeyboardLayout(0);

        //Get the name
        char lpszName[0X100] = {0};

        DWORD dwMsg = 1;
        dwMsg += cKey.scanCode << 16;
        dwMsg += cKey.flags << 24;

        GetKeyNameText(dwMsg, (LPTSTR)lpszName, 255);

        //Try to convert the key information
        ToUnicodeEx(cKey.vkCode, cKey.scanCode, keyboard_state, buffer, 4, 0, keyboard_layout);
        buffer[4] = L'\0';

        //Print the output
        qDebug() << "Key: " << cKey.vkCode << " " << QString::fromUtf16((ushort*)buffer) << " " << QString::fromUtf16((ushort*)lpszName);
    }

    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, MyLowLevelKeyBoardProc, NULL, 0);
    if (hHook == NULL) {
        qDebug() << "Hook Failed" << endl;
    }

    setLayout();
    setTrayIcon();
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

    keyPressedTimesLabel = new QLabel(mainWidget);
    mainVLayout->addWidget(keyPressedTimesLabel);
    keyPressedTimesLabel->setAlignment(Qt::AlignCenter);
    keyPressedTimesLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    keyPressedTimesLabel->setStyleSheet("QLabel { background-color: #FAD7A0; color: #8E44AD; font-size: 50px; }");
    keyPressedTimes = 0;
    keyPressedTimesLabel->setText(QString::number(keyPressedTimes));
    connect(Emitter::Instance(), &SignalEmitter::keyPressed, this, &MainWindow::keyPressed);
}

void MainWindow::setTrayIcon()
{
    trayIcon = new QSystemTrayIcon(QIcon(":/keyboard_tray_icon.png"), this);
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

    connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::trayIconActivated);
    connect(quitAction, &QAction::triggered, [this](){ trayIcon->setVisible(false); this->close(); }); //note the program can be only closed by clicking "Quit" action
}

void MainWindow::keyPressed()
{
    keyPressedTimes++;
    keyPressedTimesLabel->setText(QString::number(keyPressedTimes));
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
