#include <iostream>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "IOQ.h"
#include "variables.h"

using namespace std;

int timer = 0;

int main(int argc, char **argv){

   	char *algorithm = NULL;
  
    int index;
    int c;
     
    opterr = 0;
    while ((c = getopt (argc, argv, "s:")) != -1){
    	switch (c)
        {
        	case 's':{
	            algorithm = optarg;
             	break;
             }
           	case '?':{	
           		if (optopt == 'c')
               		fprintf (stderr, "Option -%c requires an argument.\n", optopt);
            	else if (isprint (optopt))
               		fprintf (stderr, "Unknown option `-%c'.\n", optopt);
             	else
               		fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
             	
             	return 1;
             }
           	default:
             	abort ();
        }
    }
    
     
 	string filename = string(argv[argc-1]);


	IOQ *q = new IOQ();

	switch(algorithm[0]){
		case 'i':
			q = new IOQ_FIFO(filename);
			break;
		case 'j':
			q = new IOQ_SSTF(filename);
			break;
		case 's':
			q = new IOQ_SCAN(filename);
			break;
		case 'c':
			q = new IOQ_CSCAN(filename);
			break;
		case 'f':
			q = new IOQ_FSCAN(filename);
			break;
		default:
			q = new IOQ_FIFO(filename);

	}

	q->process();

	return 0;
}