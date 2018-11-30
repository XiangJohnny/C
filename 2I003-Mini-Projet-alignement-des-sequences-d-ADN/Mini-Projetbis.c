#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

//#define MAT(y,x,m) matrice[(y)*(m) + (x)]

int min(int a, int b, int c) {
    if(a <= b) {
	if(a <= c) {
	    return a;
	}
    }
    if(b <= c) {
	return b;
    }
    return c;
}

int DistanceMinRec(char a[], char b[], int n, int m) {
    int d = 1;
    int d1, d2, d3;

    if(n == 0 && m == 0) {
	return 0;
    }
    if(n == 0 && m > 0) {
	return m;
    }
    if(m == 0 && n > 0) {
	return n;
    }
    if(n > 0 && m > 0) {
	if(a[n-1] == b[m-1]) {
	    d = 0;
	}
    }

    d1 = DistanceMinRec(a, b, n-1, m) + 1;
    d2 = DistanceMinRec(a, b, n-1, m-1) + d;
    d3 = DistanceMinRec(a, b, n, m-1) + 1;

    return min(d1, d2, d3);
}

void seqAleatoire(char tab[], int n) {
    int i;
    for(i = 0; i < n; i++) {
	tab[i] = rand()%4;
    }
}

int DistanceMinIter(char a[], char b[], int n, int m) {
    int x,y;
    int d, d1, d2, d3;
    int **ord;
    int distance;
  
    ord = malloc(sizeof(ord) * (n+1));
  
    for(y = 0; y < n+1; y++) {
	ord[y] = malloc(sizeof(int) * (m+1));
    }

    for(x = 0; x < m+1; x++) {
	*(ord[0]+x) = x;
    }
 
    for(y = 0; y < n+1; y++) {
	*(ord[y]) = y;
    }
    
    for(y = 1; y < n+1; y++) {
	for(x = 1; x < m+1; x++) {
          
	    if(a[y-1] == b[x-1]) {
		d = 0;
	    }
	    else {
		d = 1;
	    }
	    d1 = *(ord[y-1]+x) + 1;
	    d2 = *(ord[y-1]+x-1) + d;
	    d3 = *(ord[y]+x-1) + 1;
	    *(ord[y]+x) = min(d1, d2, d3);
	}
    }
 
    distance = *(ord[n]+m);
    for(y = 0; y < n+1; y++) {
	free(ord[y]);
    }
    free(ord);
    return distance;
    
}
