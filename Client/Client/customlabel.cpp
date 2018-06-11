#include "customlabel.h"

CustomLabel::CustomLabel(QWidget *parent) : QLabel(parent)
{
    setStyleSheet("QLabel { background: yellow; }");
    setFixedSize(30, 30);
}

void CustomLabel::enterEvent(QEvent *)
{
    setStyleSheet("QLabel { background: red; }");
}

void CustomLabel::leaveEvent(QEvent *)
{
    setStyleSheet("QLabel { background: yellow; }");
}
