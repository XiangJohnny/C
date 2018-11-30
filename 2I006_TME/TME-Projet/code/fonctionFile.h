#ifndef FONCTIONFILE_H
#define FONCTIONFILE_H

#include "graphe.h"

//creer un Netliste a partir d un fichier et le retourne, en cas de fail retourne NULL
Netlist * netlistFromFile(char *filename);

//ecrit dans un fichier le Netlist passe en argument
void ecrireNetist(Netlist *netlist, char *filename);

//ecrir le netlist dans un fichier au format postscript.ps pour desiner l ensemble de reseau
void visuNetlist(Netlist *netlist, char *nom);

//ecrit les couples de segment qui s intersecte
void sauvegardeIntersection(Netlist *netlist, char *file);

//ecrit le graphe dans un fichier au format postscript.ps pour desiner le graphe
void visuGraphe(Graphe *graphe, Netlist *netlist, char *filename);

//ecrit le netlist solution dans un fichier au format postscript.ps pour desiner le graphe solution
int visuNetlistSolution(Graphe* graphe, int *tabSolution, Netlist *netlist, char *nom);
#endif
