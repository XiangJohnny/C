#include <stdio.h>
#include <stdlib.h>
#include "graphe.h"
#include "structure.h"
#include "entree_sortie.h"
#include "fonction.h"

Sommet* creer_Sommet(int num, int SouP, void* elem){
    Sommet* sommet = (Sommet*)malloc(sizeof(Sommet));
    if(sommet == NULL){
	printf("fail allocation de sommet\n");
	// detruire graphe
	return NULL;
    }
	
    sommet->num = num;
    sommet->SouP = SouP;
    sommet->elem = elem;
    sommet->nbArc = 0;
    sommet->Lincid = NULL;
	
    return sommet;
}

Arc* creer_Arc(int som1, int som2){
    Arc* arc = (Arc*)malloc(sizeof(Arc));
    if(arc == NULL){
	printf("fail allocation d Aretes\n");
	return NULL;
    }
    arc->som1 = som1;
    arc->som2 = som2;

    return arc;
}

ElemListeA* creer_ElemListeA(Arc* elem){
    ElemListeA* elemlistea = (ElemListeA*)malloc(sizeof(ElemListeA));
    if(elemlistea == NULL){
	printf("fail allocation ElemListeA\n");
	return NULL;
    }

    elemlistea->elem = elem;
    elemlistea->suiv = NULL;

    return elemlistea;
}

ElemListeA* insert_ElemListeA(ElemListeA* liste, Arc* elem){
    ElemListeA* elemlistea = creer_ElemListeA(elem);
    if(liste != NULL){
	elemlistea->suiv = liste;
    }

    return elemlistea;
}

void detruire_ListeA(ElemListeA *liste){
    if(liste->suiv != NULL){
	detruire_ListeA(liste->suiv);
    }
    free(liste);
}

ElemListeS *creer_ElemListeS(Sommet *elem){
    ElemListeS *elemlistes = (ElemListeS*)malloc(sizeof(ElemListeS));
    if(elemlistes == NULL){
	printf("fail allocation ElemListes\n");
	return NULL;
    }

    elemlistes->elem = elem;
    elemlistes->suiv = NULL;
    return elemlistes;
}

ElemListeS* insert_ElemListeS(ElemListeS *liste, Sommet *elem){
    ElemListeS* elemlistes = creer_ElemListeS(elem);
    if(liste != NULL){
	elemlistes->suiv = liste;
    }

    return elemlistes;
}

void detruire_ListeS(ElemListeS *liste){
    if(liste->suiv != NULL){
	detruire_ListeS(liste->suiv);
    }
    free(liste);
}

Graphe* creer_Graphe(Netlist* netlist, char* inter_filename){
    int numSom, numPoi, nbPoi, numArc, nbSeg;
    Sommet* s1, *s2;
    Sommet** tabS;
    Arc** tabA;
    Point *point;
    nbPoi = countNbPointNetlist(netlist);
    nbSeg = countNbSegmentNetlist(netlist);
    Point **tabPoint = tabPointNetlist(netlist, nbPoi);
    FILE* file = fopen(inter_filename, "r");
    Segment** tabSeg = tabSegmentDeNetlist(netlist, nbSeg);
    Graphe* graphe = (Graphe*)malloc(sizeof(Graphe));
    if(graphe == NULL){
	printf("fail allocation de Graphe\n");
	return NULL;
    }
    graphe->nbSom = nbSeg + nbPoi;
    graphe->nbArc = netlist->NbInter + nbSeg*2;

    graphe->tabS = (Sommet**)malloc(sizeof(Sommet*) * graphe->nbSom);
    if(graphe->tabS == NULL){
	printf("fail allocation de tableau de sommet de graphe\n");
	return NULL;
    }
    graphe->tabA = (Arc**)malloc(sizeof(Arc*) * graphe->nbArc);
    if(graphe->tabA == NULL){
	printf("fail allocation de tableau d aretes de graphe\n");
	return NULL;
    }
    tabS = graphe->tabS;

    for(numSom = 0; numSom < nbSeg; numSom++){
	tabS[numSom] = creer_Sommet(numSom, 0, tabSeg[numSom]);
    }

    numPoi = 0;
    for(numSom = nbSeg; numSom < nbSeg+nbPoi; numSom++, numPoi++){
	tabS[numSom] = creer_Sommet(numSom, 1, tabPoint[numPoi]);
    }

    numArc = 0;
    tabA = graphe->tabA;
    for(numSom = 0; numSom < nbSeg; numSom++){
	s1 = tabS[numSom];
	for(numPoi = nbSeg; numPoi < nbSeg+nbPoi; numPoi++){
	    s2 = tabS[numPoi];
	    point = (Point*)s2->elem;
	    if(existeDansListSeg(point->Lincid, (Segment*)s1->elem) == 1){
		tabA[numArc] = creer_Arc(numSom, numPoi);
		s1->Lincid = insert_ElemListeA(s1->Lincid, tabA[numArc]);
		s2->Lincid = insert_ElemListeA(s2->Lincid, tabA[numArc]);
		numArc++;
	    }
	}
    }

    Segment* seg;
    int b1, b2;
    int r1, r2, p11, p12, p21, p22;
    int nbinter = netlist->NbInter;
    while(nbinter != 0 && !feof(file)){
	b1 = 0;
	b2 = 0;
	r1 = GetEntier(file);
	p11 = GetEntier(file);
	p12 = GetEntier(file);
	r2 = GetEntier(file);
	p21 = GetEntier(file);
	p22 = GetEntier(file);

	//	printf("%d %d %d %d %d %d\n",r1,p11,p12,r2,p21,p22);

	for(numSom = 0; numSom < nbSeg; numSom++){
	    seg = (Segment*)tabS[numSom]->elem;
	    if(b1 == 0){
		if(seg->NumRes == r1 && seg->p1 == p11 && seg->p2 == p12){
		    s1 = tabS[numSom];
		    b1 = 1;
		}
	    }
	    if(b2 == 0){
		if(seg->NumRes == r2 && seg->p1 == p21 && seg->p2 == p22){
		    s2 = tabS[numSom];
		    b2 = 1;
		}
	    }
	    if(b1 == 1 && b2 == 1){
		break;
	    }
	}

	tabA[numArc] = creer_Arc(s1->num, s2->num);
	s1->Lincid = insert_ElemListeA(s1->Lincid, tabA[numArc]);
	s2->Lincid = insert_ElemListeA(s2->Lincid, tabA[numArc]);
	numArc++;
	nbinter--;
    }

    if(numArc != graphe->nbArc){
	printf("Incompatible entre nombre d arc:%d et celui produit:%d\n",graphe->nbArc, numArc);
     }

    free(tabSeg);
    free(tabPoint);
    fclose(file);
    return graphe;
}

