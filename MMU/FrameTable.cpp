#include "FrameTable.h"
#include "variables.h"

FrameTable::FrameTable(){
	this->fTable = new int[frameSize];
	//initialize the frame table
	for(int i = 0; i < frameSize; i++){
		fTable[i] = -1; //-1 means the frame is empty
	}
}

bool FrameTable::hasFreeFrame(){
	for(int i = 0; i < frameSize; i++){
			if(fTable[i] == -1){//the frame is free
				return true;
			}
		}
		return false;
}

int FrameTable::getFreeFrame(){
	for(int i = 0; i < frameSize; i++){
		if(fTable[i] == -1)//the frame is free
			return i;
	}
	return -1;
}

void FrameTable::printFTable(){
	for(int i = 0; i < frameSize; i++){
		if(fTable[i] == -1)
			cout << "*";
		else
			cout << fTable[i];
		cout << " ";
	}
	cout << endl;
}

int Frame_NRU::getFrame(){
	int result;
	
	
	//if the frame table is full
	int class0 = 0;
	int class1 = 0;
	int class2 = 0;
	int class3 = 0;
	for(int i = 0; i < 64; i++){
		if(vTable.ifPresent(i)){
			if(!vTable.ifReferenced(i) && !vTable.ifModified(i))
				class0++;
			if(!vTable.ifReferenced(i) && vTable.ifModified(i))
				class1++;
			if(vTable.ifReferenced(i) && !vTable.ifModified(i))
				class2++;
			if(vTable.ifReferenced(i) && vTable.ifModified(i))
				class3++;
		}
	}

	if(class0 > 0){
		int rand = randNum.myRandom(class0);
		for(int i = 0; i < 64; i++){
			if(vTable.ifPresent(i) && !vTable.ifReferenced(i) && !vTable.ifModified(i)){
				if(rand == 0){
					result = vTable.getFrameNum(i);
					break;
				}
				else
					rand--;
			}
		}
	}

	else if(class1 > 0){
		int rand = randNum.myRandom(class1);
		for(int i = 0; i < 64; i++){
			if(vTable.ifPresent(i) && !vTable.ifReferenced(i) && vTable.ifModified(i)){
				if(rand == 0){
					result = vTable.getFrameNum(i);
					break;
				}
				else
					rand--;
			}
		}
	}

	else if(class2 > 0){
		int rand = randNum.myRandom(class2);
		for(int i = 0; i < 64; i++){
			if(vTable.ifPresent(i) && vTable.ifReferenced(i) && !vTable.ifModified(i)){
				if(rand == 0){
					result = vTable.getFrameNum(i);
					break;
				}
				else
					rand--;
			}
		}
	}

	else if(class3 > 0){
		int rand = randNum.myRandom(class3);
		for(int i = 0; i < 64; i++){
			if(vTable.ifPresent(i) && vTable.ifReferenced(i) && vTable.ifModified(i)){
				if(rand == 0){
					result = vTable.getFrameNum(i);
					break;
				}
				else
					rand--;
			}
		}
	}
	else{}

	replaceNum++;
	if(replaceNum == 10){
		for(int i = 0; i < 64; i++){
			if(vTable.ifPresent(i))
				vTable.unsetReferenced(i);
		}
		replaceNum = 0;
	}

	return result;

}

void Frame_LRU::record(int pageIndex){

	list<int>::iterator it;
	for(it = myList.begin(); it != myList.end(); it++){
		if(*it == pageIndex){
			myList.remove(pageIndex);
			break;
		}
	}
	myList.push_back(pageIndex);
}

int Frame_LRU::getFrame(){
	list<int>::iterator it;
	int pageIndex;
	for(it = myList.begin(); it != myList.end(); it++){
		pageIndex = *it;
		if(vTable.ifPresent(pageIndex))
			return vTable.getFrameNum(pageIndex);
	}
}

int Frame_FIFO::getFreeFrame(){
	for(int i = 0; i < frameSize; i++){
		if(fTable[i] == -1){//the frame is free
			myQ.push(i);
			return i;
		}
	}
	return -1;
}

int Frame_FIFO::getFrame(){
	this->hasReplace = true;
	return myQ.front();
}

void Frame_FIFO::record(int pageIndex){
	if(hasReplace){
		myQ.pop();
		myQ.push(vTable.getFrameNum(pageIndex));
	}
	hasReplace = false;
}

int Frame_Random::getFrame(){
	int rand = randNum.myRandom(frameSize);
	//cout << "rand: " << rand << endl;
	return rand;
}

int Frame_2Chance::getFreeFrame(){
	for(int i = 0; i < frameSize; i++){
		if(fTable[i] == -1){//the frame is free
			myQ.push(i);
			return i;
		}
	}
	return -1;
}

