#include <iostream>
#include <string>
#include <queue>
#include <fstream>

#include "IO.h"
#include "Module.h"
#include "Token.h"
#include "STable.h"
#include "Error.h"
#include "Processor.h"
using namespace std;

int main(int argc, char* argv[] ){
	
	IO io;
	queue<Token> tokenList;
	vector<Module> moduleList;
	Module module;
	Token token;
	string filename = "input";

	//=========== pass 1 =======================
	tokenList = io.readFile(argv[1]);  //parses the input
	io.makeModule(tokenList,moduleList); //verifies the correct syntax and determines the base address for each module
	STable stable; 
	stable.creatTable(moduleList); // determin absolute address for each defined symbol and store in symbol table
	
	
	stable.output(); //print symbol table


	//=========== pass 2 =======================
	Processor processor( moduleList, stable);
	processor.process_and_print(); //generate the actual output by relocating relative addresses and resolving external references
	

	

//	system("pause");
	return 0;
}


