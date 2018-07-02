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
    static bool isDefaultKey(QString);

private:
    QPushButton *limitBtn;
    QString contText;
    QLabel *contLbl;
    QString limitAppName;

    void showErrorText(DWORD);
    void deleteRegValue(HKEY);
    void deleteRegKey();
    void writeXml(QString, bool);
    static WCHAR *QStoWCHAR(QString);

public slots:
    void setLabelText(QString);

private slots:
    void createRegistry();
};

#endif // NOTIFICATION_H
