#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QSettings>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QToolButton>
#include <QTabWidget>

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
    QVBoxLayout *limitsHLayout;
    QToolButton *limitsBtn;
    QWidget *limitsWidget;
    QVBoxLayout *limitsVLayout;
    QTabWidget *tabWidget;
    QWidget *limitedTab, *allowedTab;

    void setWindowStyleSheet();
    void setWindowLayout();
    void createLimitsLayout();
    void createLimitsWidget();

protected:
    void paintEvent(QPaintEvent *);
};

#endif // SETTINGS_H
