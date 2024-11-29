#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "entite.h"

class Obstacle : public Entite {
public:
    Obstacle(bool etatInitial) : Entite(etatInitial) {}

    void DeterminerEtatSuivant(int voisinsVivants) override {}
};

#endif // OBSTACLE_H