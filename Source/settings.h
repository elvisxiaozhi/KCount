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

class Settings : public QWidget
{
    Q_OBJECT
public:
    explicit Settings(QWidget *parent = nullptr);
    QCheckBox *soundAlertCheckBox;
    QLineEdit *reachingNumEdit;

private:
    void closeEvent(QCloseEvent *);
    QVBoxLayout *mainVLayout, *settingsContentVLayout, *generalVLayout;
    QGroupBox *mainGBox;
    QVector<QPushButton *> settingsBtns;
    QSettings *settings;
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
