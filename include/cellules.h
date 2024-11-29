#ifndef CELLULES_H
#define CELLULES_H

#include "entite.h"

class Cellules : public Entite {
public:
    Cellules(bool etatInitial);

    void DeterminerEtatSuivant(int voisinsVivants) override;
};


#endif // CELLULES_H
