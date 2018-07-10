#include "messagebox.h"
#include <QCloseEvent>
#include <QDebug>
#include <QFlags>
#include <QFlag>

MessageBox::MessageBox(QWidget *parent) : QMessageBox(parent)
{
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    setIcon(QMessageBox::Information);
    setMinimumSize(400, 300);
}

void MessageBox::showSuccessMsBox(QString text)
{
    setWindowTitle(tr("Action Succeeded"));
    setIcon(QMessageBox::Information);
    setStandardButtons(QMessageBox::Ok);
    setText(text);
    setDetailedText("");
    exec();
}

void MessageBox::showQuestionMsBox()
{
    setIcon(QMessageBox::Warning);
    setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    setDefaultButton(QMessageBox::No);
    setText(tr("Are you sure you want to reset everything to default?"));
    setDetailedText(tr("This will reset all settings to default, including deleting user data."));
    int ret = exec();

    switch (ret) {
    case QMessageBox::Yes:
        emit resetConfirmed();
        break;
    default:
        break;
    }
}

void MessageBox::showDatabaseErrorMsBox()
{
    setWindowTitle(tr("Database Error"));
    setText(tr("Do you want the app to try fixing this issue?"));
    setIcon(QMessageBox::Warning);
    setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    setDefaultButton(QMessageBox::Yes);
    int ret = exec();

    switch (ret) {
    case QMessageBox::Yes:
        emit fixDatabaseError();
        break;
    default:
        break;
    }
}
