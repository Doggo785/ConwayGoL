#include "../include/afficheurConsole.hpp"
#include "../include/cellules.hpp"
#include "../include/obstacle.hpp"
#include <iostream>

// AfficheurConsole::AfficheurConsole() {}

void AfficheurConsole::Afficher(const Grille& grille) {
   for (int x = 0; x < grille.getHauteur(); ++x) {
       for (int y = 0; y < grille.getLargeur(); ++y) {
           Entite* entite = grille.getEntite(x, y);
           if (entite == nullptr) {
               std::cout << " . "; // Case vide
           } else if (dynamic_cast<Cellules*>(entite)) {
               std::cout << (entite->estVivante() ? " O " : " X "); // Cellule vivante ou morte
           } else if (dynamic_cast<Obstacle*>(entite) && entite->estVivante()) {
               std::cout << " # "; // Obstacle
           } else if (dynamic_cast<Obstacle*>(entite) && !entite->estVivante()) {
               std::cout << " ☐ "; // Case vide
           }
       }
       std::cout << std::endl;
   }
}
