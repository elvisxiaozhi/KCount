#include "about.h"
#include <QHBoxLayout>
#include <QPaintEvent>
#include <QPainter>
#include <QStyleOption>
#include <QShortcut>
#include <QDebug>

About::About(QWidget *parent) : QWidget(parent)
{
    this->hide();
    setFixedHeight(120);
    createMainLayout();
    setStyleSheet(
                "QWidget { background-color: white; }"
                );
    QShortcut *shortCut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_N), this);

    connect(shortCut, &QShortcut::activated, this, &About::showEasterEgg);
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

    easterEggLbl = new QLabel(this);
    easterEggLbl->hide();
    easterEggLbl->setText(" Dedicated to the girl I loved. <br> Great memories from 6/6/15 to 12/24/17.");
    easterEggLbl->setAlignment(Qt::AlignCenter);
    easterEggLbl->setStyleSheet("QLabel { color: #FF4136; font-size: 20px; }");

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

    mainVLayout->addLayout(versionHLayout);
    mainVLayout->addLayout(authorHLayout);
    mainVLayout->addLayout(webHLayout);
    mainVLayout->addWidget(easterEggLbl);
    mainVLayout->addWidget(copyrightLbl);
    mainVLayout->addStretch();
    setLayout(mainVLayout);
}

void About::showEasterEgg()
{
    easterEggLbl->show();
    setFixedHeight(150);
}

void About::paintEvent(QPaintEvent *)
{
    //to make the custom widget able to set style sheet
    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}
