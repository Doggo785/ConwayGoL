#include "../include/grille.h"
#include "../include/cellules.h"
#include "../include/obstacle.h"
#include <fstream>
#include <iostream>
#include <sstream>




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

// Calculer le nombre de voisins vivants d'une cellule à la position (x, y) grille torique
int Grille::calculerVoisins(int x, int y) const {
    int voisinsVivants = 0;

    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue; // Ignorer la cellule elle-même

            // Calculer les coordonnées des voisins dans une grille torique
            int nx = (x + dx + hauteur) % hauteur; // Wrap-around en hauteur
            int ny = (y + dy + largeur) % largeur; // Wrap-around en largeur

            Entite* voisin = grille[nx][ny];
            if (voisin && voisin->estVivante()) {
                ++voisinsVivants;
            }
        }
    }

    return voisinsVivants;
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
            // Verifications obstacles
            if (dynamic_cast<Obstacle*>(grille[x][y])) {
                // Si la cellule est un obstacle vivant, elle ne peut pas changer d'état
                continue;
            }
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

bool Grille::chargerFichier(const std::string& nomFichier) {
    std::ifstream fichier(nomFichier);
    if (!fichier) {
        std::cerr << "Erreur lors de l'ouverture du fichier : " << nomFichier << std::endl;
        return false;  // Le fichier n'a pas pu être ouvert
    }

    // Lire la première ligne pour obtenir la hauteur et la largeur
    fichier >> hauteur >> largeur;

    // Redimensionner la grille en fonction de la taille du fichier
    grille.resize(hauteur, std::vector<Entite*>(largeur, nullptr));

    // Lire chaque ligne et initialiser la grille
    for (int x = 0; x < hauteur; ++x) {
        for (int y = 0; y < largeur; ++y) {
            int valeur;
            fichier >> valeur;

            // Créer des entités en fonction de la valeur
            if (valeur == 1) {
                // Cellule vivante
                ajoutEntite(x, y, new Cellules(true));
            } else if (valeur == 2) {
                // Obsacle mort
                ajoutEntite(x, y, new Obstacle(false)); // Cellule vide
            } else if (valeur == 3) {
                // Obstacle vivant
                ajoutEntite(x, y, new Obstacle(true));
            } else {
                // Cellule morte ou vide
                ajoutEntite(x, y, nullptr);
            }
        }
    }

    return true;
}

std::vector<std::vector<bool>> Grille::capturerEtat() const {
    std::vector<std::vector<bool>> etat(hauteur, std::vector<bool>(largeur, false));

    for (int x = 0; x < hauteur; ++x) {
        for (int y = 0; y < largeur; ++y) {
            Entite* entite = grille[x][y];
            if (entite && entite->estVivante()) {
                etat[x][y] = true;
            }
        }
    }

    return etat;
}

bool Grille::etatIdentique(const std::vector<std::vector<bool>>& etat1, const std::vector<std::vector<bool>>& etat2) const {
    return etat1 == etat2;
}