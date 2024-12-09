#include "include/afficheurConsole.hpp"
#include "include/modeGraphique.hpp"
#include "include/modeConsole.hpp"
#include "include/cellules.hpp"
#include "include/obstacle.hpp"
#include "include/grille.hpp"
#include <iostream>

void choisirModeJeu(int& choixmode);
void choisirGrille(int choixmode);
void demarrerModeConsole(const std::string& grille);
void demarrerModeGraphique(const std::string& grille);
void demarrerGrilleRandom(int choixmode);
void demarrerGrilleVide(int choixmode);

std::pair<int, int> tailleGrille(int largeur, int hauteur)
{
    while (largeur < 2 || hauteur < 2) {
        if (largeur < 2) {
            std::cout << "Entrez la largeur de la grille : ";
            std::cin >> largeur;
        }
        if (hauteur < 2) {
            std::cout << "Entrez la hauteur de la grille : ";
            std::cin >> hauteur;
        }
        if (largeur < 2 || hauteur < 2) {
            std::cerr << "ERROR : La hauteur et la largeur doivent être supérieur à 2." << std::endl;
        }
    }
    return {largeur, hauteur};
}

// Fonction pour démarrer le mode console avec un fichier
void StartModeConsole(std::string nomFichier)
{
    int iterationMax;
    std::cout << "=== Mode Console ===" << std::endl;
    std::cout << "Choisir un nombre d'iteration :" << std::endl;
    std::cin >> iterationMax;

    Grille grille(0, 0);

    // Charger le fichier et initialiser la grille
    if (grille.chargerFichier(nomFichier))
    {
        std::cout << "Grille initialisée avec succès depuis le fichier !" << std::endl;
    }
    else
    {
        std::cerr << "ERROR : Erreur lors de l'initialisation de la grille depuis le fichier." << std::endl;
    }

    ModeConsole mode(&grille, iterationMax, nomFichier);
    mode.simuler();
}

// Fonction pour démarrer le mode graphique avec un fichier
void StartModeGraphique(std::string nomFichier)
{
    std::cout << "=== Mode Graphique ===" << std::endl;

    Grille grille(0, 0);

    // Charger le fichier et initialiser la grille
    if (grille.chargerFichier(nomFichier))
    {
        std::cout << "Grille initialisée avec succès depuis le fichier !" << std::endl;
    }
    else
    {
        std::cerr << "ERROR : Erreur lors de l'initialisation de la grille depuis le fichier." << std::endl;
    }

    ModeGraphique mode(&grille);
    mode.simuler();
}

int ChoixMode()
{
        // Demarrage du jeu
    std::cout << "=== Conway's Game Of Life ===" << std::endl;
    std::cout << "par : Stephane Plathey | Marine Mazou" << std::endl;

    // Saut de ligne
    std::cout << std::endl;

    // Demande à l'utilisateur de choisir le mode de jeu
    int choixmode;
    std::cout << "Choix du mode de jeu :" << std::endl;
    std::cout << "1. Mode Console" << std::endl;
    std::cout << "2. Mode Graphique" << std::endl;

    std::cin >> choixmode;

    if (choixmode == 1)
    {
        std::cout << "-> Mode Console choisi." << std::endl;
    }
    else if (choixmode == 2)
    {
        std::cout << "-> Mode Graphique choisi." << std::endl;
    }
    else
    {
        std::cerr << "-> Choix invalide." << std::endl;
        return ChoixMode();
    }
    return choixmode;
}

