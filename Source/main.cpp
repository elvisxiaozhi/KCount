#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QDebug>
#include "initialisation.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator CHTranslator;
    CHTranslator.load(Initialisation::settings.value("InitSettings/AppPath").toString() + "/Language-Pack_zh-CN.qm");

    if(Initialisation::settings.value("InitSettings/Language").isValid()) {
        if(Initialisation::settings.value("InitSettings/Language").toString() == "Chinese") {
            a.installTranslator(&CHTranslator);
        }
        else {
            a.removeTranslator(&CHTranslator);
        }
    }
    else {
        QString appLanguage = QLocale::languageToString(QLocale::system().language());
        if(appLanguage == "Chinese") {
            a.installTranslator(&CHTranslator);
        }
        else {
            a.removeTranslator(&CHTranslator);
        }
    }

    MainWindow w;

    a.setQuitOnLastWindowClosed(false);
    return a.exec();
}
