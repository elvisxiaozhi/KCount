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

private:
    QPushButton *limitBtn;
    QString contText;
    QLabel *contLbl;
    QString limitAppName;

    void showErrorText(DWORD);
    void deleteRegValue(HKEY);
    void deleteRegKey();
    void writeXml(QString, bool);
    inline const WCHAR *QStoWCHAR(const QString &);

public slots:
    void setLabelText(QString);

private slots:
    void createRegistry();
    bool isDefaultKey(HKEY, LPCTSTR);
};

#endif // NOTIFICATION_H
