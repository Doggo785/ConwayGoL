#ifndef ENTITE_H
#define ENTITE_H

class Entite {
protected:
    bool etat;
public:
    Entite(bool etatInitial) : etat(etatInitial) {}
    virtual ~Entite() {}


    bool estVivante() const { return etat; }
    void changerEtat(bool nouvelEtat) { etat = nouvelEtat; }

    virtual bool DeterminerEtatSuivant(int voisinsVivants) = 0;

};

#endif // ENTITE_H