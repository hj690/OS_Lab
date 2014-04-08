#ifndef MODULE_H
#define MODULE_H

#include <map>
#include <vector>
#include <string>

#include "Token.h"
using namespace std;

class Module{
	int num;
	DefCount defcount;
	vector<pair<Symbol, Raddress> > deflist;
	UseCount usecount;
	vector<Symbol> uselist;
	CodeCount codecount;
	vector<pair<Type, Instr> > program;
	int baseaddress;
public:
	Module(){}
	~Module(){}

	Module(int baseaddress, int num){
		this->baseaddress = baseaddress;
		this->num = num;
	}
	void setNum(int num){
		this->num = num;
	}
	void setBaseaddress(int baseaddress){
		this->baseaddress = baseaddress;
	}
	void setDefcount(DefCount defcount){
		this->defcount = defcount;
	}
	void setDeflist(vector<pair<Symbol, Raddress> > deflist){
		this->deflist = deflist;
	}
	void setUsecount(UseCount usecount){
		this->usecount = usecount;
	}
	void setUselist(vector<Symbol> uselist){
		this->uselist = uselist;
	}
	void setCodecount(CodeCount codecount){
		this->codecount = codecount;
	}
	void setProgram(vector<pair<Type, Instr> > program){
		this->program = program;
	}
	int getNum(){
		return num;
	}
	DefCount getDefcount(){
		return defcount;
	}
	vector<pair<Symbol, Raddress> > getDeflist(){
		return deflist;
	}
	UseCount getUsecount(){
		return usecount;
	}
	vector<Symbol> getUselist(){
		return uselist;
	}
	CodeCount getCodecount(){
		return codecount;
	}
	vector<pair<Type, Instr> > getProgram(){
		return program;
	}
	int getBaseaddress(){
		return baseaddress;
	}
	void set_Uselist_Symbol_Used(int position){
		uselist.at(position).setUsed();
	}
	void show();
};

#endif