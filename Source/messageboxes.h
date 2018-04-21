#ifndef MESSAGEBOXES_H
#define MESSAGEBOXES_H

#include <QObject>
#include <QMessageBox>

class MessageBoxes : public QObject
{
    Q_OBJECT
public:
    explicit MessageBoxes(QObject *parent = nullptr);
    QMessageBox questionMsBox;
    QMessageBox successMsBox;

signals:
    void resetSettingsConfirmed();

public slots:
    void showQuestionMsBox(int);
    void showSuccessMsBox();
};

#endif // MESSAGEBOXES_H
