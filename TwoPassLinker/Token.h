#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
using namespace std;

class Token{
private:
	string token;
public:
	int line;
	int offset;
	Token(){}
	Token(string token, int line, int offset){
		this->token = token;
		this->line = line;
		this->offset = offset;
	}
	~Token(){}
	void setLine(int line){
		this->line = line;
	}
	void setOffset(int offset){
		this->offset = offset;
	}
	int getLine(){
		return line;
	}
	int getOffset(){
		return offset;
	}
	string getToken(){
		return token;
	}
	virtual bool validate(Token token){return false;}
};

class DefCount: public Token{
private:
	int defcount;
public:
	DefCount(){}
	~DefCount(){}
	DefCount(Token token){ //call this only when the token is validated
		this->line = token.line;
		this->offset = token.offset;
		this->defcount = atoi(token.getToken().c_str());
	}
	bool validate(Token token); 
	int getDefcount(){
		return defcount;
	}
};

class UseCount: public Token{
private:
	int usecount;
public:
	UseCount(){}
	~UseCount(){}
	UseCount(Token token){
		this->line = token.line;
		this->offset = token.offset;
		this->usecount = atoi(token.getToken().c_str());
	}
	bool validate(Token token); 
	int getUsecount(){
		return usecount;
	}
};

class CodeCount: public Token{
private:
	int codecount;
public:
	CodeCount(){}
	~CodeCount(){}
	CodeCount(Token token){
		this->line = token.line;
		this->offset = token.offset;
		this->codecount = atoi(token.getToken().c_str());
	}
	bool validate(Token token, int base_Addr); 
	int getCodecount(){
		return codecount;
	}
};

class Raddress: public Token{
private:
	int address;
public:
	Raddress(){}
	Raddress(Token token){
		this->line = token.line;
		this->offset = token.offset;
		this->address = atoi(token.getToken().c_str());
	}
	~Raddress(){}
	bool validate(Token token);
	void setAddress(int address){
		this->address = address;
	}
	int getAddress(){
		return address;
	}
};

class Symbol : public Token{
private:
	string symbol;
	Raddress address;
	bool used;
	bool redefined;
	int define_Module_Num;
public:
	Symbol(){
		used = false;
		redefined = false;
	}
	~Symbol(){}
	Symbol(Token token){
		this->symbol = token.getToken();
		this->line = token.getLine();
		this->offset = token.getOffset();
		used = false;
		redefined = false;
	}
	string getSymbol(){
		return symbol;
	}

	void setAddress(Raddress address){
		this->address = address;
	}
	Raddress getAddress(){
		return address;
	}
	bool validate(Token token);
	bool isUsed(){
		return used;
	}
	bool isRedefined(){
		return redefined;
	}
	void setUsed(){
		used = true;
	}
	void setRedefine(){
		redefined = true;
	}

	void set_define_Module_Num(int num){
		this->define_Module_Num = num;
	}

	int get_define_Module_Num(){
		return define_Module_Num;
	}
};

class Type : public Token{
private:
	char type;
public:
	Type(){}
	~Type(){}
	Type(Token token){
		this->line = token.line;
		this->offset = token.offset;
		this->type = token.getToken().at(0);
	}
	
	bool validate(Token token);
	char getType(){
		return type;
	}
};

class Instr: public Token{
private:
	string instr;
public:
	Instr(){}
	Instr(Token token){
		this->line = token.line;
		this->offset = token.offset;
		this->instr = token.getToken();
	}
	
	string getInstr(){
		return instr;
	}
	bool validate(Token token);
};

#endif
