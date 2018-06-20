#include "notification.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <windows.h>

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

    openRegistry();
}

void Notification::setLabelText(QString appName)
{
    qDebug() << appName;
    contText = QString(tr("  %1 has been totally using over 3 hours, take a break. :)")).arg(appName);
    contLbl->setText(contText);
}

void Notification::openRegistry()
{
    HKEY hKey;
    LPCTSTR lpSubKey = TEXT("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options");

    LONG openRes = RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpSubKey, 0, KEY_ALL_ACCESS , &hKey);

    if (openRes == ERROR_SUCCESS) {
        qDebug() << "Success opening key.";
    }
    else {
        qDebug() << "Error opening key.";
    }
}
