#include <stdio.h>
#include "entree_sortie.h"
#include "fonction.h"
#include "structure.h"

Cell_segment* insertSegment(Cell_segment* cell_segment, Segment* seg){
    Cell_segment* cell_seg = (Cell_segment*)malloc(sizeof(Cell_segment));
    cell_seg->seg = seg;
    if(cell_segment == NULL){
	cell_seg->suiv = NULL;
	return cell_seg;
    }
    cell_seg->suiv = cell_segment;
    cell_segment = cell_seg;
    return cell_segment;
}

void detruireCell_segment(Cell_segment *cell_segment){
    Cell_segment *tmp;
    while(cell_segment != NULL){
	tmp = cell_segment;
	cell_segment = cell_segment->suiv;
	free(tmp);
    }
}

Netlist *creerNetlist(int nbRes){
    Netlist *netlist = (Netlist*)malloc(sizeof(Netlist));
    if(netlist == NULL){
	return NULL;
    }
    netlist->NbInter = 0;
    netlist->NbRes = nbRes;
    netlist->T_Res = (Reseau**)malloc(sizeof(Reseau*) * nbRes);
    return netlist;
}

Reseau *creerReseau(int numRes, int nbPt){
    Reseau* reseau = (Reseau*)malloc(sizeof(Reseau));
    if(reseau == NULL){
	return NULL;
    }
    reseau->NumRes = numRes;
    reseau->NbPt = nbPt;
    reseau->T_Pt = (Point**)malloc(sizeof(Point*) * nbPt);
    return reseau;
}

Point* creerPoint(int numRes, int x, int y){
    Point* point = (Point*)malloc(sizeof(Point));
    if(point == NULL){
	return NULL;
    }
    point->num_res = numRes;
    point->x = x;
    point->y = y;
    point->Lincid = NULL;
    return point;
}

Segment* creerSegment(int numRes, int numP1, int numP2, Point* p1, Point* p2){
    Segment* segment = (Segment*)malloc(sizeof(Segment));
    if(segment == NULL || p1 == NULL || p2 == NULL){
	return NULL;
    }
    segment->NumRes = numRes;
    segment->Lintersec = NULL;

    if(p1->x <= p2->x && p1->y <= p2->y){//si le point p1 et en bas a gauche de p2
	segment->p1 = numP1;//le premier numero de point et celui de p1
	segment->p2 = numP2;//le seconde numero de point et celui de p2
    }
    else{
	segment->p1 = numP2;//sinon le contraire
	segment->p2 = numP1;
    }
    
    if(p1->x == p2->x && p1->y != p2->y){
	segment->HouV = 1;
    }
    else{
	segment->HouV = 0;
    }
    
    return segment;
}

int compareSegment(Segment *seg1, Segment *seg2){
    if(seg1 == NULL || seg2 == NULL){
	return 0;
    }
    if(seg1 == seg2){
	return 1;
    }
    if(seg1->NumRes == seg2->NumRes && seg1->p1 == seg2->p1 && seg1->p2 == seg2->p2){
	return 1;
    }
    return 0;
}

int existeDansListSeg(Cell_segment *cell_segment, Segment *seg){
    if(cell_segment == NULL || seg == NULL){
	return 0;
    }

    while(cell_segment){
	if(compareSegment(cell_segment->seg, seg) == 1){
	    return 1;
	}
	cell_segment = cell_segment->suiv;
    }
    return 0;
}

