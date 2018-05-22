#include "mainwindow.h"
#include <QApplication>
#include "hook.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    a.setQuitOnLastWindowClosed(false);

    Hook hook;

    return a.exec();
}
