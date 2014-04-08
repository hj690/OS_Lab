#include <iostream>
#include <sstream>
#include <cstring>
#include <queue>
#include <vector>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "Variable.h"
#include "RandVals.h"
#include "IO.h"
#include "Process.h"
#include "Scheduler.h"
#include "Buffer.h"
using namespace std;
int timer = 0;
RandVals randNum = RandVals();

int main(int argc, char **argv){
	string schedulerType;
	string inputFile;
	string randFile;
	int c;

	while ((c = getopt (argc, argv, "vs:")) != -1)
		switch (c){
			case 's':
			{
				schedulerType = string(optarg);
             	break;
         	}
         	case 'v':{
         		break;
         	}
         	
	        case '?':
	        {/*
	            if (optopt == 's')
	               fprintf (stderr, "Option -%c requires an argument.\n", optopt);
	            else if (isprint (optopt))
	               fprintf (stderr, "Unknown option `-%c'.\n", optopt);
	            else
	               fprintf (stderr,
	                        "Unknown option character `\\x%x'.\n",
	                        optopt);*/
	            return 1;
         	}
          	default:
             abort ();
       	}
     
    inputFile = string(argv[optind]);
    randFile = string(argv[optind+1]);
	randNum = RandVals(randFile);
	IO io = IO();
	vector<Process> processes = io.getProcesses(inputFile);

	if(schedulerType.length() == 1){ //FLS
		switch(schedulerType.at(0)){
			case 'F':{		
				Scheduler_FCFS scheduler1 = Scheduler_FCFS(processes);
				cout << "FCFS" << endl;
				scheduler1.run();
				scheduler1.showSum();
				break;
			}
			case 'L':{
				Scheduler_LCFS scheduler2 = Scheduler_LCFS(processes);
				cout << "LCFS" << endl;
				scheduler2.run();
				scheduler2.showSum();
				break;
			}
			case 'S':{
				Scheduler_SJF scheduler3 = Scheduler_SJF(processes);
				cout << "SJF" << endl;
				scheduler3.run();
				scheduler3.showSum();
				break;
			}
		}
	}
	else if(schedulerType.at(0) == 'R'){ //RR
		string quantumStr = schedulerType.substr(1, schedulerType.length() - 1);
		int quantum;
		stringstream convert(quantumStr);
		if ( !(convert >> quantum) ){//give the value to Result using the characters in the string
	    	quantum = 0;
	    	cout << "quantum parse error." << endl;
	    }
	    Scheduler_RR scheduler4 = Scheduler_RR(processes, quantum);
	    cout << "RR " << quantum << endl;
		scheduler4.run();
		scheduler4.showSum();
	}

	return 0;
}
