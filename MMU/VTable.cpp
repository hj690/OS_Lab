#include "VTable.h"

VTable::VTable(){
	vTable = new unsigned int[64];	
	for(int i = 0; i < 64; i++){
		vTable[i] = 0;
	}
}

void VTable::setPresent(int index){
	B_SET(vTable[index], 31);
}

void VTable::setModified(int index){
	B_SET(vTable[index], 30);
}

void VTable::setReferenced(int index){
	B_SET(vTable[index], 29);
}

void VTable::setPageout(int index){
	B_SET(vTable[index], 28);
}

void VTable::unsetPresent(int index){
	B_UNSET(vTable[index], 31);
}

void VTable::unsetModified(int index){
	B_UNSET(vTable[index], 30);
}

void VTable::unsetReferenced(int index){
	B_UNSET(vTable[index], 29);
}

bool VTable::ifPresent(int index){
	return B_IS_SET(vTable[index], 31);
}

bool VTable::ifModified(int index){
	return B_IS_SET(vTable[index], 30);
}

bool VTable::ifReferenced(int index){
	return B_IS_SET(vTable[index], 29);
}

bool VTable::ifPageout(int index){
	return B_IS_SET(vTable[index], 28);
}

void VTable::setFrameNum(int index, int pNum){
	unsigned x = pNum;
	if(ifPresent(index))
		B_SET(x, 31);
	if(ifModified(index))
		B_SET(x, 30);
	if(ifReferenced(index))
		B_SET(x, 29);
	if(ifPageout(index))
		B_SET(x, 28);

	vTable[index] = x;
}

int VTable::getFrameNum(int index){
	unsigned int x = vTable[index];
	B_UNSET(x, 31);
	B_UNSET(x, 30);
	B_UNSET(x, 29);
	B_UNSET(x, 28);

	int pageNum = x;
	return pageNum;
}

void VTable::testShow(){
	for(int i = 0; i < 64; i++){
		cout << ifPresent(i) << " " << ifModified(i) << " " << ifReferenced(i)
			<< " " << ifPageout(i) << " " << getFrameNum(i) << endl;
	}
}