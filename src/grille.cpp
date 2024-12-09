#include "../include/cellules.hpp"
#include "../include/obstacle.hpp"
#include "../include/grille.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>

// Constructeur (grille vide)
Grille::Grille(int hauteur, int largeur) : hauteur(hauteur), largeur(largeur)
{
    grille.resize(hauteur, std::vector<Entite *>(largeur, nullptr));
};

// Destructeur
Grille::~Grille()
{
    for (auto &ligne : grille)
    {
        for (auto &entite : ligne)
        {
            delete entite;
        }
    }
}

// Ajoute une entité à la position (x, y)
void Grille::ajoutEntite(int x, int y, Entite *entite)
{
    if (x >= 0 && x < hauteur && y >= 0 && y < largeur)
    {
        delete grille[x][y];
        grille[x][y] = entite;
    }
}

// Supprime l'entité à la position (x, y)
void Grille::supprimerEntite(int x, int y)
{
    if (x >= 0 && x < hauteur && y >= 0 && y < largeur)
    {
        delete grille[x][y];
        grille[x][y] = nullptr;
    }
}

// Getter entite
Entite *Grille::getEntite(int x, int y) const
{
    if (x >= 0 && x < hauteur && y >= 0 && y < largeur)
    {
        return grille[x][y];
    }
    return nullptr;
}

// Calculer le nombre de voisins vivants d'une cellule à la position (x, y) grille torique
int Grille::calculerVoisins(int x, int y) const
{
    int voisinsVivants = 0;

    for (int dx = -1; dx <= 1; ++dx)
    {
        for (int dy = -1; dy <= 1; ++dy)
        {
            if (dx == 0 && dy == 0)
                continue; // Ignorer la cellule elle-même

            // Calculer les coordonnées des voisins dans une grille torique
            int nx = (x + dx + hauteur) % hauteur; // Wrap-around en hauteur
            int ny = (y + dy + largeur) % largeur; // Wrap-around en largeur

            Entite *voisin = grille[nx][ny];
            if (voisin && voisin->estVivante())
            {
                ++voisinsVivants;
            }
        }
    }

    return voisinsVivants;
}

void Grille::MettreAJour()
{
    // Matrice temporaire pour stocker les prochains états
    std::vector<std::vector<bool>> prochainsEtats(hauteur, std::vector<bool>(largeur, false));

    // Nombre de threads à utiliser
    const int numThreads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;

    // Fonction lambda pour calculer les prochains états sur une section de la grille
    auto calculerSection = [&](int debut, int fin)
    {
        for (int x = debut; x < fin; ++x)
        {
            for (int y = 0; y < largeur; ++y)
            {
                int voisins = calculerVoisins(x, y);
                if (grille[x][y] && grille[x][y]->estVivante())
                {
                    // Une cellule vivante possédant deux ou trois voisines vivantes reste vivante
                    prochainsEtats[x][y] = (voisins == 2 || voisins == 3);
                }
                else
                {
                    // Une cellule morte possédant exactement trois voisines vivantes devient vivante
                    prochainsEtats[x][y] = (voisins == 3);
                }
            }
        }
    };

    // Diviser la grille en sections et assigner une section à chaque thread
    int sectionTaille = hauteur / numThreads;
    for (int i = 0; i < numThreads; ++i)
    {
        int debut = i * sectionTaille;
        int fin = (i == numThreads - 1) ? hauteur : debut + sectionTaille;
        threads.emplace_back(calculerSection, debut, fin);
    }

    // Attendre la fin de tous les threads
    for (auto &t : threads)
    {
        t.join();
    }

    // Appliquer les changements d'état et ajouter/supprimer des entités
    for (int x = 0; x < hauteur; ++x)
    {
        for (int y = 0; y < largeur; ++y)
        {
            // Vérifications pour les obstacles
            if (dynamic_cast<Obstacle *>(grille[x][y]))
            {
                // Si la cellule est un obstacle vivant, elle ne peut pas changer d'état
                continue;
            }
            if (prochainsEtats[x][y])
            {
                // Si une cellule doit apparaître ou rester vivante
                if (!grille[x][y])
                {                                      // Si aucune entité n'existe à cette position, on en crée une
                    grille[x][y] = new Cellules(true); // Créer une nouvelle cellule vivante
                }
                else
                {
                    grille[x][y]->changerEtat(true); // Mettre à jour l'état de la cellule existante
                }
            }
            else
            {
                // Si la cellule doit mourir ou rester morte
                if (grille[x][y] && grille[x][y]->estVivante())
                { // Si une cellule vivante existe
                    grille[x][y]->changerEtat(false);
                    supprimerEntite(x, y); // La cellule devient morte
                }
            }
        }
    }
}

