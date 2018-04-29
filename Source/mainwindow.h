#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSystemTrayIcon>
#include <QAction>
#include <QToolButton>
#include <QLabel>
#include "label.h"
#include "database.h"
#include "about.h"
#include "settings.h"
#include "statistics.h"

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
    Database setDatabase;
    Settings setSettingsPage;
    About setAboutPage;
    Statistics setStatistics;
    void setLayout();
    void setTrayIcon();
    void setLblText();
    void setLblColor();
    void closeEvent(QCloseEvent *);

signals:

private slots:
    void updateLabels();
    void trayIconActivated(QSystemTrayIcon::ActivationReason);
    void showNextPage();
    void showPreviousPage();
    void startOnBootActionChanged();
    void changeViewMode(int);
};

#endif // MAINWINDOW_H
