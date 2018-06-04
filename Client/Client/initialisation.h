#ifndef INITIALISATION_H
#define INITIALISATION_H

#include <QSettings>

class Initialisation
{
public:
    Initialisation();
    static QSettings settings;
    static bool quit;
};

#endif // INITIALISATION_H
