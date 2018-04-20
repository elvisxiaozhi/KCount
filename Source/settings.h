#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QObject>
#include <QSettings>

class Settings : public QWidget
{
    Q_OBJECT
public:
    explicit Settings(QWidget *parent = nullptr);
    QSettings *settings;

private:
    void closeEvent(QCloseEvent *);
    QVBoxLayout *mainVLayout, *settingsContentVLayout, *generalVLayout;
    QGroupBox *mainGBox;
    QVector<QPushButton *> settingsBtns;
    void setBasicLayout();
    void setGeneralPage();
    void setSoundAlertLayout();
    void setAutoSaveLayout();

signals:

public slots:

private slots:
    void setFlatBtn();
};

#endif // SETTINGS_H
