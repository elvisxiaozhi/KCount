#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->label->setPixmap(QPixmap(":/icons/letter-n.png"));
}

Widget::~Widget()
{
    delete ui;
}
