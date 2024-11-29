#ifndef ENTITE_H
#define ENTITE_H

class Entite {
protected:
    bool etat;
public:
    Entite(bool etatInitial) : etat(etatInitial) {}
    virtual ~Entite() {}


    bool estVivante();
    void changerEtat(bool etat);

    virtual void DeterminerEtatSuivant(int voisinsVivants);

};

#endif // ENTITE_H