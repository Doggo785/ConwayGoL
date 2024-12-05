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

public:
    sf::Text boutonStart;
    sf::Text boutonQuit;
    sf::Text boutonVitessePlus;
    sf::Text boutonVitesseMoins;
    sf::Texture playTexture; // Texture pour l'icône "Play"
    sf::Texture pauseTexture; // Texture pour l'icône "Pause"
    sf::Sprite playPauseButton;
    sf::Text boutonQuitterSimulation; // Nouveau bouton
    AfficheurGraphique(sf::RenderWindow* fenetre);
    void chargerRessources();
    void afficherMenu();
    void afficherEditeur(const Grille& grille);
    void afficherSimulation(const Grille& grille, int iterations, int cellulesVivantes, int vitesse);
    void dessinerGrille(const Grille& grille);
    sf::Text creerBouton(const std::string& texte, float x, float y, sf::Color couleur);
};

#endif // AFFICHEURGRAPHIQUE_HPP
