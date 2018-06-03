#ifndef INITIALISATION_H
#define INITIALISATION_H

#include <QSettings>

class Initialisation
{
public:
    Initialisation();
    static QSettings settings;
    static int exitCode;
};

#endif // INITIALISATION_H
