#ifndef AFFICHEURCONSOLE_H
#define AFFICHEURCONSOLE_H

#include "afficheur.h"

class AfficheurConsole : public Afficheur {
public:
    AfficheurConsole() {}
    virtual void Afficher(const Grille& grille) const override;

};

#endif // AFFICHEURCONSOLE_H