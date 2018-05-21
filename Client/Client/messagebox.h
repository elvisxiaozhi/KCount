#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QObject>
#include <QMessageBox>

class MessageBox : public QMessageBox
{
    Q_OBJECT
public:
    explicit MessageBox(QWidget *parent = nullptr);
    void showSuccessMsBox();
    void showQuestionMsBox(int);
    void showDatabaseErrorMsBox();

signals:
    void resetSettingsConfirmed();
    void clearDatabaseConfirmed();
    void resetAllConfirmed();
    void deleteAppConfirmed();
    void fixDatabaseError();

public slots:
};

#endif // MESSAGEBOX_H
