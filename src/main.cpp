#include "mainwindow.h"
#include <QApplication>
#include "core/hook.h"
#include <QDebug>
#include "core/runguard.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    RunGuard guard("a_hash_key");
    if (!guard.tryToRun()) {
         return 0;
    }

    MainWindow w;
    w.show();

    Hook hook;

    a.setQuitOnLastWindowClosed(false);

    return a.exec();
}