int Frame_2Chance::getFrame(){
	this->hasReplace = true;

	int size = myQ.size();

	for(int i = 0; i < size; i++){
		int firstPage = getPageNum(myQ.front());
		if(!vTable.ifReferenced(firstPage)){ //R=0
			break;
		}
		else{//R=1
			vTable.unsetReferenced(firstPage);
			myQ.pop();
			myQ.push(vTable.getFrameNum(firstPage));
		}
	}

	return myQ.front();

	
}

void Frame_2Chance::record(int pageIndex){
	if(hasReplace){
		myQ.pop();
		myQ.push(vTable.getFrameNum(pageIndex));
	}
	hasReplace = false;
}

Frame_clock::Frame_clock(){
	clockHand = new Node;
	clockHand->index = -1;
	runner = clockHand;
	for(int i = 0; i < frameSize-1; i++){
		runner->next = new Node;
		runner = runner->next;
		runner->index = -1;
	}
	runner->next = clockHand;
	runner = clockHand;
	hasReplace = false;
}

int Frame_clock::getFreeFrame(){
	for(int i = 0; i < frameSize; i++){
		if(fTable[i] == -1){//the frame i is free
			runner->index = i;
			runner = runner->next;
			return i;
		}
	}
	return -1;
}

int Frame_clock::getFrame(){
	hasReplace = true;
	while(true){
		if(!vTable.ifReferenced(getPageNum(clockHand->index))){
			break;
		}
		else{
			vTable.unsetReferenced(getPageNum(clockHand->index));
			clockHand = clockHand->next;
		}
	}
	return clockHand->index;

}

void Frame_clock::record(int pageIndex){
	if(hasReplace){
		clockHand = clockHand->next;
		hasReplace = false;
	}
}

Frame_Clock::Frame_Clock(){
	clockHand = new Node;
	clockHand->index = 0;
	runner = clockHand;
	for(int i = 1; i < 64; i++){
		runner->next = new Node;
		runner = runner->next;
		runner->index = i;
	}
	runner->next = clockHand;
	runner = clockHand;

}

int Frame_Clock::getFrame(){
	while(true){
		int pageIndex = clockHand->index;
		if(vTable.ifPresent(pageIndex)){
			if(!vTable.ifReferenced(pageIndex))
				break;
			else
				vTable.unsetReferenced(pageIndex);
		}
		clockHand = clockHand->next;
	}
	int page = clockHand->index;
	clockHand = clockHand->next;
	return vTable.getFrameNum(page);
}

Frame_age::Frame_age(){
	this->hasReplace = false;
	ages = new unsigned int[frameSize];
	for(int i = 0; i < frameSize; i++)
		ages[i] = 0;
}

void Frame_age::record(int pageIndex){
	if(hasReplace){
		ages[vTable.getFrameNum(pageIndex)] = 0;
		hasReplace = false;
	}
}

int Frame_age::getFrame(){

	hasReplace = true;
	for(int i = 0; i < frameSize; i++){
		B_Shift_Right_1(ages[i]);
		if(vTable.ifReferenced(getPageNum(i))){
			B_SET(ages[i], 31);
		}
	}
	unsigned int min = ages[0];
	int frameIndex = 0;

	for(int i = 1; i < frameSize; i++){
		if(ages[i] < min){
			min = ages[i];
			frameIndex = i;
		}
	}

	for(int i = 0; i < frameSize; i++){
		vTable.unsetReferenced(getPageNum(i));
	}

	return frameIndex;
}

Frame_Age::Frame_Age(){
	ages = new unsigned int[64];
	for(int i = 0; i < 64; i++){
		ages[i] = 0;
	}
	hasReplace = false;
}

int Frame_Age::getFrame(){
	hasReplace = true;

	//age operations
	for(int i = 0; i < 64; i++){
		if(vTable.ifPresent(i)){
			B_Shift_Right_1(ages[i]);
			if(vTable.ifReferenced(i)){
				B_SET(ages[i], 31);
			}
		}
	}

	unsigned int min;
	int pageIndex;
	for(int i = 0; i < 64; i++){
		if(vTable.ifPresent(i)){
			min = ages[i];
			pageIndex = i;
			break;
		}
	}

	for(int i = 0; i < 64; i++){
		if(vTable.ifPresent(i) && ages[i] < min){
			min = ages[i];
			pageIndex = i;
		}
	}

	for(int i = 0; i < 64; i++){
		if(vTable.ifPresent(i))
			vTable.unsetReferenced(i);			
	}


	return vTable.getFrameNum(pageIndex);

	

}

void Frame_Age::record(int pageIndex){
	if(hasReplace){
		ages[pageIndex] = 0;
		hasReplace = false;
	}

}
