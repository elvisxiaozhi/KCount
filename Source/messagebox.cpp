#include "messagebox.h"
#include <QCloseEvent>
#include <QDebug>
#include <QFlags>
#include <QFlag>

MessageBox::MessageBox(QWidget *parent) : QMessageBox(parent)
{
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    setIcon(QMessageBox::Information);
}

void MessageBox::showSuccessMsBox()
{
    setWindowTitle(tr("Action Succeeded"));
    exec();
}

void MessageBox::showQuestionMsBox(int msBoxType)
{
    setIcon(QMessageBox::Warning);
    setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    setDefaultButton(QMessageBox::No);
    int ret = exec();

    switch (ret) {
    case QMessageBox::Yes:
        switch (msBoxType) {
        case 1:
            emit resetSettingsConfirmed();
            break;
        case 2:
            emit clearDatabaseConfirmed();
            break;
        case 3:
            emit resetAllConfirmed();
            break;
        case 4:
            emit deleteAppConfirmed();
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}
