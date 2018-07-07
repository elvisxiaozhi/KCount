#include "mainwindow.h"
#include <QApplication>
#include "core/hook.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    Hook hook;

    a.setQuitOnLastWindowClosed(false);

    return a.exec();
}
