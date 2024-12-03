#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "entite.h"

class Obstacle : public Entite {
public:
    Obstacle(bool etatInitial) : Entite(etatInitial) {}

    bool DeterminerEtatSuivant(int voisinsVivants) override {
        return etat;
    }
};

#endif // OBSTACLE_H