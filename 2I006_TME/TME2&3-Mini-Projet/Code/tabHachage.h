#ifndef _TAB_HACHAGE_
#define _TAB_HACHAGE_

#include"liste.h"

typedef struct cell {
    int clef;
    Livre *data;
    struct cell *suiv;
}Cell;

typedef struct{
    int n;
    int m;
    Cell **T;
}TabHachage;

TabHachage *initTabHachage(int m);

int fonctionClef(char *nom);

int fonctionHachage(int clef, int m);

int insererDansTabHach(TabHachage *tab, Livre *data);

int supprimerUnLivreDansHach(TabHachage *tab, Livre *livre);

Livre *rechercheParAuteurDansHach(TabHachage *tab, char *auteur);

TabHachage *livreDeMemeAuteurDansHach(TabHachage *tab, char *auteur);

TabHachage *livrePlusDeDeuxDansHach(TabHachage *tab);

void afficherTabHachage(TabHachage *tab);

void afficherNbDansChaqueCase(TabHachage *tab);
#endif
