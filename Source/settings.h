#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QObject>
#include <QSettings>
#include <QCheckBox>
#include <QLineEdit>
#include <QSpinBox>
#include "messageboxes.h"

class Settings : public QWidget
{
    Q_OBJECT
public:
    explicit Settings(QWidget *parent = nullptr);
    QCheckBox *soundAlertCheckBox;
    QLineEdit *reachingNumEdit;
    static QSettings startOnBootSetting;

private:
    void closeEvent(QCloseEvent *);
    QVBoxLayout *mainVLayout, *settingsContentVLayout, *generalVLayout;
    QGroupBox *mainGBox;
    QSettings *settings;
    bool isSoundAlertCheckBoxChecked;
    QString reachingNum;
    MessageBoxes setMsBox;
    void setBasicLayout();
    void setWindowStyleSheet();
    void setGeneralPage();
    void setSoundAlertLayout();
    void setResetLayout();
    void resetSettings();
    void resetAll();
    QString writeBatFile();

signals:
    void databaseCleared();
    void uncheckStartOnBootAct();

public slots:
    void resetChanges();

private slots:
    void saveChanges();
    void showResetSettingsMsBox();
    void showClearDatabaseMsbox();
    void showResetAllMsBox();
    void showDeleteAppMsBox();
    void deleteApp();
};

#endif // SETTINGS_H
