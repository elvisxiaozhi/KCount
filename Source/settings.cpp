#include "settings.h"
#include <QCloseEvent>

Settings::Settings(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Settings");
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint); //hide message box icon
    setBasicLayout();
}

void Settings::closeEvent(QCloseEvent *event)
{
    event->ignore();
    this->hide();
}

void Settings::setBasicLayout()
{
    mainVLayout = new QVBoxLayout(this);
    this->setLayout(mainVLayout);

    btnHLayout = new QHBoxLayout;
    btnHLayout->setSpacing(0);
    mainVLayout->addLayout(btnHLayout);

    settingsBtns.resize(3);
    for(int i = 0; i < settingsBtns.size(); i++) {
        settingsBtns[i] = new QPushButton(this);
        settingsBtns[i]->setObjectName(QString::number(i));

        btnHLayout->addWidget(settingsBtns[i]);

        connect(settingsBtns[i], &QPushButton::clicked, this, &Settings::setFlatBtn);
    }
    settingsBtns[0]->setText("General");
    settingsBtns[0]->setFlat(true);
    settingsBtns[1]->setText("Statistics");
    settingsBtns[2]->setText("Database");
}

void Settings::setFlatBtn()
{
    QPushButton *btnSender = qobject_cast<QPushButton *>(sender());
    int btnIndex = btnSender->objectName().toInt();
    for(int i = 0; i < settingsBtns.size(); i++) {
        if(i != btnIndex) {
            settingsBtns[i]->setFlat(false);
        }
        else {
            settingsBtns[i]->setFlat(true);
        }
    }
}
