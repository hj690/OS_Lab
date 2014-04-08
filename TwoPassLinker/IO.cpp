#include <fstream>
#include <iostream>
#include <string>
#include <cstring> 

#include "IO.h"

//========================================================================

void throwError(int errorcode, int line, int offset){
	Error e;
	e.__parseerror(errorcode,line, offset);
	exit(EXIT_FAILURE);
}


queue<Token> IO::readFile(char* file_name){
	queue<Token> tokenList; //split all tokens and store in queue
	string line;
	int lineNum = 1;
	int lastOffset;
	void addTokens(string line, int lineNum, queue<Token> &tokenList);

	ifstream myfile (file_name);
	if (myfile.is_open()){
		while ( getline (myfile,line) ){
			addTokens(line, lineNum, tokenList);
			lastOffset = line.length() + 1;
			lineNum++;
		}
		myfile.close();
		tokenList.push(Token("", lineNum-1, lastOffset));
	}
	else 
		cout << "Unable to open file";

	return tokenList;
}

void addTokens(string line, int lineNum, queue<Token> &tokenList){
	
	//get every token's offset
	queue<int> offsetBuf;

	if(line.size() > 0){ // if this line is not empty
		if(line.size() == 1){
			offsetBuf.push(1);
		}
		else{
			for(int i = 0; i < (line.size()-1); i++){
				if(i==0 && line.at(0) != ' ' &&  line.at(0) != '\t')
					offsetBuf.push(i+1);
				else if( (line.at(i) == ' ' || line.at(i) == '\t') 
					&& line.at(i+1) != ' ' 
					&& line.at(i+1) != '\t')
					offsetBuf.push(i+2);
			}
		}

		//construct token objects
		const char *delimiter = " \t";
		char *p;
		char *cstr = new char[line.size() + 1];
		strcpy(cstr, line.c_str());
		p = strtok(cstr, delimiter);
		while(p){
			std::string str(p); //convert char* to string
			tokenList.push(Token(str, lineNum, offsetBuf.front()));
			offsetBuf.pop();
			p=strtok(NULL,delimiter);
		}
	}
}

//========================================================================

void IO::makeModule(queue<Token> tokenList, vector<Module> &moduleList){
	int baseAddr = 0;
	int module_Num = 1;
	if(tokenList.size() == 0){
		throwError(0,0,0);
	}
	while(tokenList.size() > 1){
		Module module(baseAddr, module_Num++);
		Token tok, tok1, tok2;

		//set definition count
		tok2 = tokenList.front();
		tokenList.pop();
		DefCount defcount;
		if(defcount.validate(tok2)){
			defcount = DefCount(tok2);
		}
		module.setDefcount(defcount);

		//set definition list
		vector<pair<Symbol, Raddress> > deflist;
		int dcount = defcount.getDefcount();
		for(int i = 0; i < dcount; i++){
			tok1 = tokenList.front();  //symbol
			tokenList.pop();
			if(tok1.getToken() == "")
				throwError(1, tok1.getLine(), tok1.getOffset());
			tok2 = tokenList.front(); //address
			tokenList.pop();

			Symbol sym;
			Raddress addr;
			if(sym.validate(tok1) && addr.validate(tok2)){
				sym = Symbol(tok1);
				addr = Raddress(tok2);
				sym.setAddress(addr);
				deflist.push_back(pair<Symbol, Raddress>(sym, addr));
			}
			
		}
		module.setDeflist(deflist);

		//set use count
//		if(tokenList.size() == 1)
//			throwError(0, tok2.getLine(), tok2.getOffset());
		tok = tokenList.front();
		tokenList.pop();
		if(tok.getToken() == "")
				throwError(0, tok.getLine(), tok.getOffset());
		UseCount usecount;
		if(usecount.validate(tok)){
			usecount = UseCount(tok);
		}
		module.setUsecount(usecount);


		//set use list
		vector<Symbol> uselist;
		int ucount = usecount.getUsecount();
		for(int i = 0; i < ucount; i++){
//			if(tokenList.size() == 1)
//				throwError(1, tok.getLine(), tok.getOffset());
			tok1 = tokenList.front();
			tokenList.pop();
			if(tok1.getToken() == "")
				throwError(1, tok1.getLine(), tok1.getOffset());

			Symbol sym;
			if(sym.validate(tok1)){
				sym = Symbol(tok1);
				uselist.push_back(sym);
			}
		}
		module.setUselist(uselist);


		//set code count
//		if(tokenList.size() == 1)
//			throwError(0, tok1.getLine(), tok1.getOffset());
		tok2 = tokenList.front();
		tokenList.pop();
		if(tok2.getToken() == "")
				throwError(0, tok2.getLine(), tok2.getOffset());
		CodeCount codecount;
		if(codecount.validate(tok2, baseAddr)){
			codecount = CodeCount(tok2);
			baseAddr = baseAddr + codecount.getCodecount();
		}
		module.setCodecount(codecount);


		//set code text
		vector<pair<Type, Instr> > program;

		int ccount = codecount.getCodecount();
		for(int i = 0; i < ccount; i++){
//			if(tokenList.size() == 1)
//				throwError(2, tok2.getLine(), tok2.getOffset() + tok2.getToken().length());
			tok1 = tokenList.front();  //symbol
			tokenList.pop();
			if(tok1.getToken() == "")
				throwError(2, tok1.getLine(), tok1.getOffset());
			
//			if(tokenList.size() == 1)
//				throwError(2, tok1.getLine(), tok1.getOffset() + tok1.getToken().length());
			tok2 = tokenList.front(); //address
			tokenList.pop();
			if(tok2.getToken() == "")
				throwError(2, tok2.getLine(), tok2.getOffset());

			Type type;
			Instr instr;
			if(type.validate(tok1) && instr.validate(tok2)){
				type = Type(tok1);
				instr = Instr(tok2);
				program.push_back(pair<Type, Instr>(type, instr));
			}

		}

		module.setProgram(program);

		moduleList.push_back(module);
		
	}

	
}

