#include "about.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QPaintEvent>
#include <QPainter>
#include <QStyleOption>

About::About(QWidget *parent) : QWidget(parent)
{
    this->hide();
    setFixedHeight(120);
    createMainLayout();
    setStyleSheet(
                "QWidget { background-color: white; }"
                );
}

void About::createMainLayout()
{
    mainVLayout = new QVBoxLayout(this);
    mainVLayout->setSpacing(0);

    QHBoxLayout *versionHLayout = new QHBoxLayout();

    QLabel *versionLbl = new QLabel(this);
    versionLbl->setText(tr("<b>Version:</b>"));

    QLabel *versionDescLbl = new QLabel(this);
    versionDescLbl->setText(tr("     Nana v2.0 beta"));

    QHBoxLayout *authorHLayout = new QHBoxLayout();

    QLabel *authorLbl = new QLabel(this);
    authorLbl->setText(tr("Author:      "));
    authorLbl->setStyleSheet("QLabel { font: bold; }");

    QLabel *authorNameLbl = new QLabel(this);
    authorNameLbl->setText(tr("<a href='https://github.com/elvisxiaozhi'>Theodore Tang</a>"));
    authorNameLbl->setOpenExternalLinks(true);
    authorNameLbl->setTextInteractionFlags(Qt::TextBrowserInteraction);

    QHBoxLayout *webHLayout = new QHBoxLayout();

    QLabel *webLbl = new QLabel(this);
    webLbl->setText(tr("Website:    "));
    webLbl->setStyleSheet("QLabel { font: bold; }");

    QLabel *linkLbl = new QLabel(this);
    linkLbl->setText(tr("<a href='https://github.com/elvisxiaozhi/Nana/releases'>https://github.com/elvisxiaozhi/Nana/releases</a>"));

    QHBoxLayout *descHLayout = new QHBoxLayout();

    QLabel *descLbl = new QLabel(this);
    descLbl->setText("<b>Description: </b>");

    QLabel *descTextLbl = new QLabel(this);
    descTextLbl->setText(" Dedicated to the one I loved.");

    QLabel *copyrightLbl = new QLabel(this);
    copyrightLbl->setText("<br>Copyright © 4/14/2018 - . All rights reserved.");

    versionHLayout->addWidget(versionLbl);
    versionHLayout->addWidget(versionDescLbl);
    versionHLayout->addStretch();

    authorHLayout->addWidget(authorLbl);
    authorHLayout->addWidget(authorNameLbl);
    authorHLayout->addStretch();

    webHLayout->addWidget(webLbl);
    webHLayout->addWidget(linkLbl);
    webHLayout->addStretch();

    descHLayout->addWidget(descLbl);
    descHLayout->addWidget(descTextLbl);
    descHLayout->addStretch();

    mainVLayout->addLayout(versionHLayout);
    mainVLayout->addLayout(authorHLayout);
    mainVLayout->addLayout(webHLayout);
    mainVLayout->addLayout(descHLayout);
    mainVLayout->addWidget(copyrightLbl);
    mainVLayout->addStretch();
    setLayout(mainVLayout);
}

void About::paintEvent(QPaintEvent *)
{
    //to make the custom widget able to set style sheet
    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}
