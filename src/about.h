#ifndef ABOUT_H
#define ABOUT_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>

class About : public QWidget
{
    Q_OBJECT
public:
    explicit About(QWidget *parent = nullptr);
    QLabel *easterEggLbl;

private:
    QVBoxLayout *mainVLayout;

    void createMainLayout();

private slots:
    void showEasterEgg();

protected:
    void paintEvent(QPaintEvent *);
};

#endif // ABOUT_H
