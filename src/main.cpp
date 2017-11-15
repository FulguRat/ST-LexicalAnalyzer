#include <iostream>
#include "tokenizer.h"

using namespace std;

int main(int argc, char* argv[])
{
	Tokenizer tokenizer("src.c");

	tokenizer.findAllToken();

	tokenizer.outputKeywdList();
	tokenizer.outputDelimitList();
	tokenizer.outputConstList();

	return 0;
}