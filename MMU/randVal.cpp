#include <fstream>
#include <sstream>
#include <string>
#include "randVal.h"
#include <stdio.h>
#include <stdlib.h>

/*
int strToInt(string str){
	int result;
	stringstream convert(str);
	if ( !(convert >> result) )//give the value to Result using the characters in the string
	    result = 0;//if that fails set Result to 0
	return result;
}
*/
void randVal::getRandVal(string filename){
	string line;
	bool firstLine = true;
	int size = 0;
	ifstream myfile(filename.c_str());
	if(myfile.is_open()){
		while( getline(myfile, line)){
			if(firstLine)
			{
				size = atoi(line.c_str());
				firstLine = false;
			}
			else{
				if(line != "")
					randvals.push(atoi(line.c_str()));
			}
		}
		myfile.close();
	}
}

int randVal::getNextVal(){
	int tmp = randvals.front();
	randvals.pop();
	randvals.push(tmp);
	return tmp;
}

int randVal::myRandom(int size){
	return (getNextVal() % size);
}
