#include "../include/modeConsole.h"
#include <iostream>
#include <fstream>

ModeConsole::ModeConsole(Grille* grille, int iterationMax, const std::string& nomFichier)
    : ModeSimulation(grille, iterationMax), nomFichier(nomFichier) {}


void ModeConsole::saveFichier(int iteration) const {
    std::ofstream fichier(nomFichier + "_out", std::ios::app); // Ouverture en mode ajout
    if (!fichier) {
        std::cerr << "Erreur : impossible d'ouvrir le fichier " << nomFichier + "_out" << std::endl;
        return;
    }

    fichier << "=== Iteration " << iteration << " ===" << std::endl;

    for (int x = 0; x < grille->getHauteur(); ++x) {
        for (int y = 0; y < grille->getLargeur(); ++y) {
            if (grille->getEntite(x, y) && grille->getEntite(x, y)->estVivante()) {
                fichier << "1";
            } else {
                fichier << "0";
            }
            if (y < grille->getLargeur() - 1) {
                fichier << " "; // Ajouter un espace entre les valeurs
            }
        }
        fichier << std::endl;
    }

    fichier << std::endl; // Ligne vide entre les itérations
    fichier.close();
}

void ModeConsole::simuler() {
    for (int i = 0; i < iterationMax; ++i) {
        std::cout << "=== Iteration " << i + 1 << " / " << iterationMax << " ===" << std::endl;

        // Afficher la grille
        afficheurConsole.Afficher(*grille);        

        // Sauvegarder la grille dans un fichier
        saveFichier(i + 1);

        // Mettre à jour la grille
        grille->MettreAJour();
        
        // Pause ou interaction utilisateur (optionnel)
        std::cout << "Appuyez sur Entrée pour continuer..." << std::endl;
        std::cin.ignore();
    }
}