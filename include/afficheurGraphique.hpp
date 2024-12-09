#ifndef AFFICHEURGRAPHIQUE_HPP
#define AFFICHEURGRAPHIQUE_HPP

#include <SFML/Graphics.hpp>
#include "grille.hpp"

class AfficheurGraphique {
private:
    sf::RenderWindow* window;
    sf::Font font;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    sf::Text compteurIterations;
    sf::Text compteurCellulesVivantes;

    sf::RectangleShape cellShape;

    void dessinerGrille(const Grille& grille);

public:
    sf::Vector2i positionActuelle; // Position actuelle sur la grille
    std::string motifSelectionne; // Motif sélectionné
    sf::Text texteMotifSelectionne; // Texte pour afficher le motif sélectionné
    sf::Text boutonGlider; 
    sf::Text boutonBlinker; 
    sf::Text boutonToad;
    sf::Text boutonLWSS;
    sf::Text boutonPulsar;
    sf::Text boutonCanon;
    sf::Text boutonCellAlive;
    sf::Text boutonCellDead;
    sf::Text boutonObstacleAlive;
    sf::Text boutonObstacleDead;
    sf::Text boutonStart;
    sf::Text boutonQuit;
    sf::Text boutonVitessePlus;
    sf::Text boutonVitesseMoins;
    sf::Text boutonVider;
    sf::Texture playTexture; // Texture pour l'icône "Play"
    sf::Texture pauseTexture; // Texture pour l'icône "Pause"
    sf::Sprite playPauseButton;
    sf::Text boutonQuitterSimulation; // Nouveau bouton
    AfficheurGraphique(sf::RenderWindow* fenetre);
    void chargerRessources();
    void afficherMenu();
    void afficherEditeur(const Grille& grille);
    void afficherSimulation(const Grille& grille, int iterations, int cellulesVivantes, int vitesse);
    sf::Text creerBouton(const std::string& texte, float x, float y, sf::Color couleur, int taille);
};

#endif // AFFICHEURGRAPHIQUE_HPP