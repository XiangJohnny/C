#ifndef STRUCTURE_H
#define STRUCTURE_H

struct segment ;

typedef struct cell_segment {
    struct segment *seg;
    struct cell_segment *suiv;
} Cell_segment;

typedef struct segment {
    int NumRes; /* Numero du reseau auquel appartient ce segment*/
    int p1, p2; /* Numero des points aux extremites du segment */
    /* En utilisant la numerotation de T_Pt */
    /* p1 est le point en bas a gauche par rapport a p2*/
    int HouV; /* 0 si Horizontal et 1 si Vertical */
    struct cell_segment *Lintersec; /* Liste des segments en intersection */
} Segment;
typedef struct point {
    double x,y; /* Coordonnees du point */
    int num_res; /* Numero du reseau contenant ce point = Index du tableau T_res*/
    Cell_segment *Lincid; /* Liste des segments incidents a ce point */
} Point;

typedef struct reseau {
    int NumRes; /* Numero du reseau = Indice dans le tableau T_Res */
    int NbPt; /* Nombre de points de ce reseau */
    Point * *T_Pt; /* Tableau de pointeurs sur chaque point de ce reseau */
} Reseau;

typedef struct netlist {
    int NbInter; /* Nombre d intersection de la netlist */
    int NbRes; /* Nombre de reseaux */
    Reseau* *T_Res; /* Tableau pointant sur chaque reseau */
} Netlist;

typedef struct extremite{
    double x; /* Abscisse du point */
    int VouGouD; /* 0 si segment V / 1 si pt gauche d’un segment H / 2 si pt droit d’un segment H */
    Segment * PtrSeg; /* Pointeur sur le segment correspondant a l’extremite */
    int NumPt; /* si segment H: numero du point correspondant */
} Extremite;

int compareSegment(Segment *seg1, Segment *seg2);

//insertion en tete de liste
Cell_segment* insertSegment(Cell_segment* cell_segment, Segment* seg);

//detruit la liste des segments mais pas les segments qui est contenue dedans
void detruireCell_segment(Cell_segment *cell_segment);

//creer un Netlist et le retourne, en cas de fail retourne NULL
Netlist *creerNetlist(int nbRes);

//creer un Reseau et le retourne, en cas de fail retourne NULL
Reseau *creerReseau(int numRes, int nbPt);

//creer un Point et le retourne, en cas de fail retourne NULL
Point* creerPoint(int numRes, int x, int y);

//creer un Segment et le retourne, en can de fail retourne NULL
Segment* creerSegment(int numRes, int numP1, int numP2, Point* p1, Point* p2);

//retourne 1 si le Segment se trouve dans la liste de Segment sinon 0, en cas de fail retourne -1
int existeDansListSeg(Cell_segment *cell_segment, Segment *seg);

//creer un extremite et le retourne, en cas de fail return NULL
Extremite *creerExtremite(int numpoint, Point* point, Segment *seg);

//creer un tableau de pointeur sur les extremites et le retourne, la taille est retourner par un passage de pointeur en argumant
Extremite **creerEcheancier(Netlist * netlist, int *nbEcheancier);

Cell_segment* insertSegmentParOrdonne(Cell_segment *cell_segment, Segment *seg, Netlist *netlist);

Cell_segment * supprimerUnCell_segment(Cell_segment *cell_segment, Segment *seg);
#endif
