#include "Processor.h"


int convertToInt(string str){
		int result;
		stringstream convert(str);
		if ( !(convert >> result) )//give the value to Result using the characters in the string
	    	result = 0;//if that fails set Result to 0
	    return result;
}

pair<int,int> parseLine(string line){
	pair<int, int> instr;
	int i=0;
	const char *delimiter = " ";
	char *p;
	char *cstr = new char[line.size() + 1];
	strcpy(cstr, line.c_str());
	p = strtok(cstr, delimiter);
	while(p && i < 2){
		string str(p); //convert char* to string
		if(str.at(0) == '#'){ //invalid instruction, will not be processed
			instr.first = -1;
			instr.second = -1;
		}
		else{//valid instruction, will be processed
			if(i == 0)
				instr.first = convertToInt(str);
			else //i==1
				instr.second = convertToInt(str);
		}
		i++;
		p = strtok(NULL, delimiter);
	}
	return instr;

}

Processor::Processor(FrameTable *ftablePtr){
	this->unmapNum = 0;
	this->mapNum = 0;
	this->inNum = 0;
	this->outNum = 0;
	this->zeroNum = 0;
	this->instrNum = 0;
	this->ftablePtr = ftablePtr;
	this->cycles = 0;
	this->printer = Printer();
}

void Processor::printSum(){
	this->cycles = (long)(mapNum + unmapNum) * 400 + (long)(inNum + outNum) * 3000 + (long)zeroNum * 150 + (long)instrNum;
	printf("SUM %d U=%d M=%d I=%d O=%d Z=%d ===> %llu\n", 
		this->instrNum, this->unmapNum, this->mapNum, this->inNum, 
		this->outNum, this->zeroNum, this->cycles); 
}

void Processor::getInstr(string filename){
	pair<int, int> instr;
	string line;
	ifstream myfile(filename.c_str());
	if(myfile.is_open()){
		while( getline(myfile, line)){
			if(line.length() > 0){
				instr = parseLine(line);
				if(instr.first != -1){ //valid instruction
					Process(instr);
					this->instrNum++;
					this->ftablePtr->record(instr.second);
				}
			}
		}
		myfile.close();
		if(P) printer.printPageTable();
		if(F) ftablePtr->printFTable();
		if(S) printSum();
	}
}


void Processor::Process(pair<int, int> instr){
	
	int rw = instr.first;
	int pageIndex = instr.second;
	if(O) printer.printInstr(instr);
	cycles += 1;
	if(vTable.ifPresent(pageIndex)){
		if(rw == 1)
			vTable.setModified(pageIndex);
		vTable.setReferenced(pageIndex);
	}

	if(!vTable.ifPresent(pageIndex)){//not present
		int frameIndex;

		//free frame available
		if(ftablePtr->hasFreeFrame()){
			frameIndex = ftablePtr->getFreeFrame();
			if(!vTable.ifPageout(pageIndex)){
				if(O) printer.printZero(instrNum, frameIndex);
				this->zeroNum++;
			}
			else{
				if(O) printer.printIn(instrNum, pageIndex, frameIndex);
				this->inNum++;
			}

			//make mapping
			ftablePtr->setPageNum(frameIndex, pageIndex);
			vTable.setFrameNum(pageIndex, frameIndex);
			if(O) printer.printMap(instrNum, pageIndex, frameIndex);
			this->mapNum++;

			//set P/R/M bit
			vTable.setPresent(pageIndex);
			vTable.setReferenced(pageIndex);
			if(rw == 1)
				vTable.setModified(pageIndex);
		}


		//need replace, unmap
		else{
			if(instrNum == 91){
				cout << "";
			}
			frameIndex = ftablePtr->getFrame(); 

			//make unmapping
			int oldPageIndex = ftablePtr->getPageNum(frameIndex);


			//ummap
			if(O) printer.printUnMap(instrNum, oldPageIndex, frameIndex);
			this->unmapNum++;

			//if modified, OUT
			if(vTable.ifModified(oldPageIndex)){
				if(O) printer.printOut(instrNum, oldPageIndex, frameIndex);
				this->outNum++;
				vTable.setPageout(oldPageIndex);
			}
			vTable.unsetPresent(oldPageIndex); 
			vTable.unsetModified(oldPageIndex);
			vTable.unsetReferenced(oldPageIndex);
			

			//make mapping
			if(!vTable.ifPageout(pageIndex)){
				if(O) printer.printZero(instrNum, frameIndex);
				this->zeroNum++;
			}
			else{
				if(O) printer.printIn(instrNum, pageIndex, frameIndex);
				this->inNum++;
			}
			ftablePtr->setPageNum(frameIndex, pageIndex);
			vTable.setFrameNum(pageIndex, frameIndex);
			if(O) printer.printMap(instrNum, pageIndex, frameIndex);
			this->mapNum++;
			//set R/M bit
			vTable.setPresent(pageIndex);
			vTable.setReferenced(pageIndex);
			if(rw == 1)
				vTable.setModified(pageIndex);
		}
	

	}
	//	printer.printPageTable();
	//	ftablePtr->printFTable();

		
}
