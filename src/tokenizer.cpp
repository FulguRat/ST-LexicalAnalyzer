#include "tokenizer.h"

Tokenizer::Tokenizer(const char* srcFile)
{
	//open source code file with file stream
	srcCode.open(srcFile, ios::in);
	srcCode >> noskipws;
}

Tokenizer::~Tokenizer()
{
	//close source code file
	srcCode.close();
}

//judge the type of a character
CType Tokenizer::judgeCType(const char tmpChar)
{
	//is letter?
	if ((tmpChar >= 'a' && tmpChar <= 'z') ||
		(tmpChar >= 'A' && tmpChar <= 'Z'))
		return LETTER_TYPE;

	//is number?
	else if (tmpChar >= '0' && tmpChar <= '9')
		return NUMBER_TYPE;

	//is space or enter?
	else if (tmpChar == ' ' || tmpChar == '\n')
		return BLANK_TYPE;

	//is double quote?
	else if (tmpChar == '\"')
		return D_QUOTE_TYPE;

	//is forward quote?
	else if (tmpChar == '\'')
		return F_QUOTE_TYPE;

	//else type
	else
		return OTHER_TYPE;
}

//find next token
void Tokenizer::findToken(void)
{
#define _read_crt(tmpChar)  do \
							{ \
								if (nextReadFlag == false) { srcCode >> tmpChar; } \
								else { nextReadFlag = false; } \
							}while(0)
#define _read_next(tmpChar) do \
							{ \
								if (nextReadFlag == false) { srcCode >> tmpChar; nextReadFlag = true; } \
								else { srcCode >> tmpChar; } \
							}while(0)

	nextReadFlag = false;

	//find one string in a while()
	while (!srcCode.eof())
	{
		crtString.clear();
		_read_crt(crtChar);

		switch (judgeCType(crtChar))
		{
		case LETTER_TYPE:
			crtString += crtChar;
			cout << crtChar;
			_read_next(crtChar);

			while (judgeCType(crtChar) != BLANK_TYPE)
			{
				if (judgeCType(crtChar) == LETTER_TYPE ||
					judgeCType(crtChar) == NUMBER_TYPE)
				{
					crtString += crtChar;
					cout << crtChar;
					_read_next(crtChar);
					
					continue;
				}
				else
				{
					break;
				}
			}
			for (unsigned int i = 0; i < 6; i++)
			{
				if (crtString == keywdStr[i])
				{
					stringType = KEYWD_TYPE;
					cout << "   keyword" << i << endl;
					break;
				}
			}
			if (stringType != KEYWD_TYPE)
			{
				stringType = IDENT_TYPE;
				cout << "   identifier" << endl;
			}

			stringType = NONE_TYPE;
			break;

		case NUMBER_TYPE:
			while (judgeCType(crtChar) == NUMBER_TYPE)
			{
				cout << crtChar;
				_read_next(crtChar);
			}
			cout << "   const" << endl;
			break;

		default:
			break;
		}
	}

#undef _read_crt
#undef _read_next
}