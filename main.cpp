#include <iostream>
#include "include/grille.hpp"
#include "include/cellules.hpp"
#include "include/obstacle.hpp"
#include "include/afficheurConsole.hpp"
#include "include/modeConsole.hpp"
#include "include/modeGraphique.hpp"


void testGrille() {
    std::cout << "=== Test Grille ===" << std::endl;

    // Création d'une grille
    Grille grille(5, 5);
    std::cout << "Grille créée avec dimensions : 5x5" << std::endl;

    // Ajout de cellules vivantes et obstacles
    grille.ajoutEntite(1, 1, new Cellules(true));
    grille.ajoutEntite(2, 2, new Cellules(false));
    grille.ajoutEntite(3, 3, new Obstacle(false));

    // Vérification des voisins vivants
    int voisins = grille.calculerVoisins(2, 2);
    std::cout << "Voisins vivants autour de (2,2) : " << voisins << std::endl;

    // Mise à jour de l’état
    grille.MettreAJour();
    std::cout << "Mise à jour de l’état de la grille effectuée." << std::endl;
}

void testAfficheurConsole() {
    std::cout << "=== Test Afficheur Console ===" << std::endl;

    Grille grille(5, 5);
    grille.ajoutEntite(1, 1, new Cellules(true));
    grille.ajoutEntite(2, 2, new Cellules(false));
    grille.ajoutEntite(3, 3, new Obstacle(false));

    AfficheurConsole afficheur;
    std::cout << "Affichage initial de la grille :" << std::endl;
    afficheur.Afficher(grille);
}

void testModeConsole(std::string nomFichier) {
    int iterationMax;
    std::cout << "=== Test Mode Console ===" << std::endl;
    std::cout << "Combien d'iteration souhaite :" << std::endl;
    std::cin >> iterationMax;

    Grille grille(0, 0);

    // Charger le fichier et initialiser la grille
    if (grille.chargerFichier(nomFichier)) {
        std::cout << "Grille initialisée avec succès depuis le fichier !" << std::endl;
    } else {
        std::cerr << "Erreur lors de l'initialisation de la grille depuis le fichier." << std::endl;
    }

    ModeConsole mode(&grille, iterationMax, nomFichier); 
    mode.simuler();
}

void testModeGraphique(std::string nomFichier) {
    std::cout << "=== Test Mode Graphique ===" << std::endl;

    Grille grille(0, 0);

    // Charger le fichier et initialiser la grille
    if (grille.chargerFichier(nomFichier)) {
        std::cout << "Grille initialisée avec succès depuis le fichier !" << std::endl;
    } else {
        std::cerr << "Erreur lors de l'initialisation de la grille depuis le fichier." << std::endl;
    }
    
    ModeGraphique mode(&grille); 
    mode.simuler();
}


int main() {

    // Demande à l'utilisateur de saisir le nom du fichier
    // std::string nomFichier;
    // std::cout << "Entrez le nom du fichier de configuration : ";
    // std::cin >> nomFichier;

    std::string nomFichier = "config";
    // testGrille();
    // std::cout << std::endl;

    // testAfficheurConsole();
    // std::cout << std::endl;

    // testModeConsole(nomFichier);
    // std::cout << std::endl;

    testModeGraphique(nomFichier);
    std::cout << std::endl;

    return 0;
}

