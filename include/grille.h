#ifndef GRILLE_H
#define GRILLE_H

#include <entite.h>
#include <vector>


class Grille {
private:
    int hauteur;
    int largeur;
    std::vector<std::vector<Entite*>> grille;
public:
    Grille(int hauteur, int largeur);
    ~Grille();	

    // Getters et Setters
    void ajoutEntite(int x, int y, Entite* entite);
    void supprimerEntite(int x, int y);
    Entite* getEntite(int x, int y) const;

    // Méthodes
    int calculerVoisins(int x, int y) const;
    void MettreAJour();
};

#endif // GRILLE_H
