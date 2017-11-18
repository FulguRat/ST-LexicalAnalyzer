#include <iostream>
#include "tokenizer.h"

using namespace std;

int main(int argc, char* argv[])
{
	Tokenizer tokenizer("src.c");

	tokenizer.findAllToken();

	//tokenizer.outputList(tokenizer.keywdSet);
	//tokenizer.outputList(tokenizer.delimitSet);
	//tokenizer.outputList(tokenizer.identifierSet);
	//tokenizer.outputList(tokenizer.constSet);

	return 0;
}