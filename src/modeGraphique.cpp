#include "../include/modeGraphique.h"
#include <SFML/Graphics.hpp>
#include <iostream>

ModeGraphique::ModeGraphique(Grille* grille) 
    : ModeSimulation(grille, iterationMax), window(sf::VideoMode(800, 600), "Jeu de la Vie") {
    // Paramètres de base pour dessiner les cellules
    cellShape.setSize(sf::Vector2f(10.f, 10.f)); // Taille des cellules
    cellShape.setFillColor(sf::Color::White);     // Couleur de base (cellules vivantes)
}

void ModeGraphique::afficher() {
    window.clear(); // Effacer la fenêtre à chaque itération

    // Afficher les cellules
    for (int x = 0; x < grille->getHauteur(); ++x) {
        for (int y = 0; y < grille->getLargeur(); ++y) {
            if (grille->getEntite(x, y) && grille->getEntite(x, y)->estVivante()) {
                // Si la cellule est vivante, la dessiner en vert
                cellShape.setPosition(y * 10.f, x * 10.f); // Positionner la cellule
                window.draw(cellShape); // Dessiner la cellule
            }
        }
    }

    window.display(); // Afficher tout ce qui a été dessiné
}

void ModeGraphique::simuler() {
    std::vector<std::vector<bool>> etatPrecedent = grille->capturerEtat();
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Mettre à jour la grille à chaque itération
        grille->MettreAJour();

        // Capturer l'état après la mise à jour
        std::vector<std::vector<bool>> etatActuel = grille->capturerEtat();

        // Vérifier la stabilité
        if (grille->etatIdentique(etatPrecedent, etatActuel)) {
            std::cout << "Le jeu est stable. Fin de la simulation." << std::endl;
            break;
        }

        // Mettre à jour l'état précédent
        etatPrecedent = etatActuel;

        afficher(); // Afficher la grille mise à jour

        // Limiter la vitesse de simulation
        sf::sleep(sf::milliseconds(100));
    }
}
