#include "../include/afficheurGraphique.hpp"
#include "../include/cellules.hpp"
#include "../include/obstacle.hpp"
#include <iostream>

const float BARRE_NOIRE_LARGEUR = 250.f; // Largeur de la barre noire

AfficheurGraphique::AfficheurGraphique(sf::RenderWindow* fenetre) : window(fenetre) {
    cellShape.setSize(sf::Vector2f(20.f, 20.f)); // Taille des cellules
    positionActuelle = sf::Vector2i(0, 0); // Initialiser la position actuelle
    motifSelectionne = "Glider"; // Motif par défaut
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
    playPauseButton.setScale(0.075f, 0.075f);
    playPauseButton.setPosition(1035, 660);

    // Créer les autres boutons et compteurs
    boutonStart = creerBouton("Start", 435, 305, sf::Color::Black, 35);
    boutonQuit = creerBouton("Quit", 805, 305, sf::Color::Black, 35);
    compteurIterations = creerBouton("Iterations: 0", 1035, 100, sf::Color::White, 20);
    compteurCellulesVivantes = creerBouton("Cellules: 0", 1035, 140, sf::Color::White, 20);
    boutonVitessePlus = creerBouton("+ Vitesse", 1035, 20, sf::Color::White, 20);
    boutonVitesseMoins = creerBouton("- Vitesse", 1035, 60, sf::Color::White, 20);
    boutonQuitterSimulation = creerBouton("Quitter", 1135, 660, sf::Color::White, 20);
    boutonVider = creerBouton("Vider", 1145, 630, sf::Color::White, 20);

    // Créer les boutons de sélection de motif
    texteMotifSelectionne = creerBouton("Motif: Glider", 1050, 200, sf::Color::White, 17);
    boutonGlider = creerBouton("Glider", 1145, 380, sf::Color::White, 20);
    boutonBlinker = creerBouton("Blinker", 1145, 300, sf::Color::White, 20);
    boutonToad = creerBouton("Toad", 1035, 300, sf::Color::White, 20);
    boutonLWSS = creerBouton("LWSS", 1035, 380, sf::Color::White, 20);
    boutonPulsar = creerBouton("Pulsar", 1035, 330, sf::Color::White, 20);
    boutonCanon = creerBouton("Canon", 1035, 240, sf::Color::White, 20);
    boutonCellAlive = creerBouton("Cellule vivante", 1035, 460, sf::Color::White, 20);
    boutonCellDead = creerBouton("Cellule morte", 1035, 500, sf::Color::White, 20);
    boutonObstacleAlive = creerBouton("Obstacle vivant", 1035, 540, sf::Color::White, 20);
    boutonObstacleDead = creerBouton("Obstacle mort", 1035, 580, sf::Color::White, 20);
}

sf::Text AfficheurGraphique::creerBouton(const std::string& texte, float x, float y, sf::Color couleur, int taille) {
    sf::Text bouton;
    bouton.setFont(font);
    bouton.setString(texte);
    bouton.setCharacterSize(taille);
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

    // Calculez la taille des cellules en fonction de la taille de la fenêtre et des dimensions de la grille
    float cellWidth = (window->getSize().x - BARRE_NOIRE_LARGEUR) / static_cast<float>(grille.getLargeur());
    float cellHeight = window->getSize().y / static_cast<float>(grille.getHauteur());
    cellShape.setSize(sf::Vector2f(cellWidth, cellHeight));

    // Affichage de la grille
    for (int x = 0; x < grille.getHauteur(); ++x) {
        for (int y = 0; y < grille.getLargeur(); ++y) {
            Entite* entite = grille.getEntite(x, y);
            if (entite) {
                if (dynamic_cast<Cellules*>(entite)) {
                    cellShape.setFillColor(entite->estVivante() ? sf::Color::Green : sf::Color::Red);
                } else if (dynamic_cast<Obstacle*>(entite)) {
                    cellShape.setFillColor(entite->estVivante() ? sf::Color::Blue : sf::Color::Red);
                }
            } else {
                cellShape.setFillColor(sf::Color::Black);
            }
            cellShape.setPosition(y * cellWidth, x * cellHeight);
            window->draw(cellShape);
        }
    }

    // Dessiner la position actuelle
    sf::RectangleShape positionActuelleShape(sf::Vector2f(cellWidth, cellHeight));
    positionActuelleShape.setFillColor(sf::Color::Transparent);
    positionActuelleShape.setOutlineColor(sf::Color::White);
    positionActuelleShape.setOutlineThickness(2);
    positionActuelleShape.setPosition(this->positionActuelle.y * cellWidth, this->positionActuelle.x * cellHeight);
    window->draw(positionActuelleShape);

    // Dessiner la barre noire à droite de l'écran
    sf::RectangleShape barreNoire(sf::Vector2f(BARRE_NOIRE_LARGEUR, window->getSize().y));
    barreNoire.setFillColor(sf::Color::Black);
    barreNoire.setPosition(window->getSize().x - BARRE_NOIRE_LARGEUR, 0);
    window->draw(barreNoire);

    texteMotifSelectionne.setString("Motif: " + motifSelectionne);

    dessinerGrille(grille);

    window->draw(texteMotifSelectionne);
    window->draw(compteurIterations);
    window->draw(compteurCellulesVivantes);
    window->draw(boutonVitessePlus);
    window->draw(boutonVitesseMoins);
    window->draw(boutonVider);

    window->draw(boutonGlider);
    window->draw(boutonBlinker);
    window->draw(boutonToad);
    window->draw(boutonLWSS);
    window->draw(boutonPulsar);
    window->draw(boutonCanon);
    window->draw(boutonCellAlive);
    window->draw(boutonCellDead);
    window->draw(boutonObstacleAlive);
    window->draw(boutonObstacleDead);

    // Afficher le bouton "Play/Pause"
    window->draw(playPauseButton);
    window->draw(boutonQuitterSimulation);
    window->draw(boutonVider); // Bouton pour vider la grille
    window->display();
}

