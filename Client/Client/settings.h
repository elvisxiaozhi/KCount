#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QSettings>
#include <QScrollArea>
#include <QVBoxLayout>

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

    void setWindowStyleSheet();
    void setWindowLayout();

protected:
    void paintEvent(QPaintEvent *);

signals:

public slots:
};

#endif // SETTINGS_H
