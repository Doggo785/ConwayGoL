#include "../include/modeConsole.hpp"
#include <iostream>
#include <fstream>

ModeConsole::ModeConsole(Grille* grille, int iterationMax, const std::string& nomFichier)
    : ModeSimulation(grille, iterationMax), nomFichier(nomFichier.empty() ? "randomGrid" : nomFichier) {}

void ModeConsole::saveFichier(int iteration) const {
    std::ofstream fichier(nomFichier + "_out", std::ios::app); // Ouverture en mode ajout    

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
    std::vector<std::vector<bool>> etatPrecedent = grille->capturerEtat();
    for (int i = 0; i < iterationMax; ++i) {
        std::cout << "=== Iteration " << i + 1 << " / " << iterationMax << " ===" << std::endl;

        // Afficher la grille
        afficheurConsole.Afficher(*grille);        

        // Sauvegarder la grille dans un fichier
        saveFichier(i + 1);

        // Mettre à jour la grille
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

        // Pause ou interaction utilisateur (optionnel)
        std::cout << "Appuyez sur Entrée pour continuer..." << std::endl;
        std::cin.ignore();
    }
}