Cell_segment* insertSegmentParOrdonne(Cell_segment *cell_segment, Segment *seg, Netlist *netlist){
    Cell_segment *cell_seg = (Cell_segment*)malloc(sizeof(Cell_segment));
    Cell_segment *parcourt;
    if(cell_seg == NULL){
	return NULL;
    }
    cell_seg->seg = seg;
    if(cell_segment == NULL){
	cell_seg->suiv = NULL;
	return cell_seg;
    }

    if(netlist->T_Res[cell_segment->seg->NumRes]->T_Pt[cell_segment->seg->p1]->y >
       netlist->T_Res[seg->NumRes]->T_Pt[seg->p1]->y){
	cell_seg->suiv = cell_segment;
	cell_segment = cell_seg;
	return cell_seg;
    }
    
    parcourt = cell_segment;
    while(parcourt->suiv != NULL && netlist->T_Res[parcourt->suiv->seg->NumRes]->T_Pt[parcourt->suiv->seg->p1]->y <
	  netlist->T_Res[seg->NumRes]->T_Pt[seg->p1]->y){
	parcourt = parcourt->suiv;
    }

    cell_seg->suiv = parcourt->suiv;
    parcourt->suiv = cell_seg;
    
    return cell_segment;
}

Cell_segment *supprimerUnCell_segment(Cell_segment * cell_segment, Segment *seg){
    Cell_segment *parcourt;
    Cell_segment *tmp;
    if(cell_segment == NULL || seg == NULL){
	return NULL;
    }

    if(compareSegment(cell_segment->seg, seg) == 1){
	tmp = cell_segment->suiv;
	free(cell_segment);
	return tmp;
    }
    parcourt = cell_segment;
    while(parcourt->suiv != NULL && compareSegment(parcourt->suiv->seg, seg) == 0){
	parcourt = parcourt->suiv;
    }

    if(parcourt->suiv == NULL){
	return cell_segment;
    }
    tmp = parcourt->suiv;
    parcourt->suiv = parcourt->suiv->suiv;
    free(tmp);
    return cell_segment;
}

Extremite *creerExtremite(int numpoint, Point* point, Segment *seg){
    Extremite *extre = malloc(sizeof(*extre));

    if(extre == NULL){
	printf("fail d allocation de extremite\n");
	return NULL;
    }
    extre->x = point->x;
    extre->PtrSeg = seg;
    extre->NumPt = numpoint;

    if(seg->HouV == 0){
	if(seg->p1 == numpoint){
	    extre->VouGouD = 1;
	}
	else {
	    extre->VouGouD = 2;
	}
    }
    else {
	extre->VouGouD = 0;
    }
    return extre;
}

Extremite **creerEcheancier(Netlist * netlist, int *nbEcheancier){
    Point *point;;
    Reseau *reseau;
    int numExtremite = 0;
    int numres, numpoint;
    Extremite **tabExtremite;
    Cell_segment *listParcourtLincid;
    Cell_segment *segVercicalDejaParcourue;

    *nbEcheancier =  countNbEcheancierNetlist(netlist);
    tabExtremite = malloc(sizeof(*tabExtremite) * *nbEcheancier);

    for(numres = 0; numres < netlist->NbRes; numres++){
	segVercicalDejaParcourue = NULL;
	reseau = netlist->T_Res[numres];
	
	for(numpoint = 0; numpoint < reseau->NbPt; numpoint++){
	    point = reseau->T_Pt[numpoint];
	    listParcourtLincid = point->Lincid;

	    while(listParcourtLincid){
		if(listParcourtLincid->seg->HouV == 1){
		    if(existeDansListSeg(segVercicalDejaParcourue, listParcourtLincid->seg) == 0){
			segVercicalDejaParcourue = insertSegment(segVercicalDejaParcourue, listParcourtLincid->seg);
			tabExtremite[numExtremite] = creerExtremite(numpoint, point, listParcourtLincid->seg);
			numExtremite++;
		    }
		}
		else {
		    tabExtremite[numExtremite] = creerExtremite(numpoint, point, listParcourtLincid->seg);			
		    numExtremite++;
		}
		listParcourtLincid = listParcourtLincid->suiv;
	    }
	}
	detruireCell_segment(segVercicalDejaParcourue);
    }

    if(*nbEcheancier != numExtremite){
	printf("incompatible entre nombre extremite:%d et celui produit:%d\n",*nbEcheancier, numExtremite);
    }
    return tabExtremite;
}


