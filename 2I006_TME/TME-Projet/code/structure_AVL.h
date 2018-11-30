#ifndef STRUCTURE_AVL_H
#define STRUCTURE_AVL_H

#include "structure.h"

typedef struct avl_tree{
    int ord;
    Segment* seg;
    int hauteur;

    struct avl_tree* fd;
    struct avl_tree* fg;
} AVL;

#endif
