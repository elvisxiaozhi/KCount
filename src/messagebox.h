#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QObject>
#include <QMessageBox>

class MessageBox : public QMessageBox
{
    Q_OBJECT
public:
    explicit MessageBox(QWidget *parent = nullptr);
    void showSuccessMsBox(QString);
    void showQuestionMsBox();
    void showDatabaseErrorMsBox();

signals:
    void resetConfirmed();
    void fixDatabaseError();
};

#endif // MESSAGEBOX_H
