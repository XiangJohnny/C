#ifndef METHODE_H
#define METHODE_H

#include "structure.h"


int intersection(Netlist *N, Segment *s1, Segment *s2);

//pour un Reseau on compte le nombre de segment
int countNbSegmentReseau(Reseau *reseau);

int countNbSegmentNetlist(Netlist *netlist);

int countNbEcheancierNetlist(Netlist *netlist);

Segment **tabSegmentDeReseau(Reseau *T_Res, int nbseg);

Segment **tabSegmentDeNetlist(Netlist* netlist, int nbseg);

int intersect_naif(Netlist *netlist);

void triRapide_Echeancier(Extremite **echeancier, int taille);

Segment *premierSegmentApres(Cell_segment *list, int y, Netlist *netlist);

Segment *auDessus(Cell_segment *list, Segment *seg);

int intersect_balayage(Netlist *netlist);

int countNbPointNetlist(Netlist *netlist);

Point **tabPointNetlist(Netlist *netlist, int nbPoint);
#endif
