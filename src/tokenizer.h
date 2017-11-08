#ifndef _TOKENIZER_H
#define _TOKENIZER_H

#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

#define NONE_TYPE     0

#define LETTER_TYPE   1
#define NUMBER_TYPE   2
#define F_QUOTE_TYPE  3
#define D_QUOTE_TYPE  4
#define BLANK_TYPE    5
#define OTHER_TYPE    6

#define IDENT_TYPE    1
#define CONST_TYPE    2
#define CHAR_TYPE     3
#define STRING_TYPE   4
#define KEYWD_TYPE    5
#define DELIMIT_TYPE  6
#define ERROR_TYPE    7

//list of keyword and boundary
#define SIZE_OF_KEYWD_LIST 28
const string keywdList[SIZE_OF_KEYWD_LIST] = 
							{ "void", "int", "char", "float", "double", "short", "long", "signed", "unsigned",
							"if", "else", "while", "do", "for", "switch", "case", "default", "break", "continue", 
							"struct", "union", "enum", "typedef", "static", "extern", "include", "return","main" };

#define SIZE_OF_DELIMIT_LIST 30
const string delimitList[SIZE_OF_DELIMIT_LIST] = 
							{ ">", "<", ">=", "<=", "==", "+", "-", "*", "/", "=", "&&", "||", "!", "&", "|",
							"{", "}", "[", "]", "(", ")", ",", ";", ".", ":", "?", "#", "//", "/*", "*/"};

//type of char & string
typedef int Type;
typedef Type CType;
typedef Type SType;

//type of token
typedef struct
{
	string tokenWord;
	SType tokenType;
	int tokenValue;
}token;

class Tokenizer
{
public:
	Tokenizer(const char* srcFile);
	~Tokenizer();

	//judge the type of a character
	CType judgeCType(const char tmpChar);
	
	//find next token
	token findNextToken(void);

	void findAllToken(void);

private:
	ifstream srcCode;

	//current character
	char crtChar;

	string crtString;

	token crtToken;
};
#endif //_TOKENIZER_H