void AfficheurGraphique::afficherSimulation(const Grille& grille, int iterations, int cellulesVivantes, int vitesse) {
    window->clear(sf::Color::Black); // Effacer la fenêtre avec un fond noir

    // Calculez la taille des cellules en fonction de la taille de la fenêtre et des dimensions de la grille
    float cellWidth = (window->getSize().x - BARRE_NOIRE_LARGEUR) / static_cast<float>(grille.getLargeur());
    float cellHeight = window->getSize().y / static_cast<float>(grille.getHauteur());
    cellShape.setSize(sf::Vector2f(cellWidth, cellHeight));

    // Affichage de la grille
    for (int x = 0; x < grille.getHauteur(); ++x) {
        for (int y = 0; y < grille.getLargeur(); ++y) {
            Entite* entite = grille.getEntite(x, y);
            if (entite) {
                if (dynamic_cast<Cellules*>(entite)) {
                    cellShape.setFillColor(entite->estVivante() ? sf::Color::White : sf::Color::Red);
                } else if (dynamic_cast<Obstacle*>(entite)) {
                    cellShape.setFillColor(entite->estVivante() ? sf::Color::Blue : sf::Color::Red);
                }
            } else {
                cellShape.setFillColor(sf::Color::Black);
            }
            cellShape.setPosition(y * cellWidth, x * cellHeight);
            window->draw(cellShape);
        }
    }

    // Dessiner la grille blanche
    dessinerGrille(grille);

    // Dessiner la barre noire à droite de l'écran
    sf::RectangleShape barreNoire(sf::Vector2f(BARRE_NOIRE_LARGEUR, window->getSize().y));
    barreNoire.setFillColor(sf::Color::Black);
    barreNoire.setPosition(window->getSize().x - BARRE_NOIRE_LARGEUR, 0);
    window->draw(barreNoire);

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

    // Calculez la taille des cellules en fonction de la taille de la fenêtre et des dimensions de la grille
    float cellWidth = (window->getSize().x - BARRE_NOIRE_LARGEUR) / static_cast<float>(grille.getLargeur());
    float cellHeight = window->getSize().y / static_cast<float>(grille.getHauteur());

    // Dessiner les lignes horizontales
    for (int x = 0; x <= grille.getHauteur(); ++x) {
        lignes.append(sf::Vertex(sf::Vector2f(0, x * cellHeight), sf::Color::Black));
        lignes.append(sf::Vertex(sf::Vector2f(grille.getLargeur() * cellWidth, x * cellHeight), sf::Color::Black));
    }

    // Dessiner les lignes verticales
    for (int y = 0; y <= grille.getLargeur(); ++y) {
        lignes.append(sf::Vertex(sf::Vector2f(y * cellWidth, 0), sf::Color::Black));
        lignes.append(sf::Vertex(sf::Vector2f(y * cellWidth, grille.getHauteur() * cellHeight), sf::Color::Black));
    }

    window->draw(lignes);
}