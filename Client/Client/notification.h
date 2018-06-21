#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <windows.h>

class Notification : public QDialog
{
public:
    explicit Notification(QWidget *parent = 0);

private:
    QPushButton *limitBtn;
    QString contText;
    QLabel *contLbl;

    void showErrorText(DWORD);

public slots:
    void setLabelText(QString);

private slots:
    void openRegistry();
};

#endif // NOTIFICATION_H
