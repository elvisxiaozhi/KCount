#ifndef ABOUT_H
#define ABOUT_H

#include <QWidget>
#include <QVBoxLayout>

class About : public QWidget
{
    Q_OBJECT
public:
    explicit About(QWidget *parent = nullptr);

private:
    QVBoxLayout *mainVLayout;

    void createMainLayout();

protected:
    void paintEvent(QPaintEvent *);
};

#endif // ABOUT_H
