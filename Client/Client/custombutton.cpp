#include "custombutton.h"

CustomButton::CustomButton(QWidget *parent) : QPushButton(parent)
{
    setFixedSize(30, 30);
    setStyleSheet("QPushButton { background-color: #3498DB; border: 0px; }"
                  "QPushButton:hover { background-color: #BB8FCE; }"
                  "QPushButton:pressed { background-color: #EC7063 }");
}
