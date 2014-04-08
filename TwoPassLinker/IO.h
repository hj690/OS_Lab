#ifndef IO_H
#define IO_H

#include <string>
#include <queue>
#include <iostream>

#include "Token.h"
#include "Module.h"
#include "STable.h"
#include "Error.h"
using namespace std;

class IO{
private:
	vector<string> output;
public:
	IO(){}
	~IO(){}
	//read file and get all the tokens with their positions(line, offset)
	queue<Token> readFile(char* file_name);

	//detect all the tokens, if legal, then make module
	void makeModule(queue<Token> tokenList, vector<Module> &moduleList);
	void insertLine(string line){
		output.push_back(line);
	}
	vector<string> getOutput(){
		return output;
	}


};

#endif