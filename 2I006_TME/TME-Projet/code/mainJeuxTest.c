#include <stdio.h>
#include <stdlib.h>
#include "structure.h"
#include "fonction.h"
#include "fonctionFile.h"
#include "graphe.h"
#include "resolution.h"

#define NOM_SAVE_INTER "netlistIntersection.txt"
#define NOM_SAVE_NETLIST "outputNetlist.txt"
#define NOM_SAVE_PS "netlistPostscript.ps"
#define NOM_SAVE_GRAPHE_PS "graphePostscript.ps"
#define NOM_SAVE_NETLIST_SOLUTION_DEUX_FACE_PS "netlistSolutionDesDeuxFace.ps"
#define NOM_SAVE_NETLIST_SOLUTION_BICOLORE_PS "netlistSolutionBicolore.ps"
int main(){
    int methode;
    int instance;
    Netlist *netlist;
    
    printf("0: test6.net\n");
    printf("1: testInstance.net\n");
    printf("2: alea0030_030_10_088.net\n");
    printf("3: alea0030_030_90_007.net\n");
    printf("4: alea0100_050_10_097.net\n");
    printf("5: alea0100_080_90_024.net\n");
    printf("6: alea0300_300_10_044.net\n");
    printf("7: testInstanceVia.net\n");
    printf("Entre le numero de Netlist qu on veut tester:");
    scanf(" %d", &instance);
    switch (instance){
    case 0:
	netlist = netlistFromFile("Instance_Netlist/test6.net");
	break;
    case 1 :
	netlist = netlistFromFile("Instance_Netlist/testInstance.net");
	break;
    case 2 :
	netlist = netlistFromFile("Instance_Netlist/alea0030_030_10_088.net");
	break;
    case 3 :
	netlist = netlistFromFile("Instance_Netlist/alea0030_030_90_007.net");
	break;
    case 4 :
	netlist = netlistFromFile("Instance_Netlist/alea0100_050_10_097.net");
	break;
    case 5 :
	netlist = netlistFromFile("Instance_Netlist/alea0100_080_90_024.net");
	break;
    case 6 :
	netlist = netlistFromFile("Instance_Netlist/alea0300_300_10_044.net");
	break;
    case 7 :
	netlist = netlistFromFile("Instance_Netlist/testInstanceVia.net");
    }

    printf("nb seg %d\n", countNbSegmentNetlist(netlist)); 
    
    printf("0: Methode intersect naif\n");
    printf("1: Methode intersect balayage\n");
    printf("Entre le numero de methode qu on veut utiliser:");
    scanf(" %d",&methode);
    printf(">====================================<\n");


 

    printf("Le netlist lu est ecrit dans le fichier: %s\n",NOM_SAVE_NETLIST);
    ecrireNetist(netlist, NOM_SAVE_NETLIST);

    printf("Le netlist lu en image est dans le fichier: %s\n",NOM_SAVE_PS);
    visuNetlist(netlist, NOM_SAVE_PS);

    switch (methode){
    case 0 :
	intersect_naif(netlist);
	break;
    case 1 :
	intersect_balayage(netlist);
	break;
    }

    printf("Les sextuplets des intersections sont dans le fichier: %s\n", NOM_SAVE_INTER);
    sauvegardeIntersection(netlist, NOM_SAVE_INTER);

    printf("Creation de graphe\n");
    Graphe* graphe = creer_Graphe(netlist, NOM_SAVE_INTER);
    printf("Nombre de sommet:%d arete:%d\n",graphe->nbSom, graphe->nbArc);
    
    printf("Le graphe produit est dans le fichier: %s\n", NOM_SAVE_GRAPHE_PS);    
    visuGraphe(graphe, netlist, NOM_SAVE_GRAPHE_PS);

    int *tabSolution = via_deux_face(graphe);
    int nbVia = visuNetlistSolution(graphe, tabSolution, netlist, NOM_SAVE_NETLIST_SOLUTION_DEUX_FACE_PS);
    printf("Le graphe solution par la methode des deux face est dans le fichier: %s\n",NOM_SAVE_NETLIST_SOLUTION_DEUX_FACE_PS);
    printf("Nombre de Via par la methode des deux face: %d\n",nbVia);
    free(tabSolution);
    
    int *tabDetection = ajout_vias_cycle_impair(graphe);
    tabSolution = bicolore(graphe, tabDetection);
    nbVia = visuNetlistSolution(graphe, tabSolution, netlist, NOM_SAVE_NETLIST_SOLUTION_BICOLORE_PS);
    printf("Le graphe solution par la methode bicolore est dans le fichier: %s\n",NOM_SAVE_NETLIST_SOLUTION_BICOLORE_PS);
    printf("Nombre de Via par la methode bicolore: %d\n",nbVia);

    free(tabDetection);
    free(tabSolution);
    
    return 0;
}