int detect_cycle_impaire_descendant(Graphe *graphe, int r, int *visit, int pere, int *point_dep_cycle, ElemListeS **liste){
    int v;
    ElemListeA *cour;
    int detect = 0;

    if(visit[pere] == 1){
	visit[r] = 2;
    }
    else {
	visit[r] = 1;
    }

    cour = graphe->tabS[r]->Lincid;
    while( cour && !detect){
	v = (r == cour->elem->som1) ? cour->elem->som2 : cour->elem->som1;

	if(visit[v] == -1){
	    detect = detect_cycle_impaire_descendant(graphe, v, visit, r, point_dep_cycle, liste);
	    if(*point_dep_cycle != -1){
		*liste = insert_ElemListeS(*liste, graphe->tabS[v]);
		//printf("%d<=",r);
		if(*point_dep_cycle == r){
		    *point_dep_cycle = -1;
		}
	    }
	}
	else {
	    if(visit[r] == visit[v]){
		*point_dep_cycle = v;
		//	printf("%d<=",v);
		//printf("%d<=",r);
		detect = 1;
	    }
	}
	cour = cour->suiv;
    }

    visit[r] = 0;
    return detect;   
}

ElemListeS *detect_cycle_impaire(Graphe *graphe, int *tabDetection){
    ElemListeS *liste = NULL;
    int point_dep_cycle = -1;
    int *visit = (int*)malloc(sizeof(int) * graphe->nbSom);

    int i, r = 0;
    int detect = 0;

    for(i = 0; i < graphe->nbSom; i++){
	if(tabDetection[i] == 0){
	    visit[i] = 0;
	}
	else {
	    visit[i] = -1;
	}
    }

    for(i = 0; i < graphe->nbSom; i++){
	if(visit[i] == -1){
	    r = i;
	    break;
	}
    }

    while( (r < graphe->nbSom) && !detect){
	detect = detect_cycle_impaire_descendant(graphe, r, visit, r, &point_dep_cycle, &liste);

	while( (r < graphe->nbSom) && visit[r]!=-1){
	    r++;
	}
    }
    if(point_dep_cycle != -1){
	liste = insert_ElemListeS(liste, graphe->tabS[r]);
	//	printf("%d<=",r);
    }

    free(visit);
    return liste;
}

int *ajout_vias_cycle_impair(Graphe *graphe){
    int *tabDetection = (int*)malloc(sizeof(int) * graphe->nbSom);
    int numSom;
    ElemListeS *liste = NULL;
    ElemListeS *tmp;
    
    for(numSom = 0; numSom < graphe->nbSom; numSom++){
	tabDetection[numSom] = -1;
    }
    liste = detect_cycle_impaire(graphe, tabDetection);
    while(liste){
	tmp = liste;
	while(tmp){
	    if(tmp->elem->SouP == 1){
		tabDetection[tmp->elem->num] = 0;
		break;
	    }
	    tmp = tmp->suiv;
	}
	
	detruire_ListeS(liste);
	liste = detect_cycle_impaire(graphe, tabDetection);
    }

    return tabDetection;
}
