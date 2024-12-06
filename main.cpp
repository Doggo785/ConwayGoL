#include "include/afficheurConsole.hpp"
#include "include/modeGraphique.hpp"
#include "include/modeConsole.hpp"
#include "include/cellules.hpp"
#include "include/obstacle.hpp"
#include "include/grille.hpp"
#include <iostream>

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

int main()
{
    // Demarrage du jeu
    std::cout << "=== ConwayGoL ===" << std::endl;
    std::cout << "par : Stephane PLathey | Marine Marine" << std::endl;

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
        return 1;
    }

    // Saut de ligne
    std::cout << std::endl;

    // Demande à l'utilisateur de choisir la grille
    int choixgrille;
    std::cout << "Choix de la grille :" << std::endl;
    std::cout << "1. Fichier pré-rempli" << std::endl;
    std::cout << "2. Grille random" << std::endl;
    std::cout << "3. Grille vide -> Possible qu'en mode Graphique" << std::endl;

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

        // Demande à l'utilisateur de saisir la taille de la grille
        int largeur, hauteur;
        std::cout << "Entrez la largeur de la grille : ";
        std::cin >> largeur;
        std::cout << "Entrez la hauteur de la grille : ";
        std::cin >> hauteur;

        Grille grille(largeur, hauteur);

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
        std::cout << "-> Grille vide choisie." << std::endl;

        if (choixmode == 1)
        {
            std::cerr << "ERROR : Impossible de choisir une grille vide en mode Console." << std::endl;
            return 1;
        }
        else if (choixmode == 2)
        {
            // Demande à l'utilisateur de saisir la taille de la grille
            int largeur, hauteur;
            std::cout << "Entrez la largeur de la grille : ";
            std::cin >> largeur;
            std::cout << "Entrez la hauteur de la grille : ";
            std::cin >> hauteur;

            Grille grille(largeur, hauteur);

            ModeGraphique mode(&grille);
            mode.simuler();
        }
        else
        {
            std::cerr << "ERROR : Erreur de grille" << std::endl;
            return 1;
        }
    }
    else
    {
        std::cerr << "-> Choix invalide." << std::endl;
        return 1;
    }

    return 0;
}
