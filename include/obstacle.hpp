#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "entite.hpp"

class Obstacle : public Entite {
public:
    Obstacle(bool etatInitial) : Entite(etatInitial) {}

    bool DeterminerEtatSuivant(int voisinsVivants) override {
        return etat;  // L'état de l'obstacle reste inchangé
    }
    
};

#endif // OBSTACLE_H