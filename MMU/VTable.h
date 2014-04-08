#ifndef VTABLE_H
#define VTABLE_H

#include <iostream>
#include "bithacks.h"
using namespace std;



class VTable{
private:
	unsigned int *vTable;

public:
	VTable();
	~VTable(){}
	//unsigned int* getVTable(){return this->vTable;}
	void setPresent(int index);
	void setModified(int index);
	void setReferenced(int index);
	void setPageout(int index);

	void unsetPresent(int index);
	void unsetModified(int index);
	void unsetReferenced(int index);
	

	bool ifPresent(int index);
	bool ifModified(int index);
	bool ifReferenced(int index);
	bool ifPageout(int index);

	void setFrameNum(int index, int pNum);
	int getFrameNum(int index);

	void testShow();
};

#endif