#include <QCoreApplication>
#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <QProcess>

QString batFilePath = "C:\\Users\\Theodore\\Desktop\\test.bat";

QString writeBatFile()
{
    QFile batFile(batFilePath);
    if(batFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QTextStream textStream(&batFile);
        textStream << "start C:\\Users\\Theodore\\OneDrive\\Projects\\Nana\\Nana.exe";
    }
    else {
        qDebug() << batFile.errorString();
    }
    batFile.close();

    return batFilePath;
}

void runBatFile()
{
    QProcess process;
    process.start("cmd.exe", QStringList() << "/c" << "C://Users//Theodore//Desktop//test.bat");
    if(process.waitForStarted()) {
       process.waitForFinished();
       qDebug() << process.readAllStandardOutput();
    }
    else {
        qDebug() << "Failed to start";
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    writeBatFile();
    runBatFile();

    return a.exec();
}
