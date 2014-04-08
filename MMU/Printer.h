#ifndef PRINTER_H
#define PRINTER_H
#include <iostream>
#include <iomanip>

#include "FrameTable.h"
#include "variables.h"
using namespace std;

class Printer{
public:
	Printer(){}
	void printInstr(pair<int, int> instr){
		cout << "==> inst: " << instr.first << " " << instr.second << endl;
	}

	void printZero(int instrNum, int frameNum){
		cout << instrNum << ": ZERO";
		cout << setw(9) << frameNum <<endl;
	}

	void printMap(int instrNum, int pageNum, int frameNum){
		cout << instrNum << ": MAP";
		cout << setw(6) << pageNum;
		cout << setw(4) << frameNum <<endl;
	}

	void printUnMap(int instrNum, int pageNum, int frameNum){
		cout << instrNum << ": UNMAP";
		cout << setw(4) << pageNum;
		cout << setw(4) << frameNum <<endl;
	}

	void printIn(int instrNum, int pageNum, int frameNum){
		cout << instrNum << ": IN";
		cout << setw(7) << pageNum;
		cout << setw(4) << frameNum <<endl;
	}

	void printOut(int instrNum, int pageNum, int frameNum){
		cout << instrNum << ": OUT";
		cout << setw(6) << pageNum;
		cout << setw(4) << frameNum <<endl;
	}

	void printPageTable();
		

		
};

#endif