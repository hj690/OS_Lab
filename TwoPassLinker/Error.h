#ifndef ERROR_H
#define ERROR_H

#include <iostream>
using namespace std;

class Error{
private:
	string to_string(int num);
public:
	void __parseerror(int errcode, int linenum, int lineoffset);
};

#endif