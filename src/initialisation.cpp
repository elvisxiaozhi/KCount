#include "initialisation.h"
#include <QDebug>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFile>
#include "core/database.h"

QSettings Initialisation::settings("InitSettings", "Nana");
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
//        for(auto mapKey : xmlMap.keys()) {
//            xmlWriter.writeStartElement("App");
            xmlWriter.writeTextElement("Name", "mapKey");
            xmlWriter.writeTextElement("IsDefaultKey", "Y");
//            xmlWriter.writeEndElement();
//        }

//        xmlWriter.writeEndElement();
        xmlWriter.writeEndDocument();
        file.close();
    }
    qDebug() << "Done" << path;
}
