#ifndef GRILLE_H
#define GRILLE_H

#include "entite.hpp"
#include <vector>
#include <string>

class Grille
{
private:
    int hauteur;
    int largeur;
    std::vector<std::vector<Entite *>> grille;

public:
    Grille(int hauteur, int largeur);
    ~Grille();

    // Capture l'état actuel de la grille pour verification stable
    std::vector<std::vector<bool>> capturerEtat() const;
    bool etatIdentique(const std::vector<std::vector<bool>> &etat1, const std::vector<std::vector<bool>> &etat2) const;

    // Getters et Setters
    int getHauteur() const { return hauteur; }
    int getLargeur() const { return largeur; }
    void ajoutEntite(int x, int y, Entite *entite);
    void supprimerEntite(int x, int y);
    Entite *getEntite(int x, int y) const;

    // Méthodes
    bool chargerFichier(const std::string &nomFichier);
    int calculerVoisins(int x, int y) const;
    void MettreAJour();
    int compterCellulesVivantes() const;
    void placerPattern(const std::string &nomPattern, int x, int y);
    void vider();
    void grillerandom();
};

#endif // GRILLE_H
