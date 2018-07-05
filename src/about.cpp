#include "about.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QPaintEvent>
#include <QPainter>
#include <QStyleOption>

About::About(QWidget *parent) : QWidget(parent)
{
    this->hide();
    createMainLayout();
    setStyleSheet(
                "QWidget { background-color: white; }"
                );
}

void About::createMainLayout()
{
    QHBoxLayout *versionHLayout = new QHBoxLayout();

    QLabel *versionLbl = new QLabel(this);
    versionLbl->setText(tr("<b>Version: </b>"));

    QLabel *versionDescLbl = new QLabel(this);
    versionDescLbl->setText(tr("Nana v2.0 beta"));

    versionHLayout->addWidget(versionLbl);
    versionHLayout->addWidget(versionDescLbl);
}

void About::paintEvent(QPaintEvent *)
{
    //to make the custom widget able to set style sheet
    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}
