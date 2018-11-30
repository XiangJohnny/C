#include <stdio.h>
#include "entree_sortie.h"
#include "structure.h"
#include "fonction.h"
#include "fonctionFile.h"
#include "graphe.h"

Netlist * netlistFromFile(char *filename){
    Reseau* reseau;
    Segment* segment;
    Point* point;
    FILE* file = fopen(filename, "r");
    Netlist *netlist;
    int nbRes = GetEntier(file);//lire le nombre de reseau
    int numRes, nbPt, nbSeg;
    int x,y;
    int i, n;
    netlist = creerNetlist(nbRes);
    if(netlist == NULL) printf("creation netlist null\n");

    //    while(!feof(file)){//tanqu on n a pas atteint la fin du fichier
	for(n = 0; n < nbRes; n++){
	numRes = GetEntier(file);//lire le numero du reseau
	nbPt = GetEntier(file);//lire le nombre de point de ce reseau
	nbSeg = GetEntier(file);//lire le nombre de segment de ce reseau
	SkipLine(file);
	
	reseau = creerReseau(numRes, nbPt);
	if(reseau == NULL) printf("creation reseau null\n");
	netlist->T_Res[numRes] = reseau;

       	while(nbPt--){//pour les nbPt de points
	    i = GetEntier(file);//lire le numero du point
	    x = GetReel(file);//lire le coordonnee x
	    y = GetReel(file);//lire le coordonnee y
	    SkipLine(file);
	    
	    point = creerPoint(numRes, x, y);
	    if(point == NULL) printf("creation point null\n");
		
	    reseau->T_Pt[i] = point;// la place du point dans le tableau est le numero du point
	}
	
	while(nbSeg--){//pour les nbSeg de segments
	    x = GetEntier(file);//lire le numero du premier point
	    y = GetEntier(file);//lire le numero du seconde point
	    SkipLine(file);
	    
	    segment = creerSegment(numRes, x, y, reseau->T_Pt[x], reseau->T_Pt[y]);
	    if(segment == NULL) printf("creation segment null\n");
	    
	    //ajoute au liste incident de segement pour les deux points
	    reseau->T_Pt[x]->Lincid = insertSegment(reseau->T_Pt[x]->Lincid, segment);
	    reseau->T_Pt[y]->Lincid = insertSegment(reseau->T_Pt[y]->Lincid, segment);
	}
    }
    
    fclose(file);
    return netlist;
}

void ecrireNetist(Netlist *netlist, char *filename){
    FILE *file = fopen(filename, "w");
    Reseau *reseau;
    Point *point;
    Cell_segment *segment_ecrit = NULL, *segment_parcourt;
    int nbres, nbpt, nbseg;
    fprintf(file, "%d\n", netlist->NbRes);
    
    for(nbres = 0; nbres < netlist->NbRes; nbres++){//pour tous les reseau dans netlist
	reseau = netlist->T_Res[nbres];
	nbseg = countNbSegmentReseau(reseau);//calcule le nb de segment du reseau
	fprintf(file, "%d %d %d\n", reseau->NumRes, reseau->NbPt, nbseg);

	for(nbpt = 0; nbpt < reseau->NbPt; nbpt++){//pour tous les points
	    point = reseau->T_Pt[nbpt];
	    fprintf(file, "  %d %f %f\n", nbpt, point->x, point->y);

	}

	for(nbpt = 0; nbpt < reseau->NbPt; nbpt++){//pour tous les points
	    segment_parcourt = reseau->T_Pt[nbpt]->Lincid;//on regarde les segments qui lui sont incident
	    
	    while(segment_parcourt){
		if(existeDansListSeg(segment_ecrit, segment_parcourt->seg) == 0){//si cette segment nest pas encore ecrit on l ecrit
		    fprintf(file, "  %d %d\n", segment_parcourt->seg->p1, segment_parcourt->seg->p2);
		    segment_ecrit = insertSegment(segment_ecrit, segment_parcourt->seg);//on ajoute cet segment au liste de segment deja ecrit
		}
		segment_parcourt = segment_parcourt->suiv;
	    }
	}
		    
	while(segment_ecrit){
	    segment_parcourt =segment_ecrit;
	    segment_ecrit = segment_ecrit->suiv;
	    free(segment_parcourt);
	}
	segment_ecrit = NULL;
    }
    fclose(file);	
}

