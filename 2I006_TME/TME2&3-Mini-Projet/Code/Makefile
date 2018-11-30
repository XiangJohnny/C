all: mainTest mainCompare

entree_sortie.o: entree_sortie.c entree_sortie.h
	gcc -c -Wall -o entree_sortie.o entree_sortie.c

liste.o: liste.c liste.h
	gcc -c -Wall -o liste.o liste.c

tabHachage.o: tabHachage.c tabHachage.h
	gcc -Wall -c -o tabHachage.o tabHachage.c

mainTest.o: mainTest.c
	gcc -Wall -c -o mainTest.o mainTest.c

mainTest: entree_sortie.o liste.o tabHachage.o mainTest.o
	gcc -Wall -o mainTest mainTest.o entree_sortie.o liste.o tabHachage.o

mainCompare.o: mainCompare.c
	gcc -Wall -c -o mainCompare.o mainCompare.c

mainCompare: mainCompare.o entree_sortie.o liste.o tabHachage.o
	gcc -Wall -o mainCompare mainCompare.o entree_sortie.o liste.o tabHachage.o

clear:
	rm -f *.o mainTest mainCompare
