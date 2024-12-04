#ifndef MODECONSOLE_H
#define MODECONSOLE_H

#include "modeSimulation.hpp"
#include "afficheurConsole.hpp"

class ModeConsole : public ModeSimulation { 
private:
    std::string nomFichier;
    AfficheurConsole afficheurConsole;
    void saveFichier(int iteration) const;
public:
    ModeConsole(Grille* grille, int iterationMax, const std::string& nomFichier);
    void simuler() override;
};

#endif // MODECONSOLE_H