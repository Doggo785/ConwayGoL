#ifndef MODESIMULATION_H
#define MODESIMULATION_H

#include "grille.hpp"

class ModeSimulation { 
protected:
    Grille* grille;
    int iterationMax;
public:
    ModeSimulation(Grille* grille, int iterationMax)
    : grille(grille), iterationMax(iterationMax) {}
    virtual ~ModeSimulation();

    virtual void simuler() = 0;
};

#endif // MODESIMULATION_H