#include <iostream>
#include <cstring>
#include <stdio.h> 
#include <stdlib.h>
#include "Token.h"
#include "Error.h"

using namespace std;

bool isNumber(string str){
	char *S = new char[str.length() + 1];
	strcpy(S,str.c_str());

	for(int i = 0; i < str.length(); i++){
		if( (int)S[i] < '0' || (int)S[i] > '9')
			return false;
	}
	return true;
}

bool isSymbol(string str){
//[a-Z][a-Z0-9]
	char * S = new char[str.length() + 1];
	std::strcpy(S,str.c_str());

	if((int)S[0] < 'A' || (int)S[0] > 'z' ||( (int)S[0]>'Z' && (int)S[0] < 'a'))
		return false;

	for(int i = 1; i < str.length(); i++){
		if( (int)S[i] < '0' || ( (int)S[i] > '9' && (int)S[i] < 'A') || ( (int)S[0]>'Z' && (int)S[0] < 'a') ||(int)S[i] > 'z')
			return false;
	}
	return true;
}

bool DefCount::validate(Token token){
	if(!isNumber(token.getToken())){ //token is not number
		Error e;
		e.__parseerror(0, token.getLine(), token.getOffset());
		exit(EXIT_FAILURE);
	}
	else{ //token is number
		int count = atoi(token.getToken().c_str()); //convert string to int
		if(count > 16 || count < 0){
			Error e;
			e.__parseerror(4, token.getLine(), token.getOffset());
			exit(EXIT_FAILURE);
		}
		else return true;
	}
}

bool UseCount::validate(Token token){
	if(!isNumber(token.getToken())){ //token is not number
		Error e;
		e.__parseerror(0, token.getLine(), token.getOffset());
		exit(EXIT_FAILURE);
	}
	else{ //token is number
		int count = atoi(token.getToken().c_str()); //convert string to int
		if(count > 16 || count < 0){
			Error e;
			e.__parseerror(5, token.getLine(), token.getOffset());
			exit(EXIT_FAILURE);
		}
		else return true;
	}
}

bool CodeCount::validate(Token token, int base_Addr){
	if(!isNumber(token.getToken())){ //token is not number
		Error e;
		e.__parseerror(0, token.getLine(), token.getOffset());
		exit(EXIT_FAILURE);
	}
	else{ //token is number
		int count = atoi(token.getToken().c_str()); //convert string to int
		if((count + base_Addr) > 512 || count < 0){
			Error e;
			e.__parseerror(6, token.getLine(), token.getOffset());
			exit(EXIT_FAILURE);
		}
		else return true;
	}
}

bool Symbol::validate(Token token){
	if(!isSymbol(token.getToken())){
		Error e;
		e.__parseerror(1, token.getLine(), token.getOffset());
		exit(EXIT_FAILURE);
	}
	else{
		int length = token.getToken().length();
		if(length > 16){
			Error e;
			e.__parseerror(3, token.getLine(), token.getOffset());
			exit(EXIT_FAILURE);
		}
		else return true;
	}
}

bool Raddress::validate(Token token){
	if(!isNumber(token.getToken())){
		Error e;
		e.__parseerror(0, token.getLine(), token.getOffset());
		exit(EXIT_FAILURE);
	}
	else return true;
}

bool Type::validate(Token token){
	string type = token.getToken();
	if(type != "I" && type != "A" && type != "R" && type != "E"){
		Error e;
		e.__parseerror(2, token.getLine(), token.getOffset());
		exit(EXIT_FAILURE);
	}
	else return true;
}

bool Instr::validate(Token token){
	string str = token.getToken();
	if(!isNumber(str)){
		Error e;
		e.__parseerror(0, token.getLine(), token.getOffset());
		exit(EXIT_FAILURE);
	}
	else return true;

}
