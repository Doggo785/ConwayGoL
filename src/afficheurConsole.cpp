#include "../include/afficheurConsole.h"
#include "../include/cellules.h"
#include "../include/obstacle.h"
#include <iostream>

// AfficheurConsole::AfficheurConsole() {}

void AfficheurConsole::Afficher(const Grille& grille) const {
   for (int x = 0; x < grille.getHauteur(); ++x) {
       for (int y = 0; y < grille.getLargeur(); ++y) {
           Entite* entite = grille.getEntite(x, y);
           if (entite == nullptr) {
               std::cout << " . "; // Case vide
           } else if (dynamic_cast<Cellules*>(entite)) {
               std::cout << (entite->estVivante() ? " O " : " X "); // Cellule vivante ou morte
           } else if (dynamic_cast<Obstacle*>(entite)) {
               std::cout << " # "; // Obstacle
           }
       }
       std::cout << std::endl;
   }
}