void visuGraphe(Graphe *graphe, Netlist *netlist, char *filename){
    int numSom;
    int numArc;
    Sommet *som;
    Segment *seg;
    Point *point;
    Point *p1, *p2;
    FILE *file = fopen(filename, "w");

    int minx = 10000000, maxx = -1000000, miny = 10000000, maxy = -11111111;
    int abs, ord;
    int deltaX, deltaY;
    for(numSom = 0; numSom < graphe->nbSom; numSom++){
	som = graphe->tabS[numSom];
	if(som->SouP == 1){
	    point = (Point*)som->elem;
	    if(minx > point->x){
		minx = point->x;
	    }
	    if(maxx < point->x){
		maxx = point->x;
	    }
	    if(miny > point->y){
		miny = point->y;
	    }
	    if(maxy < point->y){
		maxy = point->y;
	    }
	}
    }

    minx -= 2;
    miny -= 2;
    deltaX = maxx - minx;
    deltaY = maxy - miny;
    
    for(numSom = 0; numSom < graphe->nbSom; numSom++){
	som = graphe->tabS[numSom];
	if(som->SouP == 0){ //pour un segment
	    seg = (Segment*)som->elem;
	    p1 = netlist->T_Res[seg->NumRes]->T_Pt[seg->p1];
	    p2 = netlist->T_Res[seg->NumRes]->T_Pt[seg->p2];
	    if(seg->HouV == 0){//segment horizontal
		abs = ((p1->x+p2->x)/2 - minx)/deltaX * 595;
		ord = (p1->y - miny)/deltaY * 777;
		fprintf(file, "%d %d 2.5 0 360 arc\n",abs, ord);
	    }
	    else {//segment vertical
		abs = (p1->x - minx)/deltaX * 595;
		ord = ((p1->y+p2->y)/2 - miny)/deltaY * 777;
		fprintf(file, "%d %d 2.5 0 360 arc\n",abs, ord);
	    }
	}
	else {//pour un point
	    point = (Point*)som->elem;
	    abs = (point->x - minx)/deltaX * 595;
	    ord = (point->y - miny)/deltaY * 777;
	    fprintf(file, "%d %d 2.5 0 360 arc\n",abs, ord);
	}
	fprintf(file, "fill\n");	
    }

    
    for(numArc = 0; numArc < graphe->nbArc; numArc++){
	som = graphe->tabS[graphe->tabA[numArc]->som1];
	if(som->SouP == 0){ //pour un segment
	    seg = (Segment*)som->elem;
	    p1 = netlist->T_Res[seg->NumRes]->T_Pt[seg->p1];
	    p2 = netlist->T_Res[seg->NumRes]->T_Pt[seg->p2];
	    if(seg->HouV == 0){//segment horizontal
		abs = ((p1->x+p2->x)/2 - minx)/deltaX * 595;
		ord = (p1->y - miny)/deltaY * 777;
		fprintf(file, "%d %d moveto\n",abs, ord);
	    }
	    else {//segment vertical
		abs = (p1->x - minx)/deltaX * 595;
		ord = ((p1->y+p2->y)/2 - miny)/deltaY * 777;
		fprintf(file, "%d %d moveto\n", abs, ord);
	    }
	}
	else {//pour un point
	    point = (Point*)som->elem;
	    abs = (point->x - minx)/deltaX * 595;
	    ord = (point->y - miny)/deltaY * 777;
	    fprintf(file, "%d %d 2.5 0 360 arc\n", abs, ord);
	}

	som = graphe->tabS[graphe->tabA[numArc]->som2];
	if(som->SouP == 0){ //pour un segment
	    seg = (Segment*)som->elem;
	    p1 = netlist->T_Res[seg->NumRes]->T_Pt[seg->p1];
	    p2 = netlist->T_Res[seg->NumRes]->T_Pt[seg->p2];
	    if(seg->HouV == 0){//segment horizontal
		abs = ((p1->x+p2->x)/2 - minx)/deltaX * 595;
		ord = (p1->y - miny)/deltaY * 777;
		fprintf(file, "%d %d lineto\n", abs, ord);
	    }
	    else {//segment vertical
		abs = (p1->x - minx)/deltaX * 595;
		ord = ((p1->y+p2->y)/2 - miny)/deltaY * 777;
		fprintf(file, "%d %d lineto\n", abs, ord);
	    }
	}
	else {//pour un extremite
	    point = (Point*)som->elem;
	    abs = (point->x - minx)/deltaX * 595;
	    ord = (point->y - miny)/deltaY * 777;
	    fprintf(file, "%d %d lineto\n", abs, ord);
	}

	fprintf(file, "stroke\n");
	}

    fclose(file);
}


