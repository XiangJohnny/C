#ifndef GRAPHE_H
#define GRAPHE_H

#include "structure.h"

typedef struct _arc{
    int som1;
    int som2;
} Arc;

typedef struct _elementListeA{
    Arc* elem;
    struct _elementListeA* suiv;
} ElemListeA;

typedef struct _sommet{
    int num;
    int SouP;// 0 pour segment  1 pour point
    void* elem;//un segment ou point
    int nbArc;
    ElemListeA* Lincid;
} Sommet;

typedef struct _elementListeS{
    Sommet *elem;
    struct _elementListeS *suiv;
} ElemListeS;

typedef struct _graphe{
    int nbSom;//nb de sommets
    int nbArc;//nb d aretes
    Sommet** tabS;
    Arc** tabA;
} Graphe;

Sommet* creer_Sommet(int num, int SouP, void* elem);

Arc* creer_Arc(int som1, int som2);

ElemListeA* creer_ElemListeA(Arc* elem);

ElemListeA* insert_ElemListeA(ElemListeA* liste, Arc* elem);

void detruire_ListeA(ElemListeA *liste);

ElemListeS *creer_ElemListeS(Sommet *elem);

ElemListeS *insert_ElemListeS(ElemListeS *liste, Sommet *elem);

void detruire_ListeS(ElemListeS *liste);

Graphe* creer_Graphe(Netlist* netlist, char* inter_filename);

ElemListeS *detect_cycle_impaire(Graphe *graphe, int *tabDetection);

int *ajout_vias_cycle_impair(Graphe *graphe);
#endif
