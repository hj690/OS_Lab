#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include "VTable.h"
#include "FrameTable.h"
#include "variables.h"

using namespace std;

class Processor{
private:
	FrameTable *ftablePtr; //FrameTable is base class, will use polymorphism
	int unmapNum;
	int mapNum;
	int inNum;
	int outNum;
	int zeroNum;
	int instrNum;
	long long cycles;
	Printer printer;
public:
	Processor(FrameTable* ftablePtr);
	void getInstr(string filename);
	void Process(pair<int, int> instr);
	void printSum();
};

#endif