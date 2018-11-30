
all: Mini-Projet Mini-Projet2

Mini-Projet.o: Mini-Projet.c
	gcc -Wall -c -o Mini-Projet.o Mini-Projet.c

main-Mini-Projet.o:  main-Mini-Projet.c
	gcc -Wall -c -o main-Mini-Projet.o main-Mini-Projet.c

Mini-Projet: main-Mini-Projet.o Mini-Projet.o
	gcc -Wall -o Mini-Projet main-Mini-Projet.o Mini-Projet.o

main-Mini-Projet2.o:  main-Mini-Projet2.c
	gcc -Wall -c -o main-Mini-Projet2.o main-Mini-Projet2.c

Mini-Projet2:  main-Mini-Projet2.o Mini-Projet.o
	gcc -Wall -o Mini-Projet2 main-Mini-Projet2.o Mini-Projet.o
clean:
	rm -f *.o Mini-Projet Mini-Projet2
