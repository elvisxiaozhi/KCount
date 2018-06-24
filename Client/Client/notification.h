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
    QString limitAppName, xmlPath;

    void showErrorText(DWORD);
    void deleteRegValue(HKEY);
    void deleteRegKey();
    void writeXml();
    void readXml();

public slots:
    void setLabelText(QString);

private slots:
    void openRegistry();
    void createRegistry(HKEY, LPCTSTR);
    bool isDefaultKey(HKEY);
};

#endif // NOTIFICATION_H
