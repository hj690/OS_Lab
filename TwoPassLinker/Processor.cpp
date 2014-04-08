#include <fstream>
#include <iomanip>
#include <iostream>
#include "Processor.h"
using namespace std;

string Processor::to_string(int num){
	char tmp[32];
	sprintf(tmp, "%d", num);
	return tmp;
}

void Processor::Process_instr(int mapNum, pair<Type, Instr> code, Module &module){
	vector<Symbol> uselist = module.getUselist();
	int codecount = module.getCodecount().getCodecount();
	int usecount = module.getUsecount().getUsecount();
	int baseAddr = module.getBaseaddress();


	char type = code.first.getType();
	string instr = code.second.getInstr();
	int opcode;
	int operand;

	switch (type){
		//(I) an immediate operand is unchanged; Note that there is no opcode in this case and operand is all fourdigits
		case 'I': 
			if(instr.length() > 4)
				cout << setfill('0') << setw(3) << mapNum << ": 9999 Error: Illegal immediate value; treated as 9999" << endl;
			else{
				int original = atoi(instr.c_str());
				cout << setfill('0') << setw(3) << mapNum << ": " <<  setfill('0') << setw(4) << original << endl;
			}
			break;

		//(A) an absolute address is unchanged;
		case 'A': 
			if(instr.length() > 4)
				cout << setfill('0') << setw(3) << mapNum << ": 9999 Error: Illegal opcode; treated as 9999" << endl;
			else{
				int original = atoi(instr.c_str());
				operand = original % 1000;
				opcode = (original - operand)/1000;
				if(operand > 512){
					cout << setfill('0') << setw(3) << mapNum << ": " <<  setfill('0') << setw(4) << opcode * 1000 
						<< " Error: Absolute address exceeds machine size; zero used" << endl;
				}
				else{
					cout << setfill('0') << setw(3) << mapNum << ": " << setfill('0') << setw(4) << original << endl;
				}
			}
			break;
		
		case 'R':
			if(instr.length() > 4)
				cout << setfill('0') << setw(3) << mapNum << ": 9999 Error: Illegal opcode; treated as 9999" << endl;
			else{
				int original = atoi(instr.c_str());
				operand = original % 1000;
				opcode = (original - operand)/1000;
				if(operand > codecount){
					cout << setfill('0') << setw(3) << mapNum << ": " <<  setfill('0') << setw(4) << opcode * 1000 + baseAddr
						<< " Error: Relative address exceeds module size; zero used" << endl;
				}
				else{
					if(operand + baseAddr > 512)
						cout << setfill('0') << setw(3) << mapNum << ": " <<  setfill('0') << setw(4) << opcode * 1000 
						<< " Error: Absolute address exceeds machine size; zero used" << endl;
					else
						cout << setfill('0') << setw(3) << mapNum << ": " <<  setfill('0') << setw(4) << original + baseAddr << endl;
				}
			}
			break;

		case 'E':
			if(instr.length() > 4)
				cout << setfill('0') << setw(3) << mapNum << ": 9999 Error: Illegal opcode; treated as 9999" << endl;
			else{
				int original = atoi(instr.c_str());
				operand = original % 1000;
				opcode = (original - operand)/1000;

				//rule 6
				if(operand >= usecount){
					cout << setfill('0') << setw(3) << mapNum << ": " << setfill('0') << setw(4) << original  
						<< " Error: External address exceeds length of uselist; treated as immediate" << endl;
				}
				else{
					string sym = uselist.at(operand).getSymbol();
					int relative = s_table.loopup(sym);
					module.set_Uselist_Symbol_Used(operand);
					if(relative != -1){//find symbol
						cout << setfill('0') << setw(3) << mapNum << ": " << setfill('0') << setw(4) << opcode * 1000 + relative << endl;
						s_table.setUsed(sym); // set used in STable
						 // set used in module
					}
					else{ //can't find symbol
						cout << setfill('0') << setw(3) << mapNum << ": " << setfill('0') << setw(4) << opcode * 1000 << " Error: " << sym
							<< " is not defined; zero used" << endl;
					}
				}

			}
			break;
			
	}
}

void Processor::process_module(int baseMapNum, Module &module){
	vector<pair<Type, Instr> > program = module.getProgram();
	vector<pair<Type, Instr> >::iterator it;
	for(it = program.begin(); it != program.end(); it++){
		Process_instr(baseMapNum++, pair<Type, Instr>(it->first, it->second), module);
	}
	rule7_Detect(module);
}



void Processor::process_and_print(){
	cout << endl << "Memory Map " << endl;
	int baseMapNum = 0;
	for(int i = 0; i < moduleList.size(); i++){
		process_module(baseMapNum, moduleList.at(i));
		baseMapNum += moduleList.at(i).getCodecount().getCodecount();
	}

	cout << endl;

	rule4_Detect();

}


void Processor::rule7_Detect(Module module){
	vector<Symbol> uselist = module.getUselist();
	for(int i = 0; i < uselist.size(); i++){
		int addr = s_table.loopup(uselist.at(i).getSymbol());
		if(uselist.at(i).isUsed() == false){
			cout << "Warning: Module " << to_string(module.getNum()) << ": " << uselist.at(i).getSymbol() 
				<< " appeared in the uselist but was not actually used" << endl;
		}
	}
}

void Processor::rule4_Detect(){
	map<string, Symbol> table = s_table.getStable();
	map<string, Symbol>::iterator it;
	for(it = table.begin(); it != table.end(); it++){
		if(it->second.isUsed() == false){ // not used
			cout << "Warning: Module " << to_string(it->second.get_define_Module_Num()) << ": " 
				<< it->second.getSymbol() << " was defined but never used" << endl;
		}
	}
}
