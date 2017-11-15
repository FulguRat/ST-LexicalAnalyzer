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
#define INTEGER_TYPE  2
#define FLOAT_TYPE    3
#define CHAR_TYPE     4
#define STRING_TYPE   5
#define KEYWD_TYPE    6
#define DELIMIT_TYPE  7
#define ERROR_TYPE    8

//list of keyword and delimiter
//identifier   0
//character    1
//string       2
//const        3
#define SIZE_OF_KEYWD_LIST 28
const string keywdList[SIZE_OF_KEYWD_LIST] = 
							{ "void"/*4*/, "int"/*5*/, "char"/*6*/, "float"/*7*/, "double"/*8*/, "short"/*9*/, "long"/*10*/, 
							"signed"/*11*/, "unsigned"/*12*/, "if"/*13*/, "else"/*14*/, "while"/*15*/, "do"/*16*/, "for"/*17*/, 
							"switch"/*18*/, "case"/*19*/, "default"/*20*/, "break"/*21*/, "continue"/*22*/, "struct"/*23*/, 
							"union"/*24*/, "enum"/*25*/, "typedef"/*26*/, "static"/*27*/, "extern"/*28*/, "include"/*29*/, 
							"return"/*30*/, "main"/*31*/ };

#define SIZE_OF_DELIMIT_LIST 40
const string delimitList[SIZE_OF_DELIMIT_LIST] = 
							{ ">"/*32*/, "<"/*33*/, ">="/*34*/, "<="/*35*/, "=="/*36*/, "+"/*37*/, "-"/*38*/, "*"/*39*/, "/"/*40*/, 
							"+="/*41*/, "-="/*42*/, "*="/*43*/, "/="/*44*/, "="/*45*/, "!="/*46*/, "++"/*47*/, "--"/*48*/, 
							"&&"/*49*/, "||"/*50*/, "!"/*51*/, "&"/*52*/, "|"/*53*/, "&="/*54*/, "|="/*55*/, "{"/*56*/, "}"/*57*/, 
							"["/*58*/, "]"/*59*/, "("/*60*/, ")"/*61*/, ","/*62*/, ";"/*63*/, "."/*64*/, ":"/*65*/, "?"/*66*/, 
							"#"/*67*/, "%"/*68*/, "//"/*69*/, "/*"/*70*/, "*/"/*71*/ };

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
	int tokenIndex;
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

	//output keyword list, delimiter list and const list
	void outputKeywdList();
	void outputDelimitList();
	void outputConstList();

	//find all token and put them to a vector
	vector<token> tokenSet;
	void findAllToken(void);

private:
	ifstream srcCode;

	//current character
	char crtChar;

	string crtString;

	token crtToken;

	vector<string> keywdSet;
	vector<string> delimitSet;
	vector<string> constSet;
};
#endif //_TOKENIZER_H