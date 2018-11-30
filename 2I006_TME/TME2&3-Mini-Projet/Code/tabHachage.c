#include<stdio.h>
#include"entree_sortie.h"
#include<math.h>
#include<string.h>
#include"tabHachage.h"
#include"liste.h"

TabHachage *initTabHachage(int m){
    TabHachage *tab = malloc(sizeof(TabHachage));
    tab->m = m;
    tab->T = calloc(m, sizeof(Cell *));
    tab->n = 0;
    return tab;
}

int fonctionClef(char *nom){
    int i = 0;
    int somme = 0;
    while(nom[i] != '\0'){
	somme += nom[i];
	i++;
    }
    return somme;
}

int fonctionHachage(int clef, int m){
    double a = (sqrt(5)-1)/2;
    return ((int)(clef*a))%m;
}

int insererDansTabHach(TabHachage *tab, Livre *data){
    if(tab == NULL || data == NULL){
	return 0;
    }
    Cell *cell = malloc(sizeof(Cell));
    cell->data = data;
    cell->clef = fonctionClef(data->auteur);
    int hash = fonctionHachage(cell->clef, tab->m);

    tab->n++;
    cell->suiv = tab->T[hash];
    tab->T[hash] = cell;
    return 1;
}

int supprimerUnLivreDansHach(TabHachage *tab, Livre *livre){
    int hash = fonctionHachage(fonctionClef(livre->auteur),tab->m);
    Cell *cell = tab->T[hash];
    Cell *tmp;

    if(livre == NULL || tab == NULL){
	return 0;
    }	
    if(tab->T[hash]->data == livre){
	tmp = tab->T[hash];
	tab->T[hash] = tab->T[hash]->suiv;
	free(livre);
	free(tmp);
	tab->n--;
	return 1;
    }
 
    while(cell->suiv != NULL){
	if(cell->suiv->data == livre){
	    tmp = cell->suiv;
	    cell->suiv = cell->suiv->suiv;
	    free(livre);
	    free(tmp);
	    tab->n--;
	    return 1;
	}
	cell = cell->suiv;
    }

    return 0;
}

Livre *rechercheParAuteurDansHach(TabHachage *tab, char *auteur){
    int hash = fonctionHachage(fonctionClef(auteur), tab->m);
    Cell *cell = tab->T[hash];
    while(cell != NULL){
	if(strcmp(cell->data->auteur, auteur) == 0){
	    return cell->data;
	}
	cell = cell->suiv;
    }
    return NULL;
}

TabHachage *livreDeMemeAuteurDansHach(TabHachage *tab, char *auteur){
    TabHachage * resu = initTabHachage(tab->m);
    int hash = fonctionHachage(fonctionClef(auteur), tab->m);
    Cell *cell = tab->T[hash];
    while(cell != NULL){
	if(strcmp(cell->data->auteur, auteur) == 0){
	    insererDansTabHach(resu, cell->data);
	}
	cell = cell->suiv;
    }
    return resu;
}

TabHachage *livrePlusDeDeuxDansHach(TabHachage *tab){
    TabHachage *resu = initTabHachage(tab->m);
    int i;
    int b_double;
    Cell *cell1, *cell2;
    for(i = 0; i < tab->m; i++){
	cell1 = tab->T[i];
	while(cell1){
	    b_double = 0;
	    cell2 = cell1->suiv;
	    if(rechercheParAuteurDansHach(resu, cell1->data->auteur) == NULL){
		while(cell2 != NULL && b_double == 0){
		    if(strcmp(cell1->data->auteur, cell2->data->auteur) == 0 && strcmp(cell1->data->titre, cell2->data->titre) == 0){
			b_double = 1;
			insererDansTabHach(resu, cell1->data);
		    }
		    cell2 = cell2->suiv;
		}
	    }
	    cell1 = cell1->suiv;
	}
    }
    return resu;
}

void afficherTabHachage(TabHachage *tab){
    int i;
    Cell *cell;
    for(i = 0; i < tab->m; i++){
	cell = tab->T[i];
	while(cell){
	    printf("%d %s %s\n",cell->data->num,cell->data->titre, cell->data->auteur);
	    cell = cell->suiv;
	}
    }
}

void afficherNbDansChaqueCase(TabHachage *tab){
    int i;
    int somme;
    Cell *cell;
    for(i = 0; i < tab->m; i++){
	somme = 0;
	cell = tab->T[i];
	while(cell){
	    somme++;
	    cell = cell->suiv;
	}
	printf("%3d\n",somme);
    }
}
