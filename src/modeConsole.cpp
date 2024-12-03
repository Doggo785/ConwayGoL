#include "../include/modeConsole.h"
#include <iostream>

ModeConsole::ModeConsole(Grille* grille, int iterationMax)
    : ModeSimulation(grille, iterationMax) {}

void ModeConsole::simuler() {
    for (int i = 0; i < iterationMax; ++i) {
        std::cout << "=== Iteration " << i + 1 << " / " << iterationMax << " ===" << std::endl;

        // Afficher la grille
        afficheurConsole.Afficher(*grille);        

        // Mettre à jour la grille
        grille->MettreAJour();
        
        // Pause ou interaction utilisateur (optionnel)
        std::cout << "Appuyez sur Entrée pour continuer..." << std::endl;
        std::cin.ignore();
    }
}