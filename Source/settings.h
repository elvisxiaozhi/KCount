#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QObject>

class Settings : public QWidget
{
    Q_OBJECT
public:
    explicit Settings(QWidget *parent = nullptr);

private:
    void closeEvent(QCloseEvent *);
    QVBoxLayout *mainVLayout, *settingsContentVLayout, *generalVLayout;
    QGroupBox *mainGBox;
    QVector<QPushButton *> settingsBtns;
    void setBasicLayout();
    void setGeneralPage();

signals:

public slots:

private slots:
    void setFlatBtn();
};

#endif // SETTINGS_H
