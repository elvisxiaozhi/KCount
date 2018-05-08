#include "settings.h"
#include <QGroupBox>
#include <QFile>
#include <QLabel>
#include <QIntValidator>
#include <QDebug>
#include "database.h"
#include <QProcess>
#include <QApplication>
#include <initialisation.h>

QSettings Settings::startOnBootSetting("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
QSettings Settings::languageSettings("My Company", "KCount");

Settings::Settings(QWidget *parent) : QWidget(parent)
{
    setWindowTitle(tr("Settings"));
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint); //hide message box icon
    setBasicLayout();
    setWindowStyleSheet();

    connect(&msBox, &MessageBox::resetSettingsConfirmed, [this](){
        resetSettings();
        msBox.setText(tr("Settings have been reset."));
        msBox.showSuccessMsBox();
    });
    connect(&msBox, &MessageBox::clearDatabaseConfirmed, [this](){
        Database::clearDatabase();
        msBox.setText(tr("Database has been cleared."));
        msBox.showSuccessMsBox();
        emit databaseCleared();
    });
    connect(&msBox, &MessageBox::resetAllConfirmed, [this](){
        resetAll(); //because resetAll() function needs to call resetSettings() and clearDatabase() and does not want showSuccessMsBox()
        msBox.setText(tr("Everything has been reset to default."));
        msBox.showSuccessMsBox();
    });
    connect(&msBox, &MessageBox::deleteAppConfirmed, this, &Settings::deleteApp);
}

void Settings::setBasicLayout()
{
    mainVLayout = new QVBoxLayout(this);
    this->setLayout(mainVLayout);
    mainVLayout->setSpacing(0);

    mainGBox = new QGroupBox(this);
    mainVLayout->addWidget(mainGBox);

    settingsContentVLayout = new QVBoxLayout;
    mainGBox->setLayout(settingsContentVLayout);

    QPushButton *okButton = new QPushButton(this);
    okButton->setText(tr("Ok"));
    QPushButton *cancelButton = new QPushButton(this);
    cancelButton->setText(tr("Cancel"));

    QHBoxLayout *bottomBtnHLayout = new QHBoxLayout;
    bottomBtnHLayout->addStretch(1);
    bottomBtnHLayout->addWidget(okButton);
    bottomBtnHLayout->addWidget(cancelButton);

    mainVLayout->addLayout(bottomBtnHLayout);

    setGeneralPage();

    connect(okButton, &QPushButton::clicked, this, &Settings::saveChanges);
    connect(cancelButton, &QPushButton::clicked, [this](){ this->close(); });
}

void Settings::setWindowStyleSheet()
{
    setStyleSheet(
                "QWidget { background-color: #FAE5D3; font-family: Comic Sans MS; }"
                ".QPushButton { background-color: #3498DB; font-size: 15px; color: white; border-radius: 15px; border: 2px solid #FF5A5F; padding: 5px 10px; margin: 5px 2px; }"
                ".QPushButton:hover { background-color: #BB8FCE; font-size: 18px; padding: 3px 5px; }"
                ".QPushButton:pressed { background-color: #EC7063 }"
                "QPushButton#dangerBtn { background-color: #F4F6F6; color: #C0392B; border-radius: 5px;}"
                "QPushButton#dangerBtn:hover { background-color: #CB4335; color: white; font-size: 16px; }"
                "QPushButton#dangerBtn:pressed { background-color: #641E16 }"
                "QGroupBox#dangerGBox { color: #C0392B; font-size: 15px; }"
                );
}

void Settings::setGeneralPage()
{
    setSoundAlertLayout();
    setLanguageLayout();
    setResetLayout();

    resetChanges(); //this function is used to initiate settings page
}

