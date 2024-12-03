#include "../include/grille.h"
#include "../include/cellules.h"
#include <iostream>



// Constructeur (grille vide)
Grille::Grille(int hauteur, int largeur) : hauteur(hauteur), largeur(largeur) {
    grille.resize(hauteur, std::vector<Entite*>(largeur, nullptr));
};

// Destructeur
Grille::~Grille() {
    for (auto& ligne : grille) {
        for (auto& entite : ligne) {
            delete entite;
        }   
    }
}

// Ajoute une entité à la position (x, y)
void Grille::ajoutEntite(int x, int y, Entite* entite) {
    if (x >= 0 && x < hauteur && y >= 0 && y < largeur) {
        delete grille[x][y];
        grille[x][y] = entite;
    }
}

// Supprime l'entité à la position (x, y)
void Grille::supprimerEntite(int x, int y) {
    if (x >= 0 && x < hauteur && y >= 0 && y < largeur) {
        delete grille[x][y];
        grille[x][y] = nullptr;
    }
}

//Getter entite
Entite* Grille::getEntite(int x, int y) const {
    if (x >= 0 && x < hauteur && y >= 0 && y < largeur) {
        return grille[x][y];
    }
    return nullptr;
}

// Calcul le nombre de voisins de l'entité à la position (x, y)
int Grille::calculerVoisins(int x, int y) const {
    int nbVoisins = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) {
                continue;
            }
            if (x + i >= 0 && x + i < hauteur && y + j >= 0 && y + j < largeur) {
                if (grille[x + i][y + j] != nullptr) {
                    nbVoisins++;
                }
            }
        }
    }
    return nbVoisins;
}



void Grille::MettreAJour() {
    // Matrice temporaire pour stocker les prochains états
    std::vector<std::vector<bool>> prochainsEtats(hauteur, std::vector<bool>(largeur, false));

    // Calculer les prochains états
    for (int x = 0; x < hauteur; ++x) {
        for (int y = 0; y < largeur; ++y) {
            int voisins = calculerVoisins(x, y);
            if (grille[x][y] && grille[x][y]->estVivante()) {
                // Une cellule vivante possédant deux ou trois voisines vivantes reste vivante, sinon elle meurt
                prochainsEtats[x][y] = (voisins == 2 || voisins == 3);
            } else {
                // Une cellule morte possédant exactement trois voisines vivantes devient vivante
                prochainsEtats[x][y] = (voisins == 3);
            }
        }
    }

    // Appliquer les changements d'état et ajouter/supprimer des entités
    for (int x = 0; x < hauteur; ++x) {
        for (int y = 0; y < largeur; ++y) {
            if (prochainsEtats[x][y]) {
                // Si une cellule doit apparaître ou rester vivante
                if (!grille[x][y]) {  // Si aucune entité n'existe à cette position, on en crée une
                    grille[x][y] = new Cellules(true);  // Créer une nouvelle cellule vivante
                } else {
                    grille[x][y]->changerEtat(true); // Mettre à jour l'état de la cellule existante
                }
            } else {
                // Si la cellule doit mourir ou rester morte
                if (grille[x][y] && grille[x][y]->estVivante()) {  // Si une cellule vivante existe
                    grille[x][y]->changerEtat(false);
                    supprimerEntite(x, y);  // La cellule devient morte
                }
            }
        }
    }
}
