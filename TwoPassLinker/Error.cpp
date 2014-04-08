#include <string>
#include <fstream>
#include "Error.h"
using namespace std;


string Error::to_string(int num){
	char tmp[32];
	sprintf(tmp, "%d", num);
	return tmp;
}

void Error::__parseerror(int errcode, int linenum, int lineoffset){
	static string errstr[] = { 
	"NUM_EXPECTED",  // Number expect 
	"SYM_EXPECTED",   // Symbol Expected 
	"ADDR_EXPECTED",   // Addressing Expected 
	"SYM_TOLONG",    // Symbol Name is to long 
	"TO_MANY_DEF_IN_MODULE",  // .. 
	"TO_MANY_USE_IN_MODULE", 
	"TO_MANY_INSTR", };

	string line = "Parse Error line "+ to_string(linenum) + " offset " + to_string(lineoffset)
		+ ": " + errstr[errcode] + "\n";

	cout << line;
}