void Settings::setSoundAlertLayout()
{
    QGroupBox *soundAlertGBox = new QGroupBox(tr("Sound Alert"), this);
    settingsContentVLayout->addWidget(soundAlertGBox);

    QLabel *soundAlertLabel = new QLabel(soundAlertGBox);
    soundAlertLabel->setText(tr("Sound alert when reaching specific numbers"));

    soundAlertCheckBox = new QCheckBox(soundAlertGBox);

    settings = new QSettings("My Company", "KCount");

    QHBoxLayout *soundAlertHLayout = new QHBoxLayout;
    soundAlertHLayout->addWidget(soundAlertCheckBox);
    soundAlertHLayout->addWidget(soundAlertLabel);
    soundAlertHLayout->addStretch();

    QLabel *reachingNumLbl = new QLabel(soundAlertCheckBox);
    reachingNumLbl->setText(tr("Make a sound when reaching each: "));

    reachingNumEdit = new QLineEdit(soundAlertCheckBox);
    reachingNumEdit->setFixedWidth(50);
    reachingNumEdit->setValidator(new QIntValidator(1, 10000, reachingNumEdit));

    QLabel *reachingNumUnit = new QLabel(soundAlertCheckBox);
    reachingNumUnit->setText(tr("Presses"));

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
}

void Settings::setLanguageLayout()
{
    QGroupBox *languageGBox = new QGroupBox(tr("Language"), this);
    settingsContentVLayout->addWidget(languageGBox);

    QLabel *languageLbl = new QLabel(languageGBox);
    languageLbl->setText(tr("Change app language to "));

    languageBox = new QComboBox(languageGBox);
    languageBox->addItem(tr("English"));
    languageBox->addItem(tr("Chinese"));

    QHBoxLayout *languageHLayout = new QHBoxLayout;
    languageHLayout->addWidget(languageLbl);
    languageHLayout->addWidget(languageBox);
    languageHLayout->addStretch();

    languageGBox->setLayout(languageHLayout);
}

void Settings::setResetLayout()
{
    QGroupBox *resetGBox = new QGroupBox(tr("Danger Zone"), this);
    settingsContentVLayout->addWidget(resetGBox);
    resetGBox->setObjectName("dangerGBox");

    QLabel *resetLabel = new QLabel(resetGBox);
    resetLabel->setText(tr("<b>Be Careful!!! None of these actions changes can be undone!!!</b>"));

    QHBoxLayout *resetLblHLayout = new QHBoxLayout;
    resetLblHLayout->addStretch();
    resetLblHLayout->addWidget(resetLabel);
    resetLblHLayout->addStretch();

    QPushButton *resetSettingsBtn = new QPushButton(resetGBox);
    resetSettingsBtn->setText(tr("Reset Settings"));
    resetSettingsBtn->setObjectName("dangerBtn");

    QPushButton *clearDatabaseBtn = new QPushButton(resetGBox);
    clearDatabaseBtn->setText(tr("Clear Database"));
    clearDatabaseBtn->setObjectName("dangerBtn");

    QPushButton *resetAllBtn = new QPushButton(resetGBox);
    resetAllBtn->setText(tr("Reset All"));
    resetAllBtn->setObjectName("dangerBtn");

    QPushButton *deleteAppBtn = new QPushButton(resetGBox);
    deleteAppBtn->setText(tr("Delete App"));
    deleteAppBtn->setObjectName("dangerBtn");

    QHBoxLayout *resetBtnHLayout = new QHBoxLayout;

    resetBtnHLayout->addWidget(resetSettingsBtn);
    resetBtnHLayout->addWidget(clearDatabaseBtn);
    resetBtnHLayout->addWidget(resetAllBtn);
    resetBtnHLayout->addWidget(deleteAppBtn);

    QVBoxLayout *resetVLayout = new QVBoxLayout;
    resetGBox->setLayout(resetVLayout);
    resetVLayout->addStretch();
    resetVLayout->addLayout(resetLblHLayout);
    resetVLayout->addLayout(resetBtnHLayout);
    resetVLayout->addStretch();

    connect(resetSettingsBtn, &QPushButton::clicked, this, &Settings::showResetSettingsMsBox);
    connect(clearDatabaseBtn, &QPushButton::clicked, this, &Settings::showClearDatabaseMsbox);
    connect(resetAllBtn, &QPushButton::clicked, this, &Settings::showResetAllMsBox);
    connect(deleteAppBtn, &QPushButton::clicked, this, &Settings::showDeleteAppMsBox);
}

