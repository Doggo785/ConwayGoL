#include "../include/cellules.h"

Cellules::Cellules(bool etatInitial) : Entite(etatInitial) {}

void Cellules::DeterminerEtatSuivant(int voisinsVivants) {
    if (estVivante()) {
        if (voisinsVivants < 2 || voisinsVivants > 3) {
            changerEtat(false);
        }
    } else {
        if (voisinsVivants == 3) {
            changerEtat(true);
        }
    }
}