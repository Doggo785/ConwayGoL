#ifndef MODEGRAPHIQUE_HPP
#define MODEGRAPHIQUE_HPP

#include "modeSimulation.hpp"
#include "afficheurGraphique.hpp"
#include <SFML/Graphics.hpp>
#include <thread>
#include <atomic>

enum Etat {
    Menu,
    Editeur,
    Simulation
};

class ModeGraphique : public ModeSimulation {
private:
    sf::RenderWindow window;
    AfficheurGraphique afficheur;
    Etat etatCourant;
    int iterations;
    std::atomic<int> vitesse;
    std::atomic<bool> simulationLancee;
    std::atomic<bool> quitterSimulation;
    std::thread simulationThread;

    void gererEvenementsMenu(bool& quitter);
    void gererEvenementsEditeur(bool& quitter);
    void gererEvenementsSimulation(bool& quitter);
    void boucleSimulation();

public:
    ModeGraphique(Grille* grille);
    ~ModeGraphique();
    void simuler() override;
};

#endif // MODEGRAPHIQUE_HPP