void Settings::saveChanges()
{
    settings->setValue("SettingsPage/soundAlertCheckBox", soundAlertCheckBox->isChecked());
    settings->setValue("SettingsPage/reachingNumEdit", reachingNumEdit->text());
    if(Initialisation::settings.value("InitSettings/Language").toString() != languageBox->currentText()) {
        Initialisation::settings.setValue("InitSettings/Language", languageBox->currentText());
        msBox.setText(tr("Changed Language will be apply next time you run this app."));
        msBox.showSuccessMsBox();
    }
    this->close(); //this must be after msBox.showSuccessMsBox() is called
}

void Settings::showResetSettingsMsBox()
{
    msBox.setWindowTitle(tr("Reset Settings"));
    msBox.setText(tr("Are you sure you want to reset all the settings?"));
    msBox.showQuestionMsBox(1);
}

void Settings::showClearDatabaseMsbox()
{
    msBox.setWindowTitle(tr("Clear DataBase"));
    msBox.setText(tr("Are you sure you want to clear the database?"));
    msBox.showQuestionMsBox(2);
}

void Settings::showResetAllMsBox()
{
    msBox.setWindowTitle(tr("Reset All"));
    msBox.setText(tr("Are you sure you want to reset everything?"));
    msBox.setDetailedText(tr("This will reset all your settings, clear the datebase and delete all the user data."));
    msBox.showQuestionMsBox(3);
}

void Settings::showDeleteAppMsBox()
{
    msBox.setWindowTitle(tr("Delete App"));
    msBox.setText(tr("Are you sure you want to delete this app?"));
    msBox.setDetailedText(tr("This will delete this app and everything relevant to this app completely."));
    msBox.showQuestionMsBox(4);
}

void Settings::deleteApp()
{
    QString batFilePath = writeBatFile();
    resetAll();
    msBox.setText(tr("KCount has been successfully removed from this Company"));
    msBox.setInformativeText(tr("You can re-download it on our <a style='text-decoration:none;' href='https://github.com/elvisxiaozhi/KCount/releases'>website</a>."));
    msBox.showSuccessMsBox();

    QProcess::startDetached("cmd.exe", QStringList() << "/c" << batFilePath);
    qApp->quit();
}

void Settings::resetSettings()
{
    settings->clear();
    startOnBootSetting.remove("KCount");
    Initialisation::settings.clear();
    //must initate the three lines below when resetting settings page
    soundAlertCheckBox->setChecked(true);
    reachingNumEdit->setText(QString::number(1000));
    languageBox->setCurrentText("English");
    emit uncheckStartOnBootAct();
}

void Settings::resetAll()
{
    resetSettings();
    Database::deleteDataFile(Database::dataPath);
}

QString Settings::writeBatFile()
{
    QString batFilePath = QString(Initialisation::settings.value("InitSettings/AppPath").toString() + "/%1.bat").arg("Delete KCount");
    QFile batFile(batFilePath);
    if(batFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QTextStream textStream(&batFile);
        textStream << QString("@RD /S /Q \"%1\"").arg(Initialisation::settings.value("InitSettings/AppPath").toString());
    }
    else {
        qDebug() << batFile.errorString();
    }

    return batFilePath;
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

    if(Initialisation::settings.value("InitSettings/Language").isValid()) {
        languageBox->setCurrentText(Initialisation::settings.value("InitSettings/Language").toString());
    }
    else {
        languageBox->setCurrentText("English");
    }
}
