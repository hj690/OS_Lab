#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include "variables.h"
#include "VTable.h"
#include "bithacks.h"
#include "randVal.h"
#include "FrameTable.h"
#include "Processor.h"
using namespace std;

randVal randNum = randVal();
VTable vTable = VTable();
bool O = 0;
bool P = 0;
bool F = 0;
bool S = 0;
int frameSize = 32;

int main(int argc, char **argv){

   	char *algorithm = NULL;
   	char *options = NULL;
    char *frames = NULL;
    int index;
    int c;
     
    opterr = 0;
    while ((c = getopt (argc, argv, "a:o:f:")) != -1){
    	switch (c)
        {
        	case 'a':{
	            algorithm = optarg;
             	break;
             }
           	case 'o':{
            	options = optarg;
             	break;
             }
           	case 'f':{
            	frames = optarg;
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
    
     
 	string filename = string(argv[argc-2]);
    string rfile = string(argv[argc-1]);

    frameSize = atoi(frames);

	randNum = randVal(rfile);
	FrameTable * ft = new FrameTable();

	switch(algorithm[0]){
		case 'N':
			ft = new Frame_NRU();
			break;
		case 'l':
			ft = new Frame_LRU();
			break;
		case 'r':
			ft = new Frame_Random();
			break;
		case 'f':
			ft = new Frame_FIFO();
			break;
		case 's':
			ft = new Frame_2Chance();
			break;
		case 'c':
			ft = new Frame_clock();
			break;
		case 'C':
			ft = new Frame_Clock();
			break;
		case 'a':
			ft = new Frame_age();
			break;
		case 'A':
			ft = new Frame_Age();
			break;
		default:
			ft = new Frame_LRU();

	}

	string option = string(options);
	for(int i = 0; i < option.length(); i++){
		char op = option.at(i);
		if(op == 'O') O = 1;
		if(op == 'P') P = 1;
		if(op == 'F') F = 1;
		if(op == 'S') S = 1;
		
	}

	Processor p = Processor(ft);

	p.getInstr(filename);


	return 0;
}