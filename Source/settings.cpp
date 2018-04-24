#include "settings.h"
#include <QCloseEvent>
#include <QGroupBox>
#include <QLabel>
#include <QIntValidator>
#include <QDebug>
#include "database.h"
#include <QProcess>
#include <QApplication>

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
        emit databaseCleared();
    });
    connect(&setMsBox, &MessageBoxes::resetAllConfirmed, [this](){
        resetAll(); //because resetAll() function needs to call resetSettings() and clearDatabase() and does not want showSuccessMsBox()
        setMsBox.successMsBox.setText("Everything has been reset to default.");
        setMsBox.showSuccessMsBox();
    });
    connect(&setMsBox, &MessageBoxes::deleteAppConfirmed, this, &Settings::deleteApp);
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

    setSoundAlertLayout();
    setResetLayout();

    //call resetChanges function when the program run to initiate variables
    //see updateLabels() in MainWinodw to see why
    //because if this function does not call, setSettingsPage.reachingNumEdit->text().toInt() will be 0 when the program run and the program will crash when reset the value to reachingNumEdit
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

    QPushButton *deleteAppBtn = new QPushButton(resetGBox);
    deleteAppBtn->setText("Delete App");

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
    settings->setValue("SettingsPage/soundAlertCheckBox", isSoundAlertCheckBoxChecked);
    settings->setValue("SettingsPage/reachingNumEdit", reachingNum);
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

void Settings::showDeleteAppMsBox()
{
    setMsBox.questionMsBox.setWindowTitle("Delete App");
    setMsBox.questionMsBox.setText("Are you sure you want to delete this app?");
    setMsBox.questionMsBox.setDetailedText("This will delete this app and everything relevant to this app completely.");
    setMsBox.showQuestionMsBox(4);
}

void Settings::deleteApp()
{
    QString batFilePath = writeBatFile();
    resetAll();
    setMsBox.successMsBox.setText("Keylogger has been successfully removed from this computer");
    setMsBox.successMsBox.setInformativeText("You can re-download it on our <a style='text-decoration:none;' href='https://github.com/elvisxiaozhi/Keyboard-Tracker/releases'>website</a>.");
    setMsBox.showSuccessMsBox();

    QProcess::startDetached("cmd.exe", QStringList() << "/c" << batFilePath);
    qApp->quit();
}

void Settings::resetSettings()
{
    settings->remove("SettingsPage");
    startOnBootSetting.remove("Keylogger");
    DataBase::appPathSetting.remove("AppPath");
    emit uncheckStartOnBootAct();
}

void Settings::resetAll()
{
    resetSettings();
    clearDatabase();
    DataBase::deleteDataFile(DataBase::dataPath);
}

QString Settings::writeBatFile()
{
    QString batFilePath = QString(DataBase::appPathSetting.value("AppPath").toString() + "/%1.bat").arg("Delete Keylogger");
    QFile batFile(batFilePath);
    if(batFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QTextStream textStream(&batFile);
        textStream << QString("@RD /S /Q \"%1\"").arg(DataBase::appPathSetting.value("AppPath").toString());
        qDebug() << "Done";
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
}
