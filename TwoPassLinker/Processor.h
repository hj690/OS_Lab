#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <vector>
#include "Module.h"
#include "STable.h"
#include "Error.h"

class Processor{
private:
	vector<Module> moduleList;
	STable s_table;
	string to_string(int num);
public:
	Processor(vector<Module> moduleList, STable s_table){
		this->moduleList = moduleList;
		this->s_table = s_table;
	}
	~Processor(){}
	void process_and_print();
	void process_module(int baseMapNum, Module &module);
	void Process_instr(int mapNum, pair<Type, Instr> code, Module &module);
	void rule7_Detect(Module module);
	void rule4_Detect();
};

#endif