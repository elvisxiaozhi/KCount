#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSystemTrayIcon>
#include <QAction>
#include <QToolButton>
#include <QLabel>
#include <QSettings>
#include "label.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QWidget *mainWidget;
    QVBoxLayout *mainVLayout, *lblsVLayout;
    QHBoxLayout *btnHLayout;
    QSystemTrayIcon *trayIcon;
    Label *totalPressedTimesLabel;
    QVector<Label *> frequentlyPressedKeys;
    QToolButton *nextPageBtn, *previousPageBtn;
    unsigned long long int keyPressedTimes;
    QAction *startOnBootAction;
    QSettings *startOnBootSettings;
    QMap<QString, unsigned long long int> pressedKeyMap;
    QVector<std::pair<QString, unsigned long long int>> mapVector; //store map, in order to sort map value
    void setLayout();
    void setTrayIcon();

private slots:
    void keyPressed(QString);
    void trayIconActivated(QSystemTrayIcon::ActivationReason);
    void closeEvent(QCloseEvent *);

private slots:
    void showNextPage();
    void showPreviousPage();
    void startOnBootActionChanged();
};

#endif // MAINWINDOW_H
