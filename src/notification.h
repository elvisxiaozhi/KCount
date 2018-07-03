#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QMap>
#include <windows.h>

class Notification : public QDialog
{
public:
    explicit Notification(QWidget *parent = 0);
    static QMap<QString, QString> xmlMap;

    static void readXml();
    static void writeXml();
    static void createRegistry(QString);
    static void deleteRegKey(QString);
    static void deleteRegValue(QString);

private:
    QPushButton *limitBtn;
    QString contText;
    QLabel *contLbl;
    QString limitAppName;

    static void showErrorText(DWORD);
    static WCHAR *QStoWCHAR(QString);
    static bool isDefaultKey(QString);

public slots:
    void setLabelText(QString);

};

#endif // NOTIFICATION_H
