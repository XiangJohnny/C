#include <stdio.h>
#include <time.h>
#include <string.h>
#include "entree_sortie.h"
#include "tabHachage.h"
#include "liste.h"

#define TAILLE_BUFFER 50
#define TAILLE_TAB_HACH 100
#define PAS 10000

int main(){
    int n, i;
    time_t time1, time2;
    float time;
    char buff[TAILLE_BUFFER];
    FILE *file = fopen("GdeBiblio.txt", "r");
    FILE *stock = fopen("resultat.txt", "w");
    
    ListeLivre *liste = NULL;
    Livre *livre;

    TabHachage *tab = initTabHachage(TAILLE_TAB_HACH);

    for(i = PAS; i < 50000+PAS; i+=PAS){	
	for(n = 0; n < PAS; n++){
	    livre = malloc(sizeof(Livre));
	    livre->num = GetEntier(file);	
	    GetChaine(file, TAILLE_BUFFER, buff);
	    livre->titre = strdup(buff);
	    GetChaine(file, TAILLE_BUFFER, buff);
	    livre->auteur = strdup(buff);

	    insererEnTete(&liste, livre);
	    insererDansTabHach(tab, livre);
	}
	time1 = clock();
	livrePlusDeDeux(liste);
	time2 = clock();
	time =  (time2 - time1)/(float)CLOCKS_PER_SEC;
	printf("liste livre en double sur %d: %f sec\n",i,time);
	fprintf(stock, "%10f ", time);
	
	time1 = clock();
	livrePlusDeDeuxDansHach(tab);
	time2 = clock();
	time =  (time2 - time1)/(float)CLOCKS_PER_SEC;
	printf("hachage livre en double sur %d: %f sec\n\n",i, time);
	fprintf(stock, "%10f %10d\n", time,i);

    }
    //    afficherNbDansChaqueCase(tab);
    fclose(file);
    return 0;
}