void visuNetlist(Netlist *netlist, char *nom){
    FILE *file = fopen(nom, "w");
    Reseau *reseau;
    Point *point;
    Cell_segment *segment_ecrit = NULL, *segment_parcourt;
    int nbres, nbpt;

    int abs, ord;
    int minx, miny,maxx, maxy;
    int deltaX, deltaY;
    minx = netlist->T_Res[0]->T_Pt[0]->x;
    maxx = netlist->T_Res[0]->T_Pt[0]->x;
    miny = netlist->T_Res[0]->T_Pt[0]->y;
    maxy = netlist->T_Res[0]->T_Pt[0]->y;
    // chercher le point max et min
    for(nbres = 0; nbres < netlist->NbRes; nbres++){//pour tous les reseau dans netlist
	reseau = netlist->T_Res[nbres];

	for(nbpt = 0; nbpt < reseau->NbPt; nbpt++){//pour tous les points
	    point = reseau->T_Pt[nbpt];
	    if(minx > point->x){
		minx = point->y;
	    }
	    if(miny > point->y){
		miny = point->y;
	    }
	    if(maxx < point->x){
		maxx = point->x;
	    }
	    if(maxy < point->y){
		maxy = point->y;
	    }
	}
    }

    minx -= 2;
    miny -= 2;
    deltaX = maxx - minx;
    deltaY = maxy - miny;
    
    for(nbres = 0; nbres < netlist->NbRes; nbres++){//pour tous les reseau dans netlist
	reseau = netlist->T_Res[nbres];

	for(nbpt = 0; nbpt < reseau->NbPt; nbpt++){//pour tous les points
	    point = reseau->T_Pt[nbpt];
	    abs = (point->x - minx)/deltaX * 595;
	    ord = (point->y - miny)/deltaY * 777;
	    fprintf(file, "%d %d 2.5 0 360 arc\n",abs, ord);
	    //	    fprintf(file, "%d %d 2.5 0 360 arc\n",(int)point->x, (int)point->y);
	    fprintf(file, "fill\n");
	}

	for(nbpt = 0; nbpt < reseau->NbPt; nbpt++){//pour tous les points
	    segment_parcourt = reseau->T_Pt[nbpt]->Lincid;//on regarde les segments qui lui sont incident
	    
	    while(segment_parcourt){
		if(existeDansListSeg(segment_ecrit, segment_parcourt->seg) == 0){//si cette segment nest pas encore ecrit on l ecrit
		    abs = (reseau->T_Pt[segment_parcourt->seg->p1]->x - minx)/deltaX * 595;
		    ord = (reseau->T_Pt[segment_parcourt->seg->p1]->y - miny)/deltaY * 777;
		    fprintf(file, "%d %d moveto\n", abs, ord);
		    abs = (reseau->T_Pt[segment_parcourt->seg->p2]->x - minx)/deltaX * 595;
		    ord = (reseau->T_Pt[segment_parcourt->seg->p2]->y - miny)/deltaY * 777;
		    
		    fprintf(file, "%d %d lineto\n", abs, ord);
		    // fprintf(file, "%d %d moveto\n", (int)reseau->T_Pt[segment_parcourt->seg->p1]->x, (int)reseau->T_Pt[segment_parcourt->seg->p1]->y);
		    // fprintf(file, "%d %d lineto\n", (int)reseau->T_Pt[segment_parcourt->seg->p2]->x, (int)reseau->T_Pt[segment_parcourt->seg->p2]->y);
		    fprintf(file, "stroke\n");
		    segment_ecrit = insertSegment(segment_ecrit, segment_parcourt->seg);//on ajoute cet segment au liste de segment deja ecrit
		}
		segment_parcourt = segment_parcourt->suiv;
	    }
	}
		    
	while(segment_ecrit){
	    segment_parcourt = segment_ecrit;
	    segment_ecrit = segment_ecrit->suiv;
	    free(segment_parcourt);
	}
	segment_ecrit = NULL;
    }
    fclose(file);
}

