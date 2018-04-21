#include "settings.h"
#include <QCloseEvent>
#include <QGroupBox>
#include <QLabel>
#include <QIntValidator>
#include <QDebug>
#include "database.h"

QSettings Settings::startOnBootSetting("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);

Settings::Settings(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Settings");
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint); //hide message box icon
    setBasicLayout();

    connect(&setMsBox, &MessageBoxes::resetSettingsConfirmed, [this](){
        resetSettings();
        setMsBox.successMsBox.setText("Settings have been reset.");
        setMsBox.showSuccessMsBox();
    });
    connect(&setMsBox, &MessageBoxes::clearDatabaseConfirmed, [this](){
        clearDatabase();
        setMsBox.successMsBox.setText("Database has been cleared.");
        setMsBox.showSuccessMsBox();
        emit refreshLabels();
    });
    connect(&setMsBox, &MessageBoxes::resetAllConfirmed, [this](){
        resetAll();
        setMsBox.successMsBox.setText("Everything has been reset to default.");
        setMsBox.showSuccessMsBox();
    });
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
    setResetLayout();
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

void Settings::setResetLayout()
{
    QGroupBox *resetGBox = new QGroupBox("Reset", this);
    settingsContentVLayout->addWidget(resetGBox);

    QLabel *resetLabel = new QLabel(resetGBox);
    resetLabel->setText("<b>Be Careful!!! None of these actions changes can be undone!!!</b>");

    QHBoxLayout *resetLblHLayout = new QHBoxLayout;
    resetLblHLayout->addStretch();
    resetLblHLayout->addWidget(resetLabel);
    resetLblHLayout->addStretch();

    QPushButton *resetSettingsBtn = new QPushButton(resetGBox);
    resetSettingsBtn->setText("Reset Settings");

    QPushButton *clearDatabaseBtn = new QPushButton(resetGBox);
    clearDatabaseBtn->setText("Clear Database");

    QPushButton *resetAllBtn = new QPushButton(resetGBox);
    resetAllBtn->setText("Reset All");

    QPushButton *deleteEverythingBtn = new QPushButton(resetGBox);
    deleteEverythingBtn->setText("Delete Everything");

    QHBoxLayout *resetBtnHLayout = new QHBoxLayout;

    resetBtnHLayout->addWidget(resetSettingsBtn);
    resetBtnHLayout->addWidget(clearDatabaseBtn);
    resetBtnHLayout->addWidget(resetAllBtn);
    resetBtnHLayout->addWidget(deleteEverythingBtn);

    QVBoxLayout *resetVLayout = new QVBoxLayout;
    resetGBox->setLayout(resetVLayout);
    resetVLayout->addStretch();
    resetVLayout->addLayout(resetLblHLayout);
    resetVLayout->addLayout(resetBtnHLayout);
    resetVLayout->addStretch();

    connect(resetSettingsBtn, &QPushButton::clicked, this, &Settings::showResetSettingsMsBox);
    connect(clearDatabaseBtn, &QPushButton::clicked, this, &Settings::showClearDatabaseMsbox);
    connect(resetAllBtn, &QPushButton::clicked, this, &Settings::showResetAllMsBox);
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

void Settings::showResetSettingsMsBox()
{
    setMsBox.questionMsBox.setWindowTitle("Reset Settings");
    setMsBox.questionMsBox.setText("Are you sure you want to reset all the settings?");
    setMsBox.showQuestionMsBox(1);
}

void Settings::showClearDatabaseMsbox()
{
    setMsBox.questionMsBox.setWindowTitle("Clear DataBase");
    setMsBox.questionMsBox.setText("Are you sure you want to clear the database?");
    setMsBox.showQuestionMsBox(2);
}

void Settings::showResetAllMsBox()
{
    setMsBox.questionMsBox.setWindowTitle("Reset All");
    setMsBox.questionMsBox.setText("Are you sure you want to reset everything?");
    setMsBox.questionMsBox.setDetailedText("This will reset all your settings, clear the datebase and delete all the user data.");
    setMsBox.showQuestionMsBox(3);
}

void Settings::resetSettings()
{
    settings->remove("SettingsPage");
    startOnBootSetting.remove("Keylogger");
    DataBase::dataFilePathSettings.remove("DataFilePath");
    qDebug() << "Settings have been reset";
}

void Settings::resetAll()
{
    resetSettings();
    clearDatabase();
    DataBase::deleteDataFile();

    qDebug() << "Everything has been reset to default";
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
