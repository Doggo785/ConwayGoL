#ifndef AFFICHEURCONSOLE_H
#define AFFICHEURCONSOLE_H

#include "afficheur.hpp"

class AfficheurConsole : public Afficheur {
public:
    AfficheurConsole() {}
    virtual void Afficher(const Grille& grille) override;

};

#endif // AFFICHEURCONSOLE_H