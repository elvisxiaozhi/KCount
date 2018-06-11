#ifndef CUSTOMLABEL_H
#define CUSTOMLABEL_H

#include <QLabel>

class CustomLabel : public QLabel
{
    Q_OBJECT
public:
    explicit CustomLabel(QWidget *parent = nullptr);

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
};

#endif // CUSTOMLABEL_H
