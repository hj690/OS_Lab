#ifndef RANDVAL_H
#define RANDVAL_H
#include <iostream>
#include <queue>

using namespace std;

class randVal{
private:
	queue<int> randvals;
public:
	randVal(){}
	randVal(string filename){
		getRandVal(filename);
	};
	void getRandVal(string filename);
	int getNextVal();
	int myRandom(int size);

};

#endif