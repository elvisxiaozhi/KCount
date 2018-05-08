#include "about.h"
#include <QDebug>

About::About(QWidget *parent) : QWidget(parent)
{
    setWindowTitle(tr("About"));
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint); //hide message box icon
    setStyleSheet("QWidget { background-color: #FAE5D3; font-family: Comic Sans MS; }");

    mainVLayout = new QVBoxLayout(this);
    this->setLayout(mainVLayout);

    infoLabel = new QLabel(this);
    infoLabel->setText(tr("<b>A Theodore Tang Production</b><br><br>© 2018"));
    infoLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    infoLabel->setAlignment(Qt::AlignCenter);
    infoLabel->setStyleSheet("QLabel { font-size: 20px; }");

    moreInfoLabel = new QLabel(this);
    moreInfoLabel->setText(tr("For more info, visit the <a style='text-decoration:none;' href='https://github.com/elvisxiaozhi/KCount'>website</a>."));
    moreInfoLabel->setOpenExternalLinks(true); //set to open link on QLabel
    moreInfoLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    moreInfoLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    moreInfoLabel->setAlignment(Qt::AlignCenter);
    moreInfoLabel->setStyleSheet("QLabel { background-color: #E67E22; font-size: 15px; }");

    mainVLayout->addWidget(infoLabel);
    mainVLayout->addWidget(moreInfoLabel);
}
