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
#include "messagebox.h"
#include <QComboBox>

class Settings : public QWidget
{
    Q_OBJECT
public:
    explicit Settings(QWidget *parent = nullptr);
    QCheckBox *soundAlertCheckBox; //MainWindow class need this to check if to make sound alert
    QLineEdit *reachingNumEdit; //MainWindow class need this to check the specific num
    static QSettings startOnBootSetting;
    static QSettings languageSettings;

private:
    QVBoxLayout *mainVLayout, *settingsContentVLayout, *generalVLayout;
    QGroupBox *mainGBox;
    QSettings *settings;
    QComboBox *languageBox;
    MessageBox msBox;
    void setBasicLayout();
    void setWindowStyleSheet();
    void setGeneralPage();
    void setSoundAlertLayout();
    void setLanguageLayout();
    void setResetLayout();
    void resetSettings();
    void resetAll();
    QString writeBatFile();

signals:
    void databaseCleared();
    void checkStartOnBootAct();

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
