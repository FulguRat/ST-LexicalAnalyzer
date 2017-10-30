#ifndef _TOKENIZER_H
#define _TOKENIZER_H

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#define NONE_TYPE     0

#define LETTER_TYPE   1
#define NUMBER_TYPE   2
#define D_QUOTE_TYPE  3
#define F_QUOTE_TYPE  4
#define BLANK_TYPE    5
#define OTHER_TYPE    6

#define IDENT_TYPE    1
#define CHAR_TYPE     2
#define STRING_TYPE   3
#define CONST_TYPE    4
#define KEYWD_TYPE    5
#define DELIMIT_TYPE  6
#define ERROR_TYPE    7

#define STRING_SIZE   5

//list of keyword and boundary
const string keywdStr[6] = { "int", "main", "void", "if", "else", "char" };
const string boundStr[16] = { ">=", "<=", "==", "=", ">", "<", "+", "-", "*", "/",
                              "{", "}", ",", ";", "(", ")"};

//type of char & string
typedef int Type;
typedef Type CType;
typedef Type SType;

//type of token
typedef struct
{
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
	void findToken(void);

private:
	ifstream srcCode;

	//current character and next character
	char crtChar;
	//whether next character have been read
	bool nextReadFlag;

	string crtString;

	SType stringType = NONE_TYPE;

	token crtToken;
};
#endif //_TOKENIZER_H