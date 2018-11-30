#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"Mini-Projet.h"

int main() {
    //{ A = 0, T = 1, G = 2, C = 3}; 
    // 53000  45.97
   
    int n, m;
    int choix;
    int distance;
    float t1, t2;
    float temps;
    char *a;
    char *b;

    srand(time(NULL));
    
    printf("\t1-calculer la distance minimum avec la fonction recursive\n");
    printf("\t2-calculer la distance minimum avec la fonction iterative\n");
    printf("\t3-calculer la distance minimum avec les deux fonctions recursive et iterative\n");
    
    do {
    scanf(" %d",&choix);
    }while(choix > 3 || choix < 1);

    printf("saisir la taille de sequence a:");
    scanf(" %d",&n);
    printf("\nsaisir la taille de sequence b:");
    scanf(" %d",&m);
    printf("\n");

    a = malloc(sizeof(char) * n);
    b = malloc(sizeof(char) * m);
  
    
    seqAleatoire(a, n);
    seqAleatoire(b, m);
    
    switch (choix) {

    case 1:
	t1 = clock();
	distance = DistanceMinRec(a, b, n, m);
	t2 = clock();
	temps = (t2-t1)/CLOCKS_PER_SEC;
	printf("la distanceminrec est : %d avec le temps:%f \n",distance, temps);
	break;
    case 2:
	t1 = clock();
	distance = DistanceMinIter(a, b, n, m);
	t2 = clock();
	temps = (t2-t1)/CLOCKS_PER_SEC;
	printf("la distanceminiter est : %d avec le temps:%f \n",distance, temps);
	break;
    case 3:
	t1 = clock();
	distance = DistanceMinRec(a, b, n, m);
	t2 = clock();
	temps = (t2-t1)/CLOCKS_PER_SEC;
	printf("la distanceminrec est : %d avec le temps:%f \n",distance, temps);
	
	t1 = clock();
	distance = DistanceMinIter(a, b, n, m);
	t2 = clock();
	temps = (t2-t1)/CLOCKS_PER_SEC;
	printf("la distanceminiter est : %d avec le temps:%f \n",distance, temps);
	break;
    }
    
    return 0;
}
