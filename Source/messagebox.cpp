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
    setDetailedText("");
    int ret = exec();

    switch (ret) {
    case QMessageBox::Yes:
        switch (msBoxType) {
        case 0:
            emit resetSettingsConfirmed();
            break;
        case 1:
            emit clearDatabaseConfirmed();
            break;
        case 2:
            emit resetAllConfirmed();
            break;
        case 3:
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
