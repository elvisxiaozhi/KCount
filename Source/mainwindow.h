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
#include "database.h"
#include "about.h"

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
    QAction *startOnBootAction;
    QSettings *startOnBootSettings;
    DataBase setDataBase;
    About setAboutPage;
    void setLayout();
    void setTrayIcon();
    void setLblText();
    void closeEvent(QCloseEvent *);

private slots:
    void updateLabels();
    void trayIconActivated(QSystemTrayIcon::ActivationReason);
    void showNextPage();
    void showPreviousPage();
    void startOnBootActionChanged();
};

#endif // MAINWINDOW_H