int ChoixGrille(int choixmode)
{
    // Demande à l'utilisateur de choisir la grille
    int choixgrille;
    std::cout << "Choix de la grille :" << std::endl;
    std::cout << "1. Fichier pré-rempli" << std::endl;
    std::cout << "2. Grille random" << std::endl;
    if (choixmode == 2) {
        std::cout << "3. Grille vide -> Possible qu'en mode Graphique" << std::endl;
    }
    std::cin >> choixgrille;

    if (choixgrille == 1)
    {
        std::cout << "-> Grille pré-remplie choisie." << std::endl;

        // Saut de ligne
        std::cout << std::endl;

        // Demande à l'utilisateur de saisir le nom du fichier
        std::string nomFichier;
        std::cout << "Entrez le nom du fichier pour la grille : ";
        std::cin >> nomFichier;

        std::string grille = "Grilles/" + nomFichier;

        if (choixmode == 1)
        {
            // Demande à l'utilisateur de saisir le nombre d'itérations
            StartModeConsole(grille);
        }
        else if (choixmode == 2)
        {
            StartModeGraphique(grille);
        }
        else
        {
            std::cout << "ERROR : Erreur de grille" << std::endl;
        }

        // std::string nomFichier = "config";
    }
    else if (choixgrille == 2)
    {
        std::cout << "-> Grille random choisie." << std::endl;

        int largeur = 0, hauteur = 0;
        std::tie(largeur, hauteur) = tailleGrille(largeur, hauteur);

        Grille grille(largeur, hauteur);
        std::srand(std::time(0));
        for (int x = 0; x < grille.getHauteur(); ++x) {
            for (int y = 0; y < grille.getLargeur(); ++y) {
                int randomValue = std::rand() % 2; // Générer un nombre aléatoire entre 0 et 1
                if (randomValue == 0) {
                    grille.ajoutEntite(x, y, new Cellules(true)); // Cellule vivante
                } else {
                    grille.supprimerEntite(x, y); // Cellule morte (case vide)
                }
            }
        }

        if (choixmode == 1)
        {
            // Demande à l'utilisateur de saisir le nombre d'itérations
            int iterationMax;
            std::cout << "Entrez le nombre d'itérations : ";
            std::cin >> iterationMax;
            ModeConsole mode(&grille, iterationMax, "");
            mode.simuler();
        }
        else if (choixmode == 2)
        {
            ModeGraphique mode(&grille);
            mode.simuler();
        }
        else
        {
            std::cerr << "ERROR : Erreur de grille" << std::endl;
            return 1;
        }
    }
    else if (choixgrille == 3)
    {        
        if (choixmode == 1)
        {
            std::cerr << "ERROR : Impossible de choisir une grille vide en mode Console." << std::endl;
            return 1;
        }
        else if (choixmode == 2)
        {
            int largeur = 0, hauteur = 0;
            std::tie(largeur, hauteur) = tailleGrille(largeur, hauteur);

            Grille grille(largeur, hauteur);

            ModeGraphique mode(&grille);
            mode.simuler();
        } else
        {
            std::cerr << "-> Choix invalide." << std::endl;
            return ChoixGrille(choixmode);
        }
    }
    return choixgrille;
}



void demarrerGrilleRandom(int choixmode) {
    int largeur = 0, hauteur = 0;
    std::tie(largeur, hauteur) = tailleGrille(largeur, hauteur);
    Grille grille(largeur, hauteur);
    std::srand(std::time(0));
    for (int x = 0; x < grille.getHauteur(); ++x) {
        for (int y = 0; y < grille.getLargeur(); ++y) {
            int randomValue = std::rand() % 2;
            if (randomValue == 0) {
                grille.ajoutEntite(x, y, new Cellules(true));
            } else {
                grille.supprimerEntite(x, y);
            }
        }
    }

    if (choixmode == 1) {
        int iterationMax;
        std::cout << "Entrez le nombre d'itérations : ";
        std::cin >> iterationMax;
        ModeConsole mode(&grille, iterationMax, "");
        mode.simuler();
    } else if (choixmode == 2) {
        ModeGraphique mode(&grille);
        mode.simuler();
    }
}

void demarrerGrilleVide(int choixmode) {
    int largeur = 0, hauteur = 0;
    std::tie(largeur, hauteur) = tailleGrille(largeur, hauteur);
    Grille grille(largeur, hauteur);
    ModeGraphique mode(&grille);
    mode.simuler();
}

int main()
{
    int choixmode = ChoixMode();
    // Saut de ligne
    std::cout << std::endl;
    ChoixGrille(choixmode);

}
