#ifndef CELLULES_H
#define CELLULES_H

#include "entite.hpp"

class Cellules : public Entite {
public:
    Cellules(bool etatInitial);

    bool DeterminerEtatSuivant(int voisinsVivants) override;
};


#endif // CELLULES_H
