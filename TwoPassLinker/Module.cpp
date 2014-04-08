#include <iostream>
#include "Module.h"

void Module::show(){
	int defcount = getDefcount().getDefcount();
	vector<pair<Symbol, Raddress> > deflist = getDeflist();
	int usecount = getUsecount().getUsecount();
	vector<Symbol> uselist = getUselist();
	int codecount = getCodecount().getCodecount();
	vector<pair<Type, Instr> > program = getProgram();
	cout << defcount << " ";
	for(int i = 0; i < deflist.size(); i++){
		cout << deflist.at(i).first.getSymbol() << " " << deflist.at(i).second.getAddress()<< " ";
	}
	cout << endl;

	cout << usecount << " ";
	for(int i = 0; i < uselist.size(); i++){
		cout << uselist.at(i).getSymbol()<< " ";
	}
	cout << endl;

	cout << codecount << " ";
	for(int i = 0; i < program.size(); i++){
		cout << program.at(i).first.getType() << " " << program.at(i).second.getInstr() << " ";
	}
	cout << endl;

}