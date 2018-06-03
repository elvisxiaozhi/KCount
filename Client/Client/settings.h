#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QSettings>

class Settings : public QWidget
{
    Q_OBJECT
public:
    explicit Settings(QWidget *parent = nullptr);
    static QSettings startOnBootSettings;

private:
    void setWindowStyleSheet();

protected:
    void paintEvent(QPaintEvent *);

signals:

public slots:
};

#endif // SETTINGS_H
