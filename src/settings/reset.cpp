#include "reset.h"
#include <QStyleOption>
#include <QPainter>
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>
#include "initialisation.h"
#include "core/database.h"
#include <QDir>

Reset::Reset(QWidget *parent) : QWidget(parent)
{
    this->hide();
    setStyleSheet("QWidget { background-color: white; }"
                  "QPushButton { background-color: #f0f8ff; font-size: 15px; border-radius: 2px; border: 1px solid #808080; padding: 6px 10px; margin: 5px 2px; }"
                  "QPushButton:hover { border: 2px solid red; margin: 5px 1px; }"
                  "QPushButton:pressed { background-color: #EC7063 }");
    createMainLayout();

    messageBox = new MessageBox(this);
    connect(messageBox, &MessageBox::resetConfirmed, this, &Reset::resetAll);
}

void Reset::createMainLayout()
{
    mainVLayout = new QVBoxLayout(this);

    QHBoxLayout *appPathHLayout = new QHBoxLayout();

    QLabel *appPathLbl = new QLabel(this);
    appPathLbl->setText("App Path: ");

    appPathEdit = new QLineEdit(this);
    appPathEdit->setMinimumWidth(800);
    appPathEdit->setText(Initialisation::settings.value("InitSettings/AppPath").toString());

    QHBoxLayout *resetHLayout = new QHBoxLayout();

    resetBtn = new QPushButton(this);
    resetBtn->setText("Reset All");

    appPathHLayout->addWidget(appPathLbl);
    appPathHLayout->addWidget(appPathEdit);
    appPathHLayout->addStretch();

    resetHLayout->addStretch();
    resetHLayout->addWidget(resetBtn);

    mainVLayout->addLayout(appPathHLayout);
    mainVLayout->addLayout(resetHLayout);
    setLayout(mainVLayout);

    connect(appPathEdit, &QLineEdit::textChanged, [this](){ Initialisation::settings.setValue("InitSettings/AppPath", appPathEdit->text()); Initialisation::writeInitXml(); });
    connect(resetBtn, &QPushButton::clicked, [this](){ messageBox->setWindowTitle(tr("Reset All")); messageBox->showQuestionMsBox(); });
}

void Reset::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

void Reset::resetAll()
{
    emit resetStarting();
    appPathEdit->setText("");

    QDir dir(Database::dataPath);
    dir.removeRecursively();

    Initialisation::settings.remove("InitSettings");
    Initialisation::startOnBoot.remove("Nana");

    messageBox->showSuccessMsBox("App has been set to default.");
}
