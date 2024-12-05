#include "modeGraphique.hpp"
#include <iostream>
#include <SFML/Window/Event.hpp>

ModeGraphique::ModeGraphique(Grille* grille)
    : ModeSimulation(grille, iterationMax),
      window(sf::VideoMode(1280, 720), "Jeu de la Vie"),
      afficheur(&window),
      etatCourant(Menu),
      iterations(0),
      vitesse(1),
      simulationLancee(false),
      quitterSimulation(false) {
    afficheur.chargerRessources();
}

ModeGraphique::~ModeGraphique() {
    quitterSimulation = true;
    if (simulationThread.joinable()) {
        simulationThread.join();
    }
}

void ModeGraphique::simuler() {
    bool quitter = false;

    while (window.isOpen() && !quitter) {
        if (etatCourant == Menu) {
            gererEvenementsMenu(quitter);
            afficheur.afficherMenu();
        } else if (etatCourant == Editeur) {
            gererEvenementsEditeur(quitter);
            afficheur.afficherEditeur(*grille);
        } else if (etatCourant == Simulation) {
            gererEvenementsSimulation(quitter);
            afficheur.afficherSimulation(*grille, iterations, grille->compterCellulesVivantes(), vitesse);
        }
    }
}

void ModeGraphique::gererEvenementsMenu(bool& quitter) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            quitter = true;
        } else if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2f souris(event.mouseButton.x, event.mouseButton.y);

            // Vérifier les clics sur les boutons
            if (afficheur.boutonStart.getGlobalBounds().contains(souris)) {
                etatCourant = Simulation; // Passer au mode simulation
                simulationLancee = true;
                quitterSimulation = false;
                simulationThread = std::thread(&ModeGraphique::boucleSimulation, this);
            } else if (afficheur.boutonQuit.getGlobalBounds().contains(souris)) {
                quitter = true;
            }
        }
    }
}

void ModeGraphique::gererEvenementsEditeur(bool& quitter) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            quitter = true;
        } else if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2f souris(event.mouseButton.x, event.mouseButton.y);

            // Vérifier les clics sur les boutons
            if (afficheur.boutonStart.getGlobalBounds().contains(souris)) {
                etatCourant = Simulation; // Lancer la simulation
                simulationLancee = true;
                quitterSimulation = false;
                simulationThread = std::thread(&ModeGraphique::boucleSimulation, this);
            } else if (afficheur.boutonQuit.getGlobalBounds().contains(souris)) {
                quitter = true;
            }
        } else if (event.type == sf::Event::KeyPressed) {
            // Gérer les entrées clavier pour placer des patterns
            if (event.key.code == sf::Keyboard::P) {
                grille->placerPattern("Glider", 10, 10); // Exemple : placer un "Glider" au centre
            } else if (event.key.code == sf::Keyboard::C) {
                grille->vider(); // Nettoyer la grille
            }
        }
    }
}

void ModeGraphique::gererEvenementsSimulation(bool& quitter) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            quitter = true;
            quitterSimulation = true;
        } else if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2f souris(event.mouseButton.x, event.mouseButton.y);

            // Vérifier les clics sur les boutons de contrôle de vitesse
            if (afficheur.boutonVitessePlus.getGlobalBounds().contains(souris)) {
                vitesse = std::min(vitesse + 1, 20); // Limite supérieure de la vitesse
            } else if (afficheur.boutonVitesseMoins.getGlobalBounds().contains(souris)) {
                vitesse = std::max(vitesse - 1, 1); // Limite inférieure de la vitesse
            } else if (afficheur.playPauseButton.getGlobalBounds().contains(souris)) {
                simulationLancee = !simulationLancee; // Alterner entre pause et reprise
                if (simulationLancee) {
                    afficheur.playPauseButton.setTexture(afficheur.pauseTexture); // Changer l'icône en "Pause"
                } else {
                    afficheur.playPauseButton.setTexture(afficheur.playTexture); // Changer l'icône en "Play"
                }
            } else if (afficheur.boutonQuitterSimulation.getGlobalBounds().contains(souris)) {
                quitter = true; // Quitter la simulation
                quitterSimulation = true;
            }
        }
    }
}

void ModeGraphique::boucleSimulation() {
    while (!quitterSimulation) {
        if (simulationLancee) {
            // Mise à jour de la grille à chaque itération
            sf::sleep(sf::milliseconds(1000 / vitesse)); // Contrôle de la vitesse
            grille->MettreAJour();
            iterations++;
        }
    }
}