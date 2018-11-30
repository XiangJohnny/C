#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"Mini-Projet.h"

int main() {
    //{ A = 0, T = 1, G = 2, C = 3}; 
    // int i;
   
    float t1, t2;
    float temps;
 
    int alpha,beta;
   
    int p;
    
    printf("saisir alpha:\n");
    scanf(" %d",&alpha);
    printf("saisir beta:\n");
    scanf(" %d",&beta);
    printf("saisir P:\n");
    scanf(" %d",&p);
  

    char a[alpha*p];
    char b[beta*p];
    
    srand(time(NULL));
    seqAleatoire(a, alpha*p);
    seqAleatoire(b, beta*p);
   

    t1 = clock();
    DistanceMinIter(a, b, alpha * p, beta * p);
    t2 = clock();
    temps = (t2-t1)/CLOCKS_PER_SEC;

    printf("temps pour alpha:%d beta%d et P:%d est :%f\n",alpha,beta,p,temps);
    printf("la constante: %f\n",temps/(alpha*beta));
    return 0;
}
