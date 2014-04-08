#ifndef STABLE_H
#define STABLE_H

#include <vector>
#include <map>
#include <string>

#include "Module.h"
#include "Token.h"
using namespace std;

class STable{
private:
	map<string, Symbol> s_table;

public:
	STable(){}
	void insert(string str, Symbol symbol);
	int loopup(string str);
	void creatTable(vector<Module> moduleList);
	void show();
	void output();
	void setUsed(string str);
	map<string, Symbol> getStable(){
		return s_table;
	}
};

#endif