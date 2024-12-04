#include "../include/modeGraphique.hpp"
#include <SFML/Graphics.hpp>
#include "../include/cellules.hpp"
#include "../include/obstacle.hpp"
#include <iostream>

ModeGraphique::ModeGraphique(Grille* grille)
    : ModeSimulation(grille, iterationMax), window(sf::VideoMode(800, 600), "Jeu de la Vie") {
    // Paramètres de base pour dessiner les cellules
    cellShape.setSize(sf::Vector2f(10.f, 10.f));

if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"))
{
    std::cerr << "Erreur lors du chargement de la police" << std::endl;
}
    float boutonX = window.getSize().x - 150;  // 150 est la largeur estimée du bouton
    float boutonY = 10.f;  // En haut, à 10 pixels du bord supérieur
    // Initialisation des textes
    iterationText.setFont(font);
    iterationText.setCharacterSize(20);
    iterationText.setFillColor(sf::Color::White);
    iterationText.setPosition(10, 10);

    timerText.setFont(font);
    timerText.setCharacterSize(20);
    timerText.setFillColor(sf::Color::White);
    timerText.setPosition(10, 40);

    // Boutons
    boutonPause.setFont(font);
    boutonPause.setCharacterSize(20);
    boutonPause.setFillColor(sf::Color::White);
    boutonPause.setPosition(boutonX, boutonY + 40);
    boutonPause.setString("Pause");

        // Bouton "Démarrer"
    boutonDemarrer.setFont(font);
    boutonDemarrer.setCharacterSize(20);
    boutonDemarrer.setFillColor(sf::Color::White);
    boutonDemarrer.setPosition(boutonX, boutonY);
    boutonDemarrer.setString("Start");

    // Message de fin
    messageFin.setFont(font);
    messageFin.setCharacterSize(20);
    messageFin.setFillColor(sf::Color::Red);
    messageFin.setPosition(10, 160);
    messageFin.setString("");

    boutonQuitter.setFont(font);
    boutonQuitter.setCharacterSize(20);
    boutonQuitter.setFillColor(sf::Color::White);
    boutonQuitter.setPosition(boutonX, boutonY + 80);
    boutonQuitter.setString("Quitter");
}

void ModeGraphique::afficher() {
    window.clear();

    for (int x = 0; x < grille->getHauteur(); ++x) {
        for (int y = 0; y < grille->getLargeur(); ++y) {
            Entite* entite = grille->getEntite(x, y);

            if (entite == nullptr) {
                cellShape.setFillColor(sf::Color::Black); // Case vide
            } else if (dynamic_cast<Cellules*>(entite)) {
                cellShape.setFillColor(entite->estVivante() ? sf::Color::Green : sf::Color::Red); // Vert = vivant, Rouge = mort
            } else if (dynamic_cast<Obstacle*>(entite)) {
                cellShape.setFillColor(entite->estVivante() ? sf::Color::Blue : sf::Color::Yellow); // Bleu = obstacle vivant, Jaune = obstacle mort
            }

            cellShape.setPosition(y * 10.f, x * 10.f);
            window.draw(cellShape);
        }
    }

    afficherTexte(); // Afficher les textes et boutons
    window.display();
}

void ModeGraphique::simuler() {
    int iteration = 0;

    while (window.isOpen()) {
        gererEvenements();

        if (etatSimulation == EnCours && !enPause) {
            auto debut = std::chrono::high_resolution_clock::now();
            grille->MettreAJour();
            auto fin = std::chrono::high_resolution_clock::now();
            auto duree = std::chrono::duration_cast<std::chrono::milliseconds>(fin - debut).count();

            ++iteration;
            iterationText.setString("Iteration : " + std::to_string(iteration));
            timerText.setString("Temps : " + std::to_string(duree) + " ms");

            afficher();
        } else if (etatSimulation == Attente) {
            afficher(); // Affiche la fenêtre avec le bouton Démarrer
        } else if (etatSimulation == Terminee) {
            afficher(); // Continue d'afficher la fenêtre avec le message de fin
        }

        sf::sleep(sf::milliseconds(100));
    }
}


void ModeGraphique::gererEvenements() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        } else if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2f souris(event.mouseButton.x, event.mouseButton.y);

            if (boutonDemarrer.getGlobalBounds().contains(souris) && etatSimulation == Attente) {
                etatSimulation = EnCours;
            } else if (boutonPause.getGlobalBounds().contains(souris) && etatSimulation == EnCours) {
                enPause = !enPause;
                boutonPause.setString(enPause ? "Reprendre" : "Pause");
            } else if (boutonQuitter.getGlobalBounds().contains(souris)) {
                window.close();
            }
        }
    }
}


void ModeGraphique::afficherTexte() {
    window.draw(iterationText);
    window.draw(timerText);
    window.draw(boutonPause);
    window.draw(boutonQuitter);
    window.draw(boutonDemarrer);

    if (etatSimulation == Terminee) {
        window.draw(messageFin);
    }
}

