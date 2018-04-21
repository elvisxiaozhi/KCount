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
    QCheckBox *soundAlertCheckBox, *autoSaveCheckBox;
    QLineEdit *reachingNumEdit;
    QSpinBox *autoSaveInterval;

private:
    void closeEvent(QCloseEvent *);
    QVBoxLayout *mainVLayout, *settingsContentVLayout, *generalVLayout;
    QGroupBox *mainGBox;
    QVector<QPushButton *> settingsBtns;
    QSettings *settings;
    bool isSoundAlertCheckBoxChecked, isAutoSaveCheckBoxChecked;
    QString reachingNum;
    int autoSaveIntervalNum;
    MessageBoxes setMsBox;
    void setBasicLayout();
    void setGeneralPage();
    void setSoundAlertLayout();
    void setAutoSaveLayout();
    void setResetLayout();
    void resetSettings();
    void resetAll();

signals:
    void refreshLabels();

public slots:
    void resetChanges();

private slots:
    void setFlatBtn();
    void saveChanges();
    void showResetSettingsMsBox();
    void showClearDatabaseMsbox();
    void showResetAllMsBox();
};

#endif // SETTINGS_H
