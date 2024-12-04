#ifndef MODEGRAPHIQUE_H
#define MODEGRAPHIQUE_H

#include "modeSimulation.hpp"
#include "SFML/Graphics.hpp"
#include <chrono>

enum EtatSimulation {
    Attente,    // Avant le démarrage
    EnCours,    // Simulation en cours
    Terminee    // Simulation terminée
};

class ModeGraphique : public ModeSimulation {
private:
    sf::RenderWindow window;
    sf::RectangleShape cellShape;
    sf::Font font;
    sf::Text iterationText, timerText, boutonPause, boutonQuitter, boutonDemarrer, messageFin;

    EtatSimulation etatSimulation = Attente; // État initial : attente

    bool enPause = false;

    void afficher();
    void afficherTexte();
    void gererEvenements();
    void demarrerSimulation(); // Lancer la simulation
    bool grilleStable() const; // Vérifier si la grille est stable

public:
    ModeGraphique(Grille* grille);
    void simuler() override;
};


#endif
