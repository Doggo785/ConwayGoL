#ifndef AFFICHEUR_H
#define AFFICHEUR_H

#include "grille.h"

class Afficheur {
public:
    virtual ~Afficheur() {}
    virtual void Afficher(const Grille& grille) const = 0;
};

#endif // AFFICHEUR_H