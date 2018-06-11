#include "custombutton.h"

CustomButton::CustomButton(QWidget *parent) : QPushButton(parent)
{
    setFixedSize(30, 30);
    setStyleSheet("QPushButton { background-color: rgba(233, 75, 60, 0.1); border: 0px; }"
                  "QPushButton:hover { background-color: #EC9787; }"
                  "QPushButton:pressed { background-color: #EC7063 }");
}
