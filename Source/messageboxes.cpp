#include "messageboxes.h"

MessageBoxes::MessageBoxes(QObject *parent) : QObject(parent)
{
}

void MessageBoxes::showQuestionMsBox(int msBoxType)
{
    questionMsBox.setIcon(QMessageBox::Warning);
    questionMsBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    questionMsBox.setDefaultButton(QMessageBox::No);
    questionMsBox.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    questionMsBox.setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint); //hide message box icon
    questionMsBox.setMinimumSize(300, 200); //need this to fix a warning
    int ret = questionMsBox.exec();

    switch (ret) {
    case QMessageBox::Yes:
        switch (msBoxType) {
        case 1:
            emit resetSettingsConfirmed();
            break;
        default:
            break;
        }
        questionMsBox.close();
        break;
    case QMessageBox::No:
        questionMsBox.close();
        break;
    default:
        break;
    }
}

void MessageBoxes::showSuccessMsBox()
{
    successMsBox.setWindowTitle("Action Succeeded");
    successMsBox.setIcon(QMessageBox::Information);
    successMsBox.setStandardButtons(QMessageBox::Ok);
    successMsBox.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    successMsBox.setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint); //hide message box icon
    successMsBox.setMinimumSize(300, 200); //need this to fix a warning
    int ret = successMsBox.exec();

    switch (ret) {
    case QMessageBox::Ok:
        successMsBox.close();
        break;
    default:
        break;
    }
}
