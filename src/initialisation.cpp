#include "initialisation.h"
#include <QDebug>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFile>
#include "core/database.h"

QSettings Initialisation::settings("InitSettings", "Nana");
QSettings Initialisation::startOnBoot("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
//bool Initialisation::quit = false;

Initialisation::Initialisation()
{
}

void Initialisation::writeInitXml()
{
    QString path = Database::dataPath + "/Init.xml";

    QFile file(path);
    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    if(file.open(QIODevice::WriteOnly)) {
        xmlWriter.writeStartDocument();
        xmlWriter.writeStartElement("Init");

        xmlWriter.writeTextElement("AppPath", settings.value("InitSettings/AppPath").toString());
        if(startOnBoot.value("Nana").isValid()) {
            xmlWriter.writeTextElement("StartOnBoot", "True");
        }
        else {
            xmlWriter.writeTextElement("StartOnBoot", "False");
        }

        xmlWriter.writeEndDocument();
        file.close();
    }
}
