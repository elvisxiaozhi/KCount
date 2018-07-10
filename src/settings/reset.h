#ifndef RESET_H
#define RESET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>

class Reset : public QWidget
{
    Q_OBJECT
public:
    explicit Reset(QWidget *parent = nullptr);

private:
    QVBoxLayout *mainVLayout;
    QLineEdit *appPathEdit;
    QPushButton *resetBtn;

    void createMainLayout();

protected:
    void paintEvent(QPaintEvent *);

private slots:
    void resetAll();
};

#endif // RESET_H
