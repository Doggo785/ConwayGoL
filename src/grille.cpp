#include "../include/grille.h"
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

// Met à jour la grille [A TESTER]
void Grille::MettreAJour() {
    std::vector<std::vector<int>> prochainsEtats(hauteur, std::vector<int>(largeur, 0));

    // Calculer les prochains états
    for (int x = 0; x < hauteur; ++x) {
        for (int y = 0; y < largeur; ++y) {
            if (grille[x][y]) {
                int voisins = calculerVoisins(x, y);
                grille[x][y]->DeterminerEtatSuivant(voisins);
            }
        }
    }
}