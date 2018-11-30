#ifndef _LISTE
#define _LISTE


typedef struct _livre{
    char *titre;
    char *auteur;
    int num;
}Livre;

typedef struct _listeLivre{
    Livre* livre;
    struct _listeLivre* suiv;
}ListeLivre;

int insererEnTete(ListeLivre **liste, Livre *livre);

int supprimerUnLivre(ListeLivre **liste, Livre *livre);

Livre *rechercheParNum(ListeLivre *liste, int num);

Livre *rechercheParTitre(ListeLivre *liste, char *titre);

ListeLivre *livreDeMemeAuteur(ListeLivre *liste, char *auteur);

ListeLivre *livrePlusDeDeux(ListeLivre *liste);

void afficherListeLivre(ListeLivre *liste);
#endif
