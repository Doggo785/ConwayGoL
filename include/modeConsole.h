#ifndef MODECONSOLE_H
#define MODECONSOLE_H

#include "modeSimulation.h"
#include "afficheurConsole.h"

class ModeConsole : public ModeSimulation { 
private:
    AfficheurConsole afficheurConsole;
public:
    ModeConsole(Grille* grille, int iterationMax);
    void simuler() override;
};

#endif // MODECONSOLE_H