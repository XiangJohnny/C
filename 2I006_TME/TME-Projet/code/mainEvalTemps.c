#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "structure.h"
#include "fonction.h"
#include "fonctionFile.h"

int main(){
    Netlist* netlist;
    double duree;
    struct timeval t1, t2;
    char *tab[6] = {"Instance_Netlist/alea0030_030_10_088.net",
		    "Instance_Netlist/alea0030_030_90_007.net",
		    "Instance_Netlist/alea0100_050_10_097.net",
		    "Instance_Netlist/alea0100_080_90_024.net",
		    "Instance_Netlist/alea0300_300_10_044.net",
		    "Instance_Netlist/alea0300_300_90_099.net"};

    int i;
    for(i = 0; i < 6; i++){
	netlist = netlistFromFile(tab[i]);
	printf("%d ",countNbSegmentNetlist(netlist));
	gettimeofday(&t1,0);
	intersect_naif(netlist);
	gettimeofday(&t2,0);
	duree = t2.tv_sec + t2.tv_usec*0.000001;
	duree -= t1.tv_sec + t1.tv_usec*0.000001;
	printf("%f ",duree);

	netlist = netlistFromFile(tab[i]);
	gettimeofday(&t1,0);
	intersect_balayage(netlist);
	gettimeofday(&t2,0);
	duree = t2.tv_sec + t2.tv_usec*0.000001;
	duree -= t1.tv_sec + t1.tv_usec*0.000001;
	printf("%f\n",duree);
    }
    
    return 0;
}
