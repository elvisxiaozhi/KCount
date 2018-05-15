#ifndef MOSTPRESSED_H
#define MOSTPRESSED_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class MostPressed : public QWidget
{
    Q_OBJECT
public:
    explicit MostPressed(QWidget *parent = nullptr);

private:
    QVBoxLayout *mainVLayout, *contVLayout;
    QLabel *title;
    QVector<QLabel *> contents;
    QVector<std::pair<QString, unsigned long int>> mostPressed;

    void setWindowStyleSheet();
    void setContents();

protected:
    void paintEvent(QPaintEvent *);

signals:

public slots:

private slots:
    void keyPressed(QString);
};

#endif // MOSTPRESSED_H
