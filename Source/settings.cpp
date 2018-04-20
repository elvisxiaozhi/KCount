#include "settings.h"
#include <QCloseEvent>
#include <QGroupBox>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QIntValidator>
#include <QSpinBox>
#include <QDebug>

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
    mainVLayout->setSpacing(0);

    QHBoxLayout *btnHLayout = new QHBoxLayout;
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

    mainGBox = new QGroupBox(this);
    mainVLayout->addWidget(mainGBox);

    settingsContentVLayout = new QVBoxLayout;
    mainGBox->setLayout(settingsContentVLayout);

    QPushButton *okButton = new QPushButton(this);
    okButton->setText("Ok");
    QPushButton *cancelButton = new QPushButton(this);
    cancelButton->setText("Cancel");

    QHBoxLayout *bottomBtnHLayout = new QHBoxLayout;
    bottomBtnHLayout->addStretch(1);
    bottomBtnHLayout->addWidget(okButton);
    bottomBtnHLayout->addWidget(cancelButton);

    mainVLayout->addLayout(bottomBtnHLayout);

    setGeneralPage();

    connect(okButton, &QPushButton::clicked, [this](){ this->hide(); });
    connect(cancelButton, &QPushButton::clicked, [this](){ this->hide(); });
}

void Settings::setGeneralPage()
{
    QGroupBox *soundAlertGBox = new QGroupBox("Sound Alert", this);
    settingsContentVLayout->addWidget(soundAlertGBox);

    QLabel *soundAlertLabel = new QLabel(soundAlertGBox);
    soundAlertLabel->setText("Sound alert when reaching specific numbers");

    QCheckBox *soundAlertCheckBox = new QCheckBox(soundAlertGBox);

    QHBoxLayout *soundAlertHLayout = new QHBoxLayout;
    soundAlertHLayout->addWidget(soundAlertCheckBox);
    soundAlertHLayout->addWidget(soundAlertLabel);
    soundAlertHLayout->addStretch();

    QLabel *reachingNumLbl = new QLabel(soundAlertCheckBox);
    reachingNumLbl->setText("Make a sound when reaching each: ");

    QLineEdit *reachingNumEdit = new QLineEdit(soundAlertCheckBox);
    reachingNumEdit->setFixedWidth(50);
    reachingNumEdit->setText(QString::number(1000));
    reachingNumEdit->setValidator(new QIntValidator(1, 10000, reachingNumEdit));

    QLabel *reachingNumUnit = new QLabel(soundAlertCheckBox);
    reachingNumUnit->setText("Presses");

    QHBoxLayout *reachingNumHLayout = new QHBoxLayout;
    reachingNumHLayout->addWidget(reachingNumLbl);
    reachingNumHLayout->addWidget(reachingNumEdit);
    reachingNumHLayout->addWidget(reachingNumUnit);
    reachingNumHLayout->addStretch();

    QVBoxLayout *soundAlertVLayout = new QVBoxLayout;
    soundAlertVLayout->addStretch();
    soundAlertVLayout->addLayout(soundAlertHLayout);
    soundAlertVLayout->addLayout(reachingNumHLayout);
    soundAlertVLayout->addStretch();

    soundAlertGBox->setLayout(soundAlertVLayout);

    QGroupBox *autoSaveGBox = new QGroupBox("Auto Save", this);
    settingsContentVLayout->addWidget(autoSaveGBox);

    QCheckBox *autoSaveCheckBox = new QCheckBox(autoSaveGBox);
    autoSaveCheckBox->setChecked(true);

    QLabel *autoSaveLabel = new QLabel(autoSaveGBox);
    autoSaveLabel->setText("Automatically save to database in every: ");

    QSpinBox *autoSaveInterval = new QSpinBox(autoSaveGBox);
    autoSaveInterval->setValue(1);
    autoSaveInterval->setMaximum(24);
    autoSaveInterval->setMinimum(1);

    QLabel *autoSaveUnitLabel = new QLabel(autoSaveGBox);
    autoSaveUnitLabel->setText("Hour");

    QHBoxLayout *autoSaveHLayout = new QHBoxLayout;
    autoSaveHLayout->addWidget(autoSaveCheckBox);
    autoSaveHLayout->addWidget(autoSaveLabel);
    autoSaveHLayout->addWidget(autoSaveInterval);
    autoSaveHLayout->addWidget(autoSaveUnitLabel);
    autoSaveHLayout->addStretch();

    autoSaveGBox->setLayout(autoSaveHLayout);

    connect(autoSaveInterval, QOverload<int>::of(&QSpinBox::valueChanged),
          [=](int i){ qDebug() << i; });
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
