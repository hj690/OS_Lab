#ifndef _IO
#define _IO

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>
#include <vector>
#include <cstring>

#include "Process.h"

using namespace std;

class IO{
public:
	IO(){}
	queue<int> getRandVals(string filename);
	vector<Process> getProcesses(string filename);
};

#endif