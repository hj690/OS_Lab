#ifndef FRAMETABLE_H
#define FRAMETABLE_H
#include <list>
#include <queue>
#include "variables.h"
#include "VTable.h"

using namespace std;

struct Node {
	int index;
	Node* next;
};

class FrameTable{
protected:
	int* fTable;
public:
	FrameTable();

	void setFree(int index){
		fTable[index] = -1;
	}

	void setPageNum(int frameIndex, int pageIndex){
		fTable[frameIndex] = pageIndex;
	}
	int getPageNum(int frameIndex){
		return fTable[frameIndex];
	}
	virtual bool hasFreeFrame();
	virtual int getFreeFrame();
	virtual int getFrame(){return -1;}//get the frame index that will be replaced
	virtual void printFTable();
	virtual void record(int pageIndex){}
	virtual void show(){}
};

class Frame_NRU : public FrameTable{
private:
	int replaceNum;
public:
	Frame_NRU(){this->replaceNum = 0;}
	int getFrame();
};

class Frame_LRU : public FrameTable{
private:
	list<int> myList;
public:
	void record(int pageIndex);
	int getFrame();
};

class Frame_FIFO : public FrameTable{
private:
	bool hasReplace;
	queue<int> myQ;//frame index stored in queue
public:
	Frame_FIFO(){this->hasReplace = false;}
	int getFreeFrame();
	void record(int pageIndex);//pageIndex is the page accessed
	int getFrame();
};

class Frame_Random : public FrameTable{
	int getFrame();
};

class Frame_2Chance : public FrameTable{
private:
	bool hasReplace;
	queue<int> myQ;//frame index stored in queue
public:
	Frame_2Chance(){this->hasReplace = false;}
	int getFreeFrame();
	void record(int pageIndex);//pageIndex is the page accessed
	int getFrame();
};

//Based on Physical Frames 
class Frame_clock : public FrameTable{
private:
	bool hasReplace;
	Node* clockHand;
	Node* runner;
public:
	Frame_clock();
	void record(int pageIndex);
	int getFreeFrame();
	int getFrame();
};

class Frame_Clock : public FrameTable{
private:
	Node* clockHand;
	Node* runner;
public:
	Frame_Clock();
	int getFrame();
};

class Frame_age : public FrameTable{
private:
	unsigned int* ages;
	bool hasReplace;
public:
	Frame_age();
	void record(int pageIndex);
	int getFrame();

};

class Frame_Age : public FrameTable{
private:
	unsigned int* ages;
	bool hasReplace;
public:
	Frame_Age();
	int getFrame();
	void record(int pageIndex);
};
#endif