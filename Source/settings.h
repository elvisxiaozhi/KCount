#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

class Settings : public QWidget
{
    Q_OBJECT
public:
    explicit Settings(QWidget *parent = nullptr);

private:
    void closeEvent(QCloseEvent *);
    QVBoxLayout *mainVLayout;
    QHBoxLayout *btnHLayout;
    QVector<QPushButton *> settingsBtns;
    void setBasicLayout();

signals:

public slots:

private slots:
    void setFlatBtn();
};

#endif // SETTINGS_H
