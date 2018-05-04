#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;
    translator.load("../Source/Language-Pack_zh-CN.qm");
    a.installTranslator(&translator);

    MainWindow w;

    return a.exec();
}
