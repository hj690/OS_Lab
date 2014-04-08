#ifndef _RandVals
#define _RandVals

#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include "IO.h"
using namespace std;

class RandVals{
private:
	queue<int> randvals;
	string filename;
public:
	RandVals(){}
	RandVals(string filename){
		IO io = IO();
		this->randvals = io.getRandVals(filename);
	}
	
	RandVals(queue<int> randvals){
		this->randvals = randvals;
	}

	int getNextVal(){
		int tmp = randvals.front();
		randvals.pop();
		randvals.push(tmp);
		return tmp;
	}

	int myRandom(int burst){
		return 1 + (getNextVal() % burst);   
	}

};

#endif