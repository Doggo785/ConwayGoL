#include "../include/modeGraphique.hpp"
#include "../include/afficheurGraphique.hpp"
#include "../include/cellules.hpp"
#include "../include/obstacle.hpp"
#include <iostream>
#include <SFML/Window/Event.hpp>
#include <iostream>


ModeGraphique::ModeGraphique(Grille* grille)
    : ModeSimulation(grille, 100),
      window(sf::VideoMode(1280, 720), "Jeu de la Vie", sf::Style::Titlebar | sf::Style::Close),
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
                etatCourant = Editeur; // Passer au mode édition
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
            if (afficheur.playPauseButton.getGlobalBounds().contains(souris)) {
                etatCourant = Simulation; // Passer au mode simulation
                simulationLancee = true;
                quitterSimulation = false;
                afficheur.playPauseButton.setTexture(afficheur.pauseTexture); // Changer l'icône en "Pause"
                if (simulationThread.joinable()) {
                    simulationThread.join();
                }
                simulationThread = std::thread(&ModeGraphique::boucleSimulation, this);
            } else if (afficheur.boutonQuit.getGlobalBounds().contains(souris)) {
                quitter = true;
            } else if (afficheur.boutonVider.getGlobalBounds().contains(souris)) {
                grille->vider(); // Vider la grille
            } else if (afficheur.boutonGlider.getGlobalBounds().contains(souris)) {
                afficheur.motifSelectionne = "Glider"; // Sélectionner le motif "Glider"
            } else if (afficheur.boutonBlinker.getGlobalBounds().contains(souris)) {
                afficheur.motifSelectionne = "Blinker"; // Sélectionner le motif "Blinker"
            } else if (afficheur.boutonToad.getGlobalBounds().contains(souris)) {
                afficheur.motifSelectionne = "Toad"; // Sélectionner le motif "Toad"
            } else if (afficheur.boutonLWSS.getGlobalBounds().contains(souris)) {
                afficheur.motifSelectionne = "LWSS"; // Sélectionner le motif "LWSS"
            } else if (afficheur.boutonPulsar.getGlobalBounds().contains(souris)) {
                afficheur.motifSelectionne = "Pulsar"; // Sélectionner le motif "Pulsar"
            } else if (afficheur.boutonCanon.getGlobalBounds().contains(souris)) {
                afficheur.motifSelectionne = "Canon"; // Sélectionner le motif "Canon"
            } else if (afficheur.boutonCellAlive.getGlobalBounds().contains(souris)) {
                afficheur.motifSelectionne = "Cellule vivante"; // Sélectionner le motif "Cellule vivante"
            } else if (afficheur.boutonCellDead.getGlobalBounds().contains(souris)) {
                afficheur.motifSelectionne = "Cellule morte"; // Sélectionner le motif "Cellule morte"
            } else if (afficheur.boutonObstacleAlive.getGlobalBounds().contains(souris)) {
                afficheur.motifSelectionne = "Obstacle vivant"; // Sélectionner le motif "Obstacle vivant"
            } else if (afficheur.boutonObstacleDead.getGlobalBounds().contains(souris)) {
                afficheur.motifSelectionne = "Obstacle mort"; // Sélectionner le motif "Obstacle mort"
            } else if (afficheur.boutonQuitterSimulation.getGlobalBounds().contains(souris)) {
                quitter = true; // Quitter la simulation
                quitterSimulation = true;
                if (simulationThread.joinable()) {
                    simulationThread.join();
                }
            }
        } else if (event.type == sf::Event::KeyPressed) {
            // Gérer les entrées clavier pour déplacer la position actuelle et placer des motifs
            if (event.key.code == sf::Keyboard::Up) {
                afficheur.positionActuelle.x = std::max(0, afficheur.positionActuelle.x - 1);
            } else if (event.key.code == sf::Keyboard::Down) {
                afficheur.positionActuelle.x = std::min(grille->getHauteur() - 1, afficheur.positionActuelle.x + 1);
            } else if (event.key.code == sf::Keyboard::Left) {
                afficheur.positionActuelle.y = std::max(0, afficheur.positionActuelle.y - 1);
            } else if (event.key.code == sf::Keyboard::Right) {
                afficheur.positionActuelle.y = std::min(grille->getLargeur() - 1, afficheur.positionActuelle.y + 1);
            } else if (event.key.code == sf::Keyboard::Enter) {
                if (afficheur.motifSelectionne == "Cellule vivante") {
                    grille->ajoutEntite(afficheur.positionActuelle.x, afficheur.positionActuelle.y, new Cellules(true));
                } else if (afficheur.motifSelectionne == "Cellule morte") {
                    grille->supprimerEntite(afficheur.positionActuelle.x, afficheur.positionActuelle.y); 
                } else if (afficheur.motifSelectionne == "Obstacle vivant") {
                    grille->ajoutEntite(afficheur.positionActuelle.x, afficheur.positionActuelle.y, new Obstacle(true));
                } else if (afficheur.motifSelectionne == "Obstacle mort") {
                    grille->ajoutEntite(afficheur.positionActuelle.x, afficheur.positionActuelle.y, new Obstacle(false));
                } else {
                    grille->placerPattern(afficheur.motifSelectionne, afficheur.positionActuelle.x, afficheur.positionActuelle.y);
                }
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
            if (simulationThread.joinable()) {
                simulationThread.join();
            }
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
                    quitterSimulation = true;
                    if (simulationThread.joinable()) {
                        simulationThread.join();
                    }
                    etatCourant = Editeur; // Repasser en mode édition
                }
            } else if (afficheur.boutonQuitterSimulation.getGlobalBounds().contains(souris)) {
                quitter = true; // Quitter la simulation
                quitterSimulation = true;
                if (simulationThread.joinable()) {
                    simulationThread.join();
                }
            }
        }
    }
}

void ModeGraphique::boucleSimulation() {
    std::vector<std::vector<bool>> etatPrecedent = grille->capturerEtat();
    while (!quitterSimulation) {
        if (simulationLancee) {
            // Mise à jour de la grille à chaque itération
            sf::sleep(sf::milliseconds(1000 / vitesse)); // Contrôle de la vitesse
            grille->MettreAJour();
            iterations++;

            // Capturer l'état actuel de la grille
            std::vector<std::vector<bool>> etatActuel = grille->capturerEtat();

            // Vérifier la stabilité
            if (grille->etatIdentique(etatPrecedent, etatActuel)) {
                simulationLancee = false; // Mettre en pause la simulation
                afficheur.playPauseButton.setTexture(afficheur.playTexture); // Changer l'icône en "Play"
            }

            // Mettre à jour l'état précédent
            etatPrecedent = etatActuel;
        }
    }
}