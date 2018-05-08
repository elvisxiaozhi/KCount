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

private:
    QVBoxLayout *mainVLayout;
    QLabel *infoLabel, *moreInfoLabel;

signals:

public slots:
};

#endif // ABOUT_H