int visuNetlistSolution(Graphe* graphe, int *tabSolution, Netlist *netlist, char *nom){
    FILE *file = fopen(nom, "w");
    Reseau *reseau;
    Point *point;
    Segment *seg;
    Sommet *som;

    int numSom, nbVia = 0;
    int abs, ord;
    int minx, miny,maxx, maxy;
    int deltaX, deltaY;
    minx = netlist->T_Res[0]->T_Pt[0]->x;
    maxx = netlist->T_Res[0]->T_Pt[0]->x;
    miny = netlist->T_Res[0]->T_Pt[0]->y;
    maxy = netlist->T_Res[0]->T_Pt[0]->y;
    // chercher le point max et min
    for(numSom = 0; numSom < graphe->nbSom; numSom++){
	som = graphe->tabS[numSom];
	if(som->SouP == 1){
	    point = (Point*)som->elem;
	    if(minx > point->x){
		minx = point->x;
	    }
	    if(maxx < point->x){
		maxx = point->x;
	    }
	    if(miny > point->y){
		miny = point->y;
	    }
	    if(maxy < point->y){
		maxy = point->y;
	    }
	}
    }

    minx -= 2;
    miny -= 2;
    deltaX = maxx - minx;
    deltaY = maxy - miny;
    
    for(numSom = 0; numSom < graphe->nbSom; numSom++){
	som = graphe->tabS[numSom];

	if(som->SouP == 0){
	    seg = (Segment*)som->elem;
	    reseau = netlist->T_Res[seg->NumRes];
	    if(tabSolution[numSom] == 1){ //face A<=>1 rouge
		fprintf(file, "1 0 0 setrgbcolor\n");
	    }
	    else { //face B<=>2 bleu
		fprintf(file, "0 0 1 setrgbcolor\n");
	    }
	    
	    abs = (reseau->T_Pt[seg->p1]->x - minx)/deltaX * 595;
	    ord = (reseau->T_Pt[seg->p1]->y - miny)/deltaY * 777;
	    fprintf(file, "%d %d moveto\n", abs, ord);
	    abs = (reseau->T_Pt[seg->p2]->x - minx)/deltaX * 595;
	    ord = (reseau->T_Pt[seg->p2]->y - miny)/deltaY * 777;
		    
	    fprintf(file, "%d %d lineto\n", abs, ord);
	    fprintf(file, "stroke\n");

	}
	else {
	    if(tabSolution[numSom] == 0){
		nbVia++;
		point = (Point*)som->elem;
		abs = (point->x - minx)/deltaX * 595;
		ord = (point->y - miny)/deltaY * 777;
		fprintf(file, "%d %d 2.5 0 360 arc\n",abs, ord);
		fprintf(file, "fill\n");
	    }   
	}
    }
    fclose(file);

    return nbVia;
}

void sauvegardeIntersection(Netlist *netlist, char *filename){
    int numres, numpt;
    Cell_segment *segDejaEcrit = NULL;
    Cell_segment *segParcourtLincid;
    Cell_segment *segParcourtLintersec;
    Segment* segIncident, *segIntersec;
    FILE *file = fopen(filename, "w");
    Point *point;

    // fprintf(file, "%d\n",netlist->NbInter);//le nombre d intersection
    
    for(numres = 0; numres < netlist->NbRes; numres++){//pour tous les reseaus
	for(numpt = 0; numpt < netlist->T_Res[numres]->NbPt; numpt++){//pour tous les points de ce reseau
	    point = netlist->T_Res[numres]->T_Pt[numpt];//un point des points
	    segParcourtLincid = point->Lincid;//la liste des segments incident de ce point
	    
	    while(segParcourtLincid){//tanque la liste des segments nest pas vide
		segIncident = segParcourtLincid->seg;
		if(existeDansListSeg(segDejaEcrit, segIncident) == 0){
		    //si cette segment nest pas encore parcouru. cette verification est du qu un segment est represente deux fois qui sont les deux points
		    segDejaEcrit = insertSegment(segDejaEcrit, segIncident);//on marque le segment incident comme segment deja ecrit
		    segParcourtLintersec = segIncident->Lintersec;//la liste des segments en intersection avec ce segment
		
		    while(segParcourtLintersec){//tanque la liste des segments en intersection nest pas vide
			segIntersec = segParcourtLintersec->seg;
			if(existeDansListSeg(segDejaEcrit, segIntersec) == 0){//si on na pas encore ecrit l intersection entre ces deux segments
			    //segDejaEcrit = insertSegment(segDejaEcrit, segIntersec);
			    fprintf(file, "%d %d %d %d %d %d\n", segIncident->NumRes, segIncident->p1, segIncident->p2,
				    segIntersec->NumRes, segIntersec->p1, segIntersec->p2);//on l ecrit
			}
			segParcourtLintersec = segParcourtLintersec->suiv;//segment d intersection suivant
		    }
		}
		segParcourtLincid = segParcourtLincid->suiv;//segment incident suivant
	    }
	}
    }
    detruireCell_segment(segDejaEcrit);
    fclose(file);
}


