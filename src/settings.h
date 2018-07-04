#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QSettings>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QToolButton>
#include "applimits.h"

class Settings : public QWidget
{
    Q_OBJECT
public:
    explicit Settings(QWidget *parent = nullptr);
    static QSettings startOnBootSettings;

private:
    QVBoxLayout *mainVLayout, *scrollVLayout;
    QScrollArea *scrollArea;
    QWidget *scrollWidget;
    QToolButton *limitsBtn, *aboutBtn;
    QVector<QToolButton *> btnVec;
    AppLimits *appLimits;

    void setWindowStyleSheet();
    void setWindowLayout();
    QToolButton *createToolBtn(QString);

protected:
    void paintEvent(QPaintEvent *);

private slots:
    void limitsBtnClicked(QString, bool);
    void showToolBtn(QString, bool);
};

#endif // SETTINGS_H
