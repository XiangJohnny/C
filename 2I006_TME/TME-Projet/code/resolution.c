#include "stdio.h"
#include "stdlib.h"
#include "resolution.h"

int *via_deux_face(Graphe* graphe){
    if(graphe == NULL){
	printf("Graphe en parametre null\n");
	return NULL;
    }
    int bool_H, bool_V;
    int numSom;
    int nbSom = graphe->nbSom;
    int *tabSolution = (int*)malloc(sizeof(int) * nbSom);//tableau de taille nombre de sommet
    Sommet **tabS = graphe->tabS;
    Sommet *som, *voisin;
    Segment *seg;
    ElemListeA* lincid;
    Arc *arcincid;
    
    for(numSom = 0; numSom < nbSom; numSom++){ //pour tous les sommets
	som = tabS[numSom];
	if(som->SouP == 0){ //sommet pour un segment
	    seg = (Segment*)som->elem;
	    if(seg->HouV == 0){ //segment horizontal
		//	printf("sommet %d horizontal\n",numSom);
		tabSolution[numSom] = 1; //face 1 <=> A
	    }
	    else {//segment vertical
		//	printf("sommet %d vertical\n",numSom);
		tabSolution[numSom] = 2; //face 2 <=> B
	    }
	}
	else { //sommet pour un point
	    bool_H = 0; //ce point est il un point d un segment horizontal
	    bool_V = 0; //ce point est il un point d un segment vertical
	    lincid = som->Lincid;
	    while(lincid){
		arcincid = lincid->elem;
		if(arcincid->som1 == numSom){
		    voisin = tabS[arcincid->som2];
		}
		else {
		    voisin = tabS[arcincid->som1];
		}

		if(((Segment*)voisin->elem)->HouV == 0){ //un point d horizontal
		    bool_H = 1;
		}
		else { //un point de vertical
		    bool_V = 1;
		}

		if(bool_V == 1 && bool_H == 1){ //ce point est de horiz et de verti
		    //	    printf("sommet %d point Via\n",numSom);
		    tabSolution[numSom] = 0; //ce point est un VIA
		    break;
		}

		lincid = lincid->suiv;
	    }

	    if(bool_H == 0 || bool_V == 0){ //dans le cas contraire
		//	printf("sommet %d point non Via\n",numSom);
		tabSolution[numSom] = 1; //ce point nest pas un VIA
	    }
	}
    }
    
    return tabSolution;
}

void coloration(Graphe *graphe, int *tabSolution, int som, int face){
    if(tabSolution[som] != -1){
	return;
    }
    int suiv;
    ElemListeA *lincid = graphe->tabS[som]->Lincid;
    tabSolution[som] = face;

    if(face == 1){
	face = 2;
    }
    else {
	face = 1;
    }

    while(lincid){
	if(lincid->elem->som1 == som){
	    suiv = lincid->elem->som2;
	}
	else {
	    suiv = lincid->elem->som1;
	}
	
	coloration(graphe, tabSolution, suiv, face);
	lincid = lincid->suiv;
    }
    
}

int *bicolore(Graphe *graphe, int *tabDetection){
    int numSom;
    int *tabSolution = malloc(sizeof(int) * graphe->nbSom);
    for(numSom = 0; numSom < graphe->nbSom; numSom++){
	tabSolution[numSom] = tabDetection[numSom];
    }
    
    for(numSom = 0; numSom < graphe->nbSom; numSom++){
	coloration(graphe, tabSolution, numSom, 1);
    }

    return tabSolution;
}
