#include "settings.h"
#include <QCloseEvent>
#include <QGroupBox>
#include <QLabel>
#include <QIntValidator>
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

    connect(okButton, &QPushButton::clicked, this, &Settings::saveChanges);
    connect(cancelButton, &QPushButton::clicked, [this](){ this->hide(); });
}

void Settings::setGeneralPage()
{
    //must initiate first, or the general settings will not show correctly
    isSoundAlertCheckBoxChecked = true;
    reachingNum = QString::number(1000);
    autoSaveIntervalNum = 1;
    isAutoSaveCheckBoxChecked = true;

    setSoundAlertLayout();
    setAutoSaveLayout();

//    settings->remove("SettingsPage/soundAlertCheckBox");
//    settings->remove("SettingsPage/reachingNumEdit");
//    settings->remove("SettingsPage/autoSaveCheckBox");
//    settings->remove("SettingsPage/autoSaveInterval");

    resetChanges();
}

void Settings::setSoundAlertLayout()
{
    QGroupBox *soundAlertGBox = new QGroupBox("Sound Alert", this);
    settingsContentVLayout->addWidget(soundAlertGBox);

    QLabel *soundAlertLabel = new QLabel(soundAlertGBox);
    soundAlertLabel->setText("Sound alert when reaching specific numbers");

    soundAlertCheckBox = new QCheckBox(soundAlertGBox);

    settings = new QSettings("My Company", "Keylogger");

    QHBoxLayout *soundAlertHLayout = new QHBoxLayout;
    soundAlertHLayout->addWidget(soundAlertCheckBox);
    soundAlertHLayout->addWidget(soundAlertLabel);
    soundAlertHLayout->addStretch();

    QLabel *reachingNumLbl = new QLabel(soundAlertCheckBox);
    reachingNumLbl->setText("Make a sound when reaching each: ");

    reachingNumEdit = new QLineEdit(soundAlertCheckBox);
    reachingNumEdit->setFixedWidth(50);
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

    connect(soundAlertCheckBox, &QCheckBox::clicked, [this](){ isSoundAlertCheckBoxChecked = soundAlertCheckBox->isChecked(); });
    connect(reachingNumEdit, &QLineEdit::textChanged, [this](QString text){ reachingNum = text; });
}

void Settings::setAutoSaveLayout()
{
    QGroupBox *autoSaveGBox = new QGroupBox("Auto Save", this);
    settingsContentVLayout->addWidget(autoSaveGBox);

    autoSaveCheckBox = new QCheckBox(autoSaveGBox);

    QLabel *autoSaveLabel = new QLabel(autoSaveGBox);
    autoSaveLabel->setText("Automatically save to database in every: ");

    autoSaveInterval = new QSpinBox(autoSaveGBox);

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

    connect(autoSaveCheckBox, &QCheckBox::clicked, [=](){ isAutoSaveCheckBoxChecked = autoSaveCheckBox->isChecked(); });
    connect(autoSaveInterval, QOverload<int>::of(&QSpinBox::valueChanged),
            [=](int i){ autoSaveIntervalNum = i; });
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

void Settings::saveChanges()
{
    settings->setValue("SettingsPage/soundAlertCheckBox", isSoundAlertCheckBoxChecked);
    settings->setValue("SettingsPage/reachingNumEdit", reachingNum);
    settings->setValue("SettingsPage/autoSaveCheckBox", isAutoSaveCheckBoxChecked);
    settings->setValue("SettingsPage/autoSaveInterval", autoSaveIntervalNum);
    this->hide();
}

void Settings::resetChanges()
{
    if(settings->value("SettingsPage/soundAlertCheckBox").isValid()) {
        if(settings->value("SettingsPage/soundAlertCheckBox") == true) {
            soundAlertCheckBox->setChecked(true);
        }
        else {
            soundAlertCheckBox->setChecked(false);
        }
    }
    else {
        soundAlertCheckBox->setChecked(true);
    }

    if(settings->value("SettingsPage/reachingNumEdit").isValid()) {
        reachingNumEdit->setText(QString::number(settings->value("SettingsPage/reachingNumEdit").toInt()));
    }
    else {
        reachingNumEdit->setText(QString::number(1000));
    }

    if(settings->value("SettingsPage/autoSaveCheckBox").isValid()) {
        if(settings->value("SettingsPage/autoSaveCheckBox") == true) {
            autoSaveCheckBox->setChecked(true);
        }
        else {
            autoSaveCheckBox->setChecked(false);
        }
    }
    else {
        autoSaveCheckBox->setChecked(true);
    }

    if(settings->value("SettingsPage/autoSaveInterval").isValid()) {
        autoSaveInterval->setValue(settings->value("SettingsPage/autoSaveInterval").toInt());
    }
    else {
        autoSaveInterval->setValue(1);
    }
}
