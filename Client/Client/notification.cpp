#include "notification.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <Tchar.h>
#include <QDebug>

Notification::Notification(QWidget *parent) : QDialog(parent)
{
    setFixedSize(300, 200);
    move(qApp->desktop()->geometry().bottomRight() - QPoint(300, 290));
    setWindowTitle("App Limit");
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint); //only show close button and hide question mark btn in title bar

    QVBoxLayout *vLayout = new QVBoxLayout(this);

    contLbl = new QLabel(this);
    contLbl->setStyleSheet("QLabel { font: 25px; }");
    contLbl->setWordWrap(true);

    limitBtn = new QPushButton(this);
    limitBtn->setText("Limit This App");

    QHBoxLayout *btnHLayout = new QHBoxLayout();
    btnHLayout->addStretch();
    btnHLayout->addWidget(limitBtn);

    vLayout->addWidget(contLbl);
    vLayout->addLayout(btnHLayout);

    connect(limitBtn, &QPushButton::clicked, this, &Notification::createRegistry);
}

void Notification::showErrorText(DWORD errorNum)
{
    char *messageBuffer = NULL;
    FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, errorNum, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

    qDebug() <<  errorNum << messageBuffer;
}

void Notification::deleteRegValue(HKEY hKey)
{
    PCTSTR deleteValue = TEXT("debugger");
    LONG deletValueRes = RegDeleteValue(hKey, deleteValue);

    if(deletValueRes == ERROR_SUCCESS) {
        qDebug() << "delete success!";
    }
    else {
        qDebug() << "delete failed!";
        showErrorText(deletValueRes);
    }
}

void Notification::deleteRegKey()
{
    HKEY hKey;
    LPCTSTR sk = TEXT("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options");

    LONG openRes = RegOpenKeyEx(HKEY_LOCAL_MACHINE, sk, 0, KEY_ALL_ACCESS , &hKey);

    if(openRes == ERROR_SUCCESS) {
        qDebug() << "Success opening key.";
    }
    else {
        qDebug() << "Error opening key.";
        showErrorText(openRes);
    }

    PCTSTR deleteKey = TEXT("notepad.exe");
    LONG deleteKeyRes = RegDeleteKey(hKey, deleteKey);

    if(deleteKeyRes == ERROR_SUCCESS) {
        qDebug() << "Delete key success";
    }
    else {
        qDebug() << "Delete key failed";
        showErrorText(deleteKeyRes);
    }

    LONG closeOut = RegCloseKey(hKey);

    if(closeOut == ERROR_SUCCESS) {
        qDebug() << "Success closing key.";
    }
    else {
        qDebug() << "Error closing key.";
    }
}

void Notification::setLabelText(QString appName)
{
    qDebug() << appName;
    contText = QString(tr("  %1 has been totally using over 3 hours, take a break. :)")).arg(appName);
    contLbl->setText(contText);
    limitAppName = appName;
}

void Notification::createRegistry()
{
    HKEY hKey;
    LPCTSTR sk = TEXT("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\notepad.exe");

    LONG openRes = RegOpenKeyEx(HKEY_LOCAL_MACHINE, sk, 0, KEY_ALL_ACCESS , &hKey);

    if (openRes == ERROR_SUCCESS) {
        qDebug() << "Success opening key.";

        if(RegQueryValueEx(hKey, TEXT("MitigationOptions"), NULL, NULL, NULL, NULL) == ERROR_SUCCESS) {
            qDebug() << "Default Key";
        }
        else {
            qDebug() << "User Created Key";
        }
    }
    else {
        qDebug() << "Error opening key.";

        showErrorText(openRes);
    }

    LONG createResKey = RegCreateKeyEx(HKEY_LOCAL_MACHINE, sk, 0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL);

    if (createResKey == ERROR_SUCCESS) {
        qDebug() << "Success creating key.";
    }
    else {
        qDebug() << "Error creating key.";

        showErrorText(createResKey);
    }

    LPCTSTR value = TEXT("debugger");
    LPCTSTR data = TEXT("debugfile.exe");
    LONG setRes = RegSetValueEx(hKey, value, 0, REG_SZ, (LPBYTE)data, _tcslen(data) * sizeof(TCHAR));

    if(setRes == ERROR_SUCCESS) {
        qDebug() << "Success writing to Registry.";
    }
    else {
        qDebug() << "Error writing to Registry.";
    }

    LONG closeOut = RegCloseKey(hKey);

    if(closeOut == ERROR_SUCCESS) {
        qDebug() << "Success closing key.";
    }
    else {
        qDebug() << "Error closing key.";
    }
}
