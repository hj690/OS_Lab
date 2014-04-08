#include <iostream>
#include <fstream>
#include <cstring>
#include "STable.h"
using namespace std;

string to_string(int num){
	char tmp[32];
	sprintf(tmp, "%d", num);
	return tmp;
}

//write warning based on rule 5
void write_Warning(int moduleNum, Symbol symbol, Raddress tmpAddr, int codecount){
	string line = "Warning: Module " + to_string(moduleNum) +
					": " + symbol.getSymbol() + " to big "
					+ to_string(tmpAddr.getAddress()) + " (max=" + to_string(codecount-1) 
					+ ") assume zero relative";
//	ofstream outfile;

//	outfile.open("out", ios_base::app);
//	outfile << line << endl;
//	outfile.close();
	cout << line << endl;
}

void STable::insert(string str, Symbol symbol){
	map<string, Symbol>::iterator it;
	it = s_table.find(str);
	if(it == s_table.end()){ //symbol not find
		s_table.insert(pair<string, Symbol>(str, symbol));
	}
	else{
		it->second.setRedefine();
	}
}

int STable::loopup(string str){
	map<string, Symbol>::iterator it;
	it = s_table.find(str);
	if(it == s_table.end()){ //symbol not find
		return -1;
	}
	else{
		return it->second.getAddress().getAddress();
	}
}

void STable::setUsed(string str){
	map<string, Symbol>::iterator it;
	it = s_table.find(str);
	it->second.setUsed();
}

void STable::creatTable(vector<Module> moduleList){
	Module module;
	vector<pair<Symbol, Raddress> > deflist;
	Symbol tmpSymbol;
	
	Raddress tmpAddr;
	int codecount;
	for(int i=0; i < moduleList.size(); i++){
		module = moduleList.at(i);
		deflist = module.getDeflist();
		codecount = module.getCodecount().getCodecount();

		for(int j=0; j < deflist.size(); j++){
			tmpSymbol = deflist.at(j).first;
			tmpAddr = deflist.at(j).second;

			map<string, Symbol>::iterator it;
			it = s_table.find(tmpSymbol.getSymbol());
			//Rule 5: detect if relative address too big
			if( (it == s_table.end()) && //not defined
				(tmpAddr.getAddress() > (codecount-1))){ //address too big
				write_Warning(i+1, tmpSymbol, tmpAddr, codecount);
				tmpAddr.setAddress(0);
			}
			tmpAddr.setAddress(tmpAddr.getAddress() + module.getBaseaddress());
			tmpSymbol.setAddress(tmpAddr);
			tmpSymbol.set_define_Module_Num(module.getNum());
			this->insert(deflist.at(j).first.getSymbol(), tmpSymbol);
		}
	}
}

void STable::show(){
	map<string, Symbol>::iterator it;
	for(it = s_table.begin(); it != s_table.end(); it++){
		cout << it->first << " " << it->second.getAddress().getAddress() << endl;
	}
}

void STable::output(){
//	ofstream outfile;

//	outfile.open("out", ios_base::app);
	cout << "Symbol Table" << endl;
	map<string, Symbol>::iterator it;
	for(it = s_table.begin(); it != s_table.end(); it++){
		cout << it->first << "=" << to_string(it->second.getAddress().getAddress());
		if(it->second.isRedefined()){
			cout << " Error: This variable is multiple times defined; first value used" << endl;
		}
		else cout << endl;
	}
	//outfile.close();

}