#include <stdio.h>
#include <stdlib.h>
#include "structure.h"
#include "fonction.h"

int intersection(Netlist *N, Segment *s1, Segment *s2){
    Point *s1p1, *s1p2;
    Point *s2p1, *s2p2;

    if(s1->NumRes == s2->NumRes){
	return 0;
    }
    if(s1->HouV == s2->HouV){//si les deux segments sont dans la meme position alors non intersection
	return 0;//non intersection
    }


    s1p1 = N->T_Res[s1->NumRes]->T_Pt[s1->p1];//point bas gauche de segment s1
    s1p2 = N->T_Res[s1->NumRes]->T_Pt[s1->p2];//point haut droit de segment s1
    s2p1 = N->T_Res[s2->NumRes]->T_Pt[s2->p1];//point bas gauche de segment s2
    s2p2 = N->T_Res[s2->NumRes]->T_Pt[s2->p2];//point haut droit de segment s2

    if(s1->HouV == 0 && s2->HouV == 1){//s1 horizontal et s2 vertical
	if(s1p1->x <= s2p1->x && s2p1->x <= s1p2->x){//si abscisse du s2 comprise entre s1
	    if(s2p1->y <= s1p1->y && s1p1->y <= s2p2->y){//si ordonne du s1 comprise entre s2
		return 1;//intersection
	    }
	}
    }
    else{//s1 vertical et s2 horizontal
	if(s2p1->x <= s1p1->x && s1p1->x <= s2p2->x){//si abscisse du s1 comprise entre s2
	    if(s1p1->y <= s2p1->y && s2p1->y <= s1p2->y){//si ordonne du s2 comprise entre s1
		return 1;//intersection
	    }
	}
    }

    return 0;//non intersection
}

int countNbSegmentReseau(Reseau *reseau){
    Point **T_Pt;
    Cell_segment *cell_seg;
    int i, count = 0;
    int NbPt;
    if(reseau == NULL){
	return -1;
    }
    T_Pt = reseau->T_Pt;
    NbPt = reseau->NbPt;
    //on compte le nombre total de segment en intersection pour chaque point
    for(i = 0; i < NbPt; i++){
	cell_seg =T_Pt[i]->Lincid;
	while(cell_seg){
	    count++;
	    cell_seg = cell_seg->suiv;
	}
    }
    //comme chaque segment se trouve a la fois dans la liste d intersection de premier point et de la seconde, on divise donc le nombre de segment total par 2
    return count/2;
}

int countNbSegmentNetlist(Netlist *netlist){
    int count = 0;
    int i;
    if(netlist == NULL){
	return -1;
    }
    for(i = 0; i < netlist->NbRes; i++){
	count += countNbSegmentReseau(netlist->T_Res[i]);
    }
    return count;
}

int countNbEcheancierNetlist(Netlist *netlist){
    Point **T_Pt;
    Cell_segment *cell_seg;
    int i, countVerti = 0, countHori = 0;
    int NbPt, nbres;
    if(netlist == NULL){
	return -1;
    }

    for(nbres = 0; nbres < netlist->NbRes; nbres++){
	T_Pt = netlist->T_Res[nbres]->T_Pt;
	NbPt = netlist->T_Res[nbres]->NbPt;
    
	for(i = 0; i < NbPt; i++){
	    cell_seg =T_Pt[i]->Lincid;
	    while(cell_seg){
		if(cell_seg->seg->HouV == 0){
		    countHori++;
		}
		else {
		    countVerti++;
		}
		cell_seg = cell_seg->suiv;
	    }
	}
    }
    return countVerti/2 + countHori;
}

Segment **tabSegmentDeReseau(Reseau *T_Res, int nbseg){
    int index = 0, n, exist, i;
    Cell_segment *cell_segment;
    Segment **tabseg = malloc(sizeof(tabseg) * nbseg);

    for(n = 0; n < T_Res->NbPt; n++){
	cell_segment = T_Res->T_Pt[n]->Lincid;
	while(cell_segment){
	    exist = 0;
	    for(i = 0; i < index; i++){
		if(cell_segment->seg == tabseg[i]){
		    exist = 1;
		    break;
		}
	    }
	    if(exist == 0){
		tabseg[index] = cell_segment->seg;
		index++;
	    }
	    cell_segment = cell_segment->suiv;
	}
    }
    return tabseg;
}

Segment **tabSegmentDeNetlist(Netlist* netlist, int nbseg){
    Reseau* T_Res;
    int index = 0, n, exist, i, x;
    Cell_segment *cell_segment;
    Segment **tabseg = malloc(sizeof(tabseg) * nbseg);

    for(x = 0; x < netlist->NbRes; x++){
	T_Res = netlist->T_Res[x];
	
	for(n = 0; n < T_Res->NbPt; n++){
	    cell_segment = T_Res->T_Pt[n]->Lincid;
	    while(cell_segment){
		exist = 0;
		for(i = 0; i < index; i++){
		    if(cell_segment->seg == tabseg[i]){
			exist = 1;
			break;
		    }
		}
		if(exist == 0){
		    tabseg[index] = cell_segment->seg;
		    index++;
		}
		cell_segment = cell_segment->suiv;
	    }
	}
    }
    return tabseg;

}

int listIntersectDeSeg_naif(Netlist *netlist, Segment **tabSegReseau[netlist->NbRes], int tabNbSeg[netlist->NbRes], Segment *seg){
    int numres, numseg, nbinter = 0;
    Segment **tabSeg;
    Segment *segbis;

    for(numres = 0; numres <netlist->NbRes; numres++){
	if(seg->NumRes == numres){
	    continue;
	}
	tabSeg = tabSegReseau[numres];
	for(numseg = 0; numseg < tabNbSeg[numres]; numseg++){
	    segbis = tabSeg[numseg];
	    if(existeDansListSeg(seg->Lintersec, segbis) == 1){
		continue;
	    }

	    if(intersection(netlist, seg, segbis) == 1){
		nbinter++;
		seg->Lintersec = insertSegment(seg->Lintersec, segbis);
		segbis->Lintersec = insertSegment(segbis->Lintersec, seg);		
	    }
	}
    }
    return nbinter;
}

int intersect_naif(Netlist *netlist){
    int numres, numseg, nbinter = 0;
    Segment **tabSeg;;
    
    if(netlist == NULL){
	return -1;
    }
    int tabNbSeg[netlist->NbRes];
    Segment **tabSegReseau[netlist->NbRes];

    for(numres = 0; numres < netlist->NbRes; numres++){
	tabNbSeg[numres] = countNbSegmentReseau(netlist->T_Res[numres]);
	tabSegReseau[numres] = tabSegmentDeReseau(netlist->T_Res[numres], tabNbSeg[numres]);
    }
    for(numres = 0; numres < netlist->NbRes; numres++){
	tabSeg = tabSegReseau[numres];

	for(numseg = 0; numseg < tabNbSeg[numres]; numseg++){
	    nbinter += listIntersectDeSeg_naif(netlist, tabSegReseau, tabNbSeg, tabSeg[numseg]);
	}
    }

    for(numres = 0; numres < netlist->NbRes; numres++){
	free(tabSegReseau[numres]);
    }

    netlist->NbInter = nbinter;
    return nbinter;
}

int compareExtremite(Extremite *extre1, Extremite *extre2){
    if((int)extre1->x < (int)extre2->x){
	return -1;
    }
    if((int)extre1->x > (int)extre2->x){
	return 1;
    }

    if(extre1->VouGouD == 0){
	if(extre2->VouGouD == 1){
	    return 1;
	}
	else{
	    return -1;
	}
    }
    else if(extre1->VouGouD == 1){
	return -1;
    }
    else {
	if(extre2->VouGouD == 2){
	    return -1;
	}
	else {
	    return 1;
	}
    }
}

int triRapide_Separation_Extremite(Extremite ** tabExtremite, int deb, int fin){
    int i, sep = deb+1;
    Extremite * tmp;
    for(i = deb+1; i <= fin; i++){
	if(compareExtremite(tabExtremite[i], tabExtremite[deb]) == -1){
	    if(i != sep){
		tmp = tabExtremite[i];
		tabExtremite[i] = tabExtremite[sep];
		tabExtremite[sep] = tmp;
	    }
	    sep++;
	}
    }

    if(sep != deb+1){
	tmp = tabExtremite[deb];
	tabExtremite[deb] = tabExtremite[sep-1];
	tabExtremite[sep-1] = tmp;
    }
    return sep-1;
}

void triRapide_Extremite(Extremite **tabExtremite, int deb, int fin){
    int milieu;
    if(deb < fin){
	milieu = triRapide_Separation_Extremite(tabExtremite, deb, fin);
	triRapide_Extremite(tabExtremite, deb, milieu-1);
	triRapide_Extremite(tabExtremite, milieu+1, fin);
    }
}

void triRapide_Echeancier(Extremite **echeancier, int taille){
    triRapide_Extremite(echeancier, 0, taille-1);
}

Segment *premierSegmentApres(Cell_segment *list, int y, Netlist *netlist){
    if(list == NULL || netlist == NULL){
	return NULL;
    }

    while(list != NULL && netlist->T_Res[list->seg->NumRes]->T_Pt[list->seg->p1]->y < y){
	list = list->suiv;
    }
    if(list == NULL){
	return NULL;
    }
    return list->seg;
}

Segment *auDessus(Cell_segment *list, Segment *seg){
    while(list != NULL && list->seg != seg){
	list = list->suiv;
    }
    if(list->suiv == NULL){
	return NULL;
    }
    return list->suiv->seg;
}

int intersect_balayage(Netlist *netlist){
    int i, y1, y2, nbinter = 0;
    int nb_echeanchier;
    Extremite *point;
    Segment *h;
    Cell_segment *T = NULL;
    Extremite **tab_echeancier;

    tab_echeancier = creerEcheancier(netlist, &nb_echeanchier);
    triRapide_Echeancier(tab_echeancier, nb_echeanchier);
    
    /*     for(i = 0; i < nb_echeanchier; i++){
	printf("%f    %d\n", tab_echeancier[i]->x, tab_echeancier[i]->VouGouD);
    }
    */

    for(i = 0; i < nb_echeanchier; i++){
	point = tab_echeancier[i];

	if(point->VouGouD == 1){
	    T = insertSegmentParOrdonne(T, point->PtrSeg, netlist);
	}
	if(point->VouGouD == 2){
	    T = supprimerUnCell_segment(T, point->PtrSeg);
	}
	if(point->VouGouD == 0){
	    y1 = netlist->T_Res[point->PtrSeg->NumRes]->T_Pt[point->PtrSeg->p1]->y;
	    y2 = netlist->T_Res[point->PtrSeg->NumRes]->T_Pt[point->PtrSeg->p2]->y;
	    h = premierSegmentApres(T, y1, netlist);
	    while(h != NULL && netlist->T_Res[h->NumRes]->T_Pt[h->p1]->y <= y2){
		if(h->NumRes != point->PtrSeg->NumRes){
		    nbinter++;
		    point->PtrSeg->Lintersec = insertSegment(point->PtrSeg->Lintersec, h);
		    h->Lintersec = insertSegment(h->Lintersec, point->PtrSeg);
		}
		h = auDessus(T, h);
	    }	    
	}
    }

    for(i = 0; i < nb_echeanchier; i++){
	free(tab_echeancier[i]);
    }
    free(tab_echeancier);

    netlist->NbInter = nbinter;
    return nbinter;
}

int countNbPointNetlist(Netlist* netlist){
    int nbPoint = 0;
    int numRes;

    for(numRes = 0; numRes < netlist->NbRes; numRes++){
	nbPoint += netlist->T_Res[numRes]->NbPt;
    }

    return nbPoint;
}

Point **tabPointNetlist(Netlist *netlist, int nbPoint){
    Point **T_Pt;
    Point **tabPoint = (Point**)malloc(sizeof(Point*) * nbPoint);
    int numPoi, numPoint, numRes;
    if(tabPoint == NULL){
	printf("fail d allocation de tableau de pointeur sur Point\n");
	return NULL;
    }

    numPoi = 0;
    for(numRes = 0; numRes < netlist->NbRes; numRes++){
	T_Pt = netlist->T_Res[numRes]->T_Pt;
	for(numPoint = 0; numPoint < netlist->T_Res[numRes]->NbPt; numPoint++, numPoi++){
	    tabPoint[numPoi] = T_Pt[numPoint];
	}
    }

    return tabPoint;
}
