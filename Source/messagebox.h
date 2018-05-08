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

signals:
    void resetSettingsConfirmed();
    void clearDatabaseConfirmed();
    void resetAllConfirmed();
    void deleteAppConfirmed();

public slots:
};

#endif // MESSAGEBOX_H
