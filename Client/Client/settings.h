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
    QTabWidget *tabWidget;

    void setWindowStyleSheet();
    void setWindowLayout();
    void createLimitsLayout();

protected:
    void paintEvent(QPaintEvent *);

signals:

public slots:
};

#endif // SETTINGS_H
