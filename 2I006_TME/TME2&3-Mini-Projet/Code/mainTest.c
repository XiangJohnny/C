#include <stdio.h>
#include "entree_sortie.h"
#include "liste.h"
#include "tabHachage.h"
#include <string.h>
#include "time.h"

#define NBLIVREALIRE 10
#define NBLIVRELIREENPLUS 50000
#define AUTEUR "yhidd"

int main(){
    time_t time1, time2;
    int i, taille = 50;
    char buff[taille];
    FILE *file = fopen("GdeBiblio.txt","r");
    Livre *livre;
    ListeLivre *liste = NULL;

    time1 = clock();
    for(i = 0; i < NBLIVREALIRE; i++){
	livre = malloc(sizeof(Livre));
	livre->num = GetEntier(file);
	
	GetChaine(file, taille, buff);
	livre->titre = strdup(buff);
	GetChaine(file, taille, buff);
	livre->auteur = strdup(buff);
	insererEnTete(&liste, livre);

    }
    printf("afficher les %d premieres livres lu:\n",NBLIVREALIRE);
    afficherListeLivre(liste);

    printf("\nsupprimer le livre numero 6 et le livre de titre SOFS\n");
    supprimerUnLivre(&liste, rechercheParNum(liste, 6));
    supprimerUnLivre(&liste, rechercheParTitre(liste, "SOFS"));
    afficherListeLivre(liste);

    for(i = 0; i < NBLIVRELIREENPLUS; i++){
	livre = malloc(sizeof(Livre));
	livre->num = GetEntier(file);
	
	GetChaine(file, taille, buff);
	livre->titre = strdup(buff);
	GetChaine(file, taille, buff);
	livre->auteur = strdup(buff);
	insererEnTete(&liste, livre);

    }

    printf("\nles livre a pour de meme auteur %s sont: \n", AUTEUR);
    afficherListeLivre(livreDeMemeAuteur(liste, AUTEUR));
    printf("\nles livres en plus de deux sont:\n");
    afficherListeLivre(livrePlusDeDeux(liste));
    time2 = clock();
    printf("temps d execution est de %ld sec\n", (time2 - time1)/CLOCKS_PER_SEC);
    
    fclose(file);
    // pour le tableau de hachage
    printf("-------------------------------------------------\n");
    
    file = fopen("GdeBiblio.txt","r");

    TabHachage * tab = initTabHachage(100);

    time1 = clock();
    for(i = 0; i < NBLIVREALIRE; i++){
	livre = malloc(sizeof(Livre));
	livre->num = GetEntier(file);
	
	GetChaine(file, taille, buff);
	livre->titre = strdup(buff);
	GetChaine(file, taille, buff);
	livre->auteur = strdup(buff);
	insererDansTabHach(tab, livre);
    }
    printf("afficher les %d premieres livres lu:\n",NBLIVREALIRE);
    afficherTabHachage(tab);

    printf("\nsupprimer le livre de l auteur adrr\n");
    supprimerUnLivreDansHach(tab, rechercheParAuteurDansHach(tab, "adrr"));
    afficherTabHachage(tab);

    for(i = 0; i < NBLIVRELIREENPLUS; i++){
	livre = malloc(sizeof(Livre));
	livre->num = GetEntier(file);
	
	GetChaine(file, taille, buff);
	livre->titre = strdup(buff);
	GetChaine(file, taille, buff);
	livre->auteur = strdup(buff);
	insererDansTabHach(tab, livre);
    }
    
    printf("\nles livre a pour de meme auteur %s sont: \n", AUTEUR);
    afficherTabHachage(livreDeMemeAuteurDansHach(tab, "AUTEUR"));
    printf("\nles livres en plus de deux sont:\n");
    afficherTabHachage(livrePlusDeDeuxDansHach(tab));
    time2 = clock();
    printf("temps d execution est de %ld sec\n", (time2 - time1)/CLOCKS_PER_SEC);

    fclose(file);
    return 0;
}
