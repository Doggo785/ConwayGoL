#ifndef MODEGRAPHIQUE_H
#define MODEGRAPHIQUE_H

#include "modeSimulation.h"
#include "SFML/Graphics.hpp"

class ModeGraphique : public ModeSimulation {
private:
    sf::RenderWindow window; // Fenêtre d'affichage SFML
    sf::RectangleShape cellShape; // Forme des cellules
public:
    ModeGraphique(Grille* grille);
    void simuler() override;
    void afficher();
};

#endif // MODEGRAPHIQUE_H