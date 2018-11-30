#include <stdio.h>
#include <stdlib.h>
#include "structure_AVL.h"

AVL* creeAVL_noeud(int ord, Segment* seg, AVL* fg, AVL* fd){
    AVL* n = (AVL*)malloc(sizeof(AVL));
    if(n == NULL){
	return NULL;
    }
    n->ord = ord;
    n->seg = seg;
    n->hauteur = 1 + fg->hauteur > fd->hauteur ? fg->hauteur : fd->hauteur;
    n->fg = fg;
    n->fd = fd;
    return n;
}

void majHauteur_noeud(AVL* noeud){
    noeud->hauteur =  1 + noeud->fg->hauteur > noeud->fd->hauteur ? noeud->fg->hauteur : noeud->fd->hauteur;
}

AVL* rotDroite(AVL* rac){
    AVL* nvrac = rac->fg;
    rac->fg = nvrac->fd;
    nvrac->fd = rac;

    majHauteur_noeud(rac);
    majHauteur_noeud(nvrac);

    return nvrac;
}

AVL* rotGauche(AVL* rac){
    AVL* nvrac = rac->fd;
    rac->fd = nvrac->fg;
    nvrac->fg = rac;

    majHauteur_noeud(rac);
    majHauteur_noeud(nvrac);

    return nvrac;
}

AVL* bienEquilibrer(AVL* noeud){
    if(noeud->fg->hauteur - noeud->fd->hauteur == 2){
	if(noeud->fg->fg->hauteur < noeud->fg->fd->hauteur){
	    noeud->fg = rotGauche(noeud->fg);
	}
	noeud = rotDroite(noeud);
    }

    if(noeud->fg->hauteur - noeud->fd->hauteur == -2){
	if(noeud->fd->fd->hauteur < noeud->fd->fg->hauteur){
	    noeud->fd = rotDroite(noeud->fd);
	}
	noeud = rotGauche(noeud);
    }
    
    return noeud;
}

AVL* insererAVL_seg(AVL* tree, int ord, Segment* seg){
    AVL* nv;
    if(tree == NULL){
	return creeAVL_noeud(ord, seg, NULL, NULL);
    }

    if(ord <= tree->ord){
	if(tree->fg == NULL){
	    nv = creeAVL_noeud(ord, seg, NULL, NULL);
	    tree->fg = nv;
	}
	else{
	    tree->fg = insererAVL_seg(tree->fg, ord, seg);
	}
    }
    else{
	if(tree->fd == NULL){
	    nv = creeAVL_noeud(ord, seg, NULL, NULL);
	    tree->fd = nv;
	}
	else{
	    tree->fd = insererAVL_seg(tree->fd, ord, seg);
	}
    }

    majHauteur_noeud(tree);
    tree = bienEquilibrer(tree);
    
    return tree;
}


Segment* enleverMaxSeg_AVL(AVL** tree){
    Segment* max;
    AVL* max_noeud;
    if((*tree)->fd == NULL){
	max= (*tree)->seg;
	max_noeud = *tree;
	*tree = (*tree)->fg;
	free(max_noeud);
	return max;
    }
    
    max = enleverMaxSeg_AVL(&((*tree)->fd));

    majHauteur_noeud(*tree);
    *tree = bienEquilibrer(*tree);
    
    return max;
}

Segment* enleverMinSeg_AVL(AVL** tree){
    Segment* min;
    AVL* min_noeud;
    if((*tree)->fg == NULL){
	min = (*tree)->seg;
	min_noeud = *tree;
	*tree = (*tree)->fd;
	free(min_noeud);
	return min;
    }
    
    min = enleverMinSeg_AVL(&((*tree)->fg));

    majHauteur_noeud(*tree);
    *tree = bienEquilibrer(*tree);
    
    return min;
}

AVL* supprimerSeg_AVL(AVL* tree, int ord, Segment* seg){
 
    if(tree == NULL){
        return NULL;
    }
    if(ord < tree->ord){
        tree->fg = supprimerSeg_AVL(tree->fg, ord, seg);
    }
    else if(ord > tree->ord ){
        tree->fd = supprimerSeg_AVL(tree->fd, ord, seg);
    }
    else if(compareSegment(tree->seg, seg) != 1){
	tree->fd = supprimerSeg_AVL(tree->fg, ord, seg);
    }
    else{
	tree->seg = enleverMaxSeg_AVL(&(tree->fg));
    }

    majHauteur_noeud(tree);
    tree = bienEquilibrer(tree);

    return tree;
}