bool Grille::chargerFichier(const std::string &nomFichier)
{
    std::ifstream fichier(nomFichier);
    if (!fichier)
    {
        std::cerr << "Erreur lors de l'ouverture du fichier : " << nomFichier << std::endl;
        return false; // Le fichier n'a pas pu être ouvert
    }

    // Lire la première ligne pour obtenir la hauteur et la largeur
    fichier >> hauteur >> largeur;

    // Redimensionner la grille en fonction de la taille du fichier
    grille.resize(hauteur, std::vector<Entite *>(largeur, nullptr));

    // Lire chaque ligne et initialiser la grille
    for (int x = 0; x < hauteur; ++x)
    {
        for (int y = 0; y < largeur; ++y)
        {
            int valeur;
            fichier >> valeur;

            // Créer des entités en fonction de la valeur
            if (valeur == 1)
            {
                // Cellule vivante
                ajoutEntite(x, y, new Cellules(true));
            }
            else if (valeur == 2)
            {
                // Obsacle mort
                ajoutEntite(x, y, new Obstacle(false)); // Cellule vide
            }
            else if (valeur == 3)
            {
                // Obstacle vivant
                ajoutEntite(x, y, new Obstacle(true));
            }
            else
            {
                // Cellule morte ou vide
                ajoutEntite(x, y, nullptr);
            }
        }
    }

    return true;
}

std::vector<std::vector<bool>> Grille::capturerEtat() const
{
    std::vector<std::vector<bool>> etat(hauteur, std::vector<bool>(largeur, false));

    for (int x = 0; x < hauteur; ++x)
    {
        for (int y = 0; y < largeur; ++y)
        {
            Entite *entite = grille[x][y];
            if (entite && entite->estVivante())
            {
                etat[x][y] = true;
            }
        }
    }

    return etat;
}

bool Grille::etatIdentique(const std::vector<std::vector<bool>> &etat1, const std::vector<std::vector<bool>> &etat2) const
{
    return etat1 == etat2;
}

// Nouvelle fonction pour compter les cellules vivantes
int Grille::compterCellulesVivantes() const
{
    int compteur = 0;
    for (int x = 0; x < hauteur; ++x)
    {
        for (int y = 0; y < largeur; ++y)
        {
            if (grille[x][y] && grille[x][y]->estVivante())
            {
                compteur++;
            }
        }
    }
    return compteur;
}

// Nouvelle fonction pour placer des motifs prédéfinis sur la grille
void Grille::placerPattern(const std::string &nomPattern, int x, int y)
{
    // Exemple de motifs prédéfinis
    std::vector<std::vector<int>> pattern;

    if (nomPattern == "Glider")
    {
        pattern = {
            {0, 1, 0},
            {0, 0, 1},
            {1, 1, 1}};
    }
    else if (nomPattern == "Blinker")
    {
        pattern = {
            {1, 1, 1}};
    }
    else if (nomPattern == "Toad")
    {
        pattern = {
            {0, 1, 1, 1},
            {1, 1, 1, 0}};
    }
    else if (nomPattern == "LWSS")
    {
        pattern = {
            {1, 0, 0, 1, 0},
            {0, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {0, 1, 1, 1, 1},
        };
    } else if (nomPattern == "Pulsar")
    {
        pattern = {
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1},
    {0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0},
    {0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0},
    {0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0},
    {1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0}
        };
    } else if (nomPattern == "Canon")
    {
        pattern = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
    {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
                    };
    } else if (nomPattern == "Cell alive") {
        pattern = {
    {1}
        };
    }else if (nomPattern == "Cell dead") {
        pattern = {
    {0}
        };
    }else if (nomPattern == "Obstacle alive") {
        pattern = {
    {3}
        };
    }else if (nomPattern == "Obsacle dead") {
        pattern = {
    {2}
        };
    } else {
        std::cerr << "Motif inconnu : " << nomPattern << std::endl;
        return;
    }

    // Placer le motif sur la grille
    for (int i = 0; i < pattern.size(); ++i)
    {
        for (int j = 0; j < pattern[i].size(); ++j)
        {
            if (pattern[i][j] == 1)
            {
                ajoutEntite(x + i, y + j, new Cellules(true));
            }
        }
    }
}

// Nouvelle fonction pour vider la grille
void Grille::vider()
{
    for (int x = 0; x < hauteur; ++x)
    {
        for (int y = 0; y < largeur; ++y)
        {
            supprimerEntite(x, y);
        }
    }
}
