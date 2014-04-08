#include "Printer.h"
#include "variables.h"
void Printer::printPageTable(){
	for(int i = 0; i < 64; i++){
			if(vTable.ifPresent(i)){
				cout << i << ":";
				if(vTable.ifReferenced(i))
					cout << "R";
				else
					cout << "-";
				if(vTable.ifModified(i))
					cout << "M";
				else
					cout << "-";
				if(vTable.ifPageout(i))
					cout << "S";
				else
					cout << "-";
			}
			else{
				if(vTable.ifPageout(i))
					cout << "#";
				else
					cout << "*";
			}
			cout << " ";
		}
		cout << endl;
}