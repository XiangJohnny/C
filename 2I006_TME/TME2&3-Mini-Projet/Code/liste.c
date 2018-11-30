#include <stdio.h>
#include "liste.h"
#include <string.h>
#include "entree_sortie.h"


int insererEnTete(ListeLivre **liste, Livre *livre){
    ListeLivre *tmp = (ListeLivre*)malloc(sizeof(ListeLivre));

    if(tmp == NULL){
	return 0;
    }
    tmp->livre = livre;
    tmp->suiv = NULL;

    if(*liste != NULL){
	tmp->suiv = *liste;
    }

    *liste = tmp;
    return 1;
}

int supprimerUnLivre(ListeLivre **liste, Livre *livre){
    ListeLivre *tmp;
    if( (*liste)->livre == livre){
	*liste = (*liste)->suiv;
	free(livre);
	return 1;
    }

    tmp = *liste;
    while(tmp->suiv){
	if(tmp->suiv->livre == livre){
	    tmp->suiv = tmp->suiv->suiv;
	    free(livre);
	    return 1;
	}
	tmp = tmp->suiv;
    }

    return 0;
}

Livre *rechercheParNum(ListeLivre *liste, int num){
    
    while(liste){
	if(liste->livre->num == num){
	    return liste->livre;
	}
	liste = liste->suiv;
    }
    return NULL;
}

Livre *rechercheParTitre(ListeLivre *liste, char *titre){

    while(liste){
	if(strcmp(liste->livre->titre, titre) == 0){
	    return liste->livre;
	}
	liste = liste->suiv;
    }

    return NULL;
}

ListeLivre *livreDeMemeAuteur(ListeLivre *liste, char *auteur){
    ListeLivre *livres = NULL;

    while(liste){
	if(strcmp(liste->livre->auteur, auteur) == 0){
	    insererEnTete(&livres, liste->livre);
	}
	liste = liste->suiv;
    }

    return livres;
}

ListeLivre *livrePlusDeDeux(ListeLivre *liste){
    ListeLivre *livres = NULL;
    ListeLivre *tmp;
    int b_double;
    while(liste != NULL){
	b_double = 0;
	if(rechercheParTitre(livres, liste->livre->titre) == NULL){
	    tmp = liste->suiv;
	    while( (tmp != NULL) && (b_double == 0) ){
		if( strcmp(liste->livre->titre,tmp->livre->titre) == 0 && strcmp(liste->livre->auteur,tmp->livre->auteur) == 0  ){
		    b_double = 1;
		    insererEnTete(&livres, liste->livre);
		}
		tmp = tmp->suiv;
	    }
	}
	liste = liste->suiv;
    }
    
    return livres;
}

void afficherListeLivre(ListeLivre *liste){
    while(liste){
	printf("%d %s %s\n",liste->livre->num,liste->livre->titre, liste->livre->auteur);
	liste = liste->suiv;
    }
}
