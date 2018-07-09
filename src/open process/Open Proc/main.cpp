#include <QCoreApplication>
#include <QTextStream>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFile>
#include <QDebug>
#include <QProcess>
#include <QStandardPaths>

void writeBatFile(QString appPath)
{
    QFile batFile(appPath + "\\OpenProc.bat");
    if(batFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QTextStream textStream(&batFile);
        textStream << QString("start " + appPath + "\\Nana.exe");
    }
    else {
        qDebug() << batFile.errorString();
    }
    batFile.close();
}

void runBatFile(QString appPath)
{
    QProcess process;
    process.start("cmd.exe", QStringList() << "/c" << appPath + "\\Nana.exe");
    if(process.waitForStarted()) {
       process.waitForFinished();
       qDebug() << process.readAllStandardOutput();
    }
    else {
        qDebug() << "Failed to start";
    }
}

void readInitXml() {
    QStringList homePath = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
    QString filePath = homePath.first() + "/AppData/Local/Nana/Init.xml";
    QXmlStreamReader xmlReader;
    QFile file(filePath);
    file.open(QIODevice::ReadOnly);
    xmlReader.setDevice(&file);
    QString appPath;
    while(!xmlReader.atEnd()) {
        QXmlStreamReader::TokenType token = xmlReader.readNext();
        if(token == QXmlStreamReader::StartElement) {
            if(xmlReader.name() == "AppPath") {
                appPath = xmlReader.readElementText();
            }
        }
    }
    file.close();

    writeBatFile(appPath);
    runBatFile(appPath);
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    readInitXml();

    return a.exec();
}
