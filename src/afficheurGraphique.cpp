#include "afficheurGraphique.hpp"
#include <iostream>

AfficheurGraphique::AfficheurGraphique(sf::RenderWindow* fenetre) : window(fenetre) {
    cellShape.setSize(sf::Vector2f(20.f, 20.f)); // Taille des cellules
}

void AfficheurGraphique::chargerRessources() {
    // Charger le fond d'écran
    if (!backgroundTexture.loadFromFile("assets/background.png")) {
        std::cerr << "Erreur lors du chargement du fond d'écran." << std::endl;
    }
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(1280.f / backgroundTexture.getSize().x, 720.f / backgroundTexture.getSize().y);

    // Charger la police
    if (!font.loadFromFile("assets/Archive.ttf")) {
        std::cerr << "Erreur lors du chargement de la police." << std::endl;
    }

    // Charger les textures des icônes "Play" et "Pause"
    if (!playTexture.loadFromFile("assets/play.png")) {
        std::cerr << "Erreur lors du chargement de l'icône Play." << std::endl;
    }
    if (!pauseTexture.loadFromFile("assets/pause.png")) {
        std::cerr << "Erreur lors du chargement de l'icône Pause." << std::endl;
    }

    // Initialiser le sprite du bouton "Play/Pause"
    playPauseButton.setTexture(playTexture);
    playPauseButton.setScale(0.1f, 0.1f);
    playPauseButton.setPosition(20, 660);

    // Créer les autres boutons et compteurs
    boutonStart = creerBouton("Start", 435, 305, sf::Color::Black);
    boutonQuit = creerBouton("Quit", 805, 305, sf::Color::Black);
    compteurIterations = creerBouton("Iterations: 0", 20, 20, sf::Color::Red);
    compteurCellulesVivantes = creerBouton("Cellules: 0", 20, 60, sf::Color::Red);
    boutonVitessePlus = creerBouton("+ Vitesse", 1000, 20, sf::Color::Red);
    boutonVitesseMoins = creerBouton("- Vitesse", 1000, 60, sf::Color::Red);
    boutonQuitterSimulation = creerBouton("Quitter", 500, 660, sf::Color::Red);
}

sf::Text AfficheurGraphique::creerBouton(const std::string& texte, float x, float y, sf::Color couleur) {
    sf::Text bouton;
    bouton.setFont(font);
    bouton.setString(texte);
    bouton.setCharacterSize(40);
    bouton.setFillColor(couleur);
    bouton.setPosition(x, y);
    return bouton;
}

void AfficheurGraphique::afficherMenu() {
    window->clear();
    window->draw(backgroundSprite);
    window->draw(boutonStart);
    window->draw(boutonQuit);
    window->display();
}

void AfficheurGraphique::afficherEditeur(const Grille& grille) {
    window->clear(sf::Color::Black); // Effacer la fenêtre avec un fond noir

    // Affichage de la grille
    for (int x = 0; x < grille.getHauteur(); ++x) {
        for (int y = 0; y < grille.getLargeur(); ++y) {
            Entite* entite = grille.getEntite(x, y);
            cellShape.setFillColor(entite ? sf::Color::Green : sf::Color::Black);
            cellShape.setPosition(y * 20.f, x * 20.f);
            window->draw(cellShape);
        }
    }

    window->draw(boutonStart); // Bouton pour lancer la simulation
    window->draw(boutonQuit);
    window->display();
}

void AfficheurGraphique::afficherSimulation(const Grille& grille, int iterations, int cellulesVivantes, int vitesse) {
    window->clear(sf::Color::Black); // Effacer la fenêtre avec un fond noir

    // Affichage de la grille
    for (int x = 0; x < grille.getHauteur(); ++x) {
        for (int y = 0; y < grille.getLargeur(); ++y) {
            Entite* entite = grille.getEntite(x, y);
            cellShape.setFillColor(entite ? sf::Color::Green : sf::Color::Black);
            cellShape.setPosition(y * 20.f, x * 20.f);
            window->draw(cellShape);
        }
    }

    // Dessiner la grille blanche
    dessinerGrille(grille);

    // Mettre à jour et afficher les compteurs
    compteurIterations.setString("Iterations: " + std::to_string(iterations));
    compteurCellulesVivantes.setString("Cellules: " + std::to_string(cellulesVivantes));
    boutonVitessePlus.setString("+ Vitesse (" + std::to_string(vitesse) + ")");
    boutonVitesseMoins.setString("- Vitesse");

    window->draw(compteurIterations);
    window->draw(compteurCellulesVivantes);
    window->draw(boutonVitessePlus);
    window->draw(boutonVitesseMoins);

    // Afficher le bouton "Play/Pause"
    window->draw(playPauseButton);
    window->draw(boutonQuitterSimulation);

    window->display();
}

void AfficheurGraphique::dessinerGrille(const Grille& grille) {
    sf::VertexArray lignes(sf::Lines);

    // Dessiner les lignes horizontales
    for (int x = 0; x <= grille.getHauteur(); ++x) {
        lignes.append(sf::Vertex(sf::Vector2f(0, x * 20.f), sf::Color::White));
        lignes.append(sf::Vertex(sf::Vector2f(grille.getLargeur() * 20.f, x * 20.f), sf::Color::White));
    }

    // Dessiner les lignes verticales
    for (int y = 0; y <= grille.getLargeur(); ++y) {
        lignes.append(sf::Vertex(sf::Vector2f(y * 20.f, 0), sf::Color::White));
        lignes.append(sf::Vertex(sf::Vector2f(y * 20.f, grille.getHauteur() * 20.f), sf::Color::White));
    }

    window->draw(lignes);
}