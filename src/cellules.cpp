#include "../include/cellules.hpp"

Cellules::Cellules(bool etatInitial) : Entite(etatInitial) {}

bool Cellules::DeterminerEtatSuivant(int voisinsVivants) {
    if (estVivante()) {
        if (voisinsVivants < 2 || voisinsVivants > 3) {
            changerEtat(false);
            return false;
        }
    } else {
        if (voisinsVivants == 3) {
            changerEtat(true);
        }
    }
    return true;
}