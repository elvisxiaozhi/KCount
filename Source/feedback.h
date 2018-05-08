#ifndef FEEDBACK_H
#define FEEDBACK_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QPushButton>

class Feedback : public QWidget
{
    Q_OBJECT
public:
    explicit Feedback(QWidget *parent = nullptr);

private:
    QVBoxLayout *mainVLayout;
    QTextEdit *feedbackEdit;
    QPushButton *sendBtn;
    void setWindowStyleSheet();

signals:

public slots:
};

#endif // FEEDBACK_H
