#include "tokenizer.h"

Tokenizer::Tokenizer(const char* srcFile)
{
	//open source code file with file stream
	srcCode.open(srcFile, ios::in);
	if (!srcCode.is_open())
	{
		cout << "Source Code Not Found ¡Ñ©n¡Ñ" << endl;
		exit(1);
	}
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
	else if (tmpChar == ' ' || tmpChar == '\f' || tmpChar == '\n' || 
			tmpChar == '\r' || tmpChar == '\t' || tmpChar == '\v')
		return BLANK_TYPE;

	//is forward quote?
	else if (tmpChar == '\'')
		return F_QUOTE_TYPE;

	//is double quote?
	else if (tmpChar == '\"')
		return D_QUOTE_TYPE;

	//is underline?
	else if (tmpChar == '_')
		return UDLINE_TYPE;

	//else type
	else
		return OTHER_TYPE;
}

//find next token
token Tokenizer::findNextToken(void)
{
#define _err_report do \
					{ \
						crtToken.tokenType = ERROR_TYPE; \
						cout << setiosflags(ios::left) << setw(9) << crtString << \
								"Error token type (£þ¡¢£þ)" << endl; \
						return crtToken; \
					} while (0) \

	//initialize current token
	crtToken.tokenWord.clear();
	crtToken.tokenType = NONE_TYPE;
	crtToken.tokenValue = -1;

	//initialize current string
	crtString.clear();

	//input characters from srcCode until one character has a valid type
	do
	{
		//if all of srcCode has been read, return an empty token
		if (!(srcCode >> crtChar))
		{ 
			return crtToken; 
		}

		//is it annotation?
		if (crtChar == '/')
		{
			if (!(srcCode >> crtChar))
			{
				break;
			}

			// "//"
			if (crtChar == '/')
			{
				do
				{
					if (!(srcCode >> crtChar))
					{
						return crtToken;
					}
				} while (crtChar != '\n');
			}
			// "/*...*/"
			else if (crtChar == '*')
			{
				while (true)
				{
					if (!(srcCode >> crtChar))
					{
						crtToken.tokenType = ERROR_TYPE;
						cout << "/*...*/ annotation have only one half. (¡£©n¡£)" << endl;
						return crtToken;
					}

					if (crtChar == '*')
					{
						srcCode >> crtChar;
						if (crtChar == '/')
						{
							srcCode >> crtChar;
							break;
						}
					}
				}
			}
			//not annotation
			else
			{
				srcCode.seekg(-1, srcCode.cur);
				crtChar = '/';
			}
		}

	} while (judgeCType(crtChar) == BLANK_TYPE);

	//Judge type of next token
	switch (judgeCType(crtChar))
	{
	//keyword or identifier
	case UDLINE_TYPE:
	case LETTER_TYPE:
		crtString += crtChar;
		if (!(srcCode >> crtChar))
		{
			crtChar = ' ';
		}

		while (judgeCType(crtChar) == LETTER_TYPE || judgeCType(crtChar) == NUMBER_TYPE || 
			judgeCType(crtChar) == UDLINE_TYPE)
		{
			crtString += crtChar;
			if (!(srcCode >> crtChar))
			{
				break;
			}
		}

		//match keyword
		for (unsigned int i = 0; i < SIZE_OF_KEYWD_LIST; i++)
		{
			if (crtString == keywdList[i])
			{
				crtToken.tokenType = KEYWD_TYPE;
				pushBackNoRepeat(keywdSet, crtString);
				crtToken.tokenValue = 4 + i;
				break;
			}
		}
		if (crtToken.tokenType != KEYWD_TYPE)
		{
			crtToken.tokenType = IDENT_TYPE;
			pushBackNoRepeat(identifierSet, crtString);
			crtToken.tokenValue = 0;
		}

		break;

	//const
	//***.***e(+/-)***  (* is a number)
	//For example: +6.23e-89
	case NUMBER_TYPE:
		crtToken.tokenType = INTEGER_TYPE;
		crtString += crtChar;
		if (!(srcCode >> crtChar))
		{
			crtChar = ' ';
		}

		while (judgeCType(crtChar) == NUMBER_TYPE)
		{
			crtString += crtChar;
			if (!(srcCode >> crtChar))
			{
				break;
			}
		}

		if (crtChar == '.')
		{
			crtToken.tokenType = FLOAT_TYPE;
			crtString += crtChar;
			srcCode >> crtChar;

			while (judgeCType(crtChar) == NUMBER_TYPE)
			{
				crtString += crtChar;
				srcCode >> crtChar;
			}
		}

		if (crtChar == 'e')
		{
			//fix me, scientific notation should have a token type
			crtString += crtChar;
			srcCode >> crtChar;

			if (crtChar == '+' || crtChar == '-')
			{
				crtString += crtChar;
				srcCode >> crtChar;
			}

			while (judgeCType(crtChar) == NUMBER_TYPE)
			{
				crtString += crtChar;
				srcCode >> crtChar;
			}
		}

		pushBackNoRepeat(constSet, crtString);
		crtToken.tokenValue = 3;
		break;

	//character
	case F_QUOTE_TYPE:
		crtString += crtChar;
		srcCode >> crtChar;
		if (crtChar == '\\')
		{
			crtString += crtChar;
			srcCode >> crtChar;
			if (crtChar != 'a' && crtChar != 'b' && crtChar != 'f' && crtChar != 'n' && crtChar != 'r' &&
				crtChar != 't' && crtChar != 'v' && crtChar != '\\' && crtChar != '\'' && crtChar != '\"' &&
				crtChar != '?' && crtChar != '0')
			{
				cout << "Error in position 6" << endl;
				_err_report;
			}
		}
		crtString += crtChar;

		srcCode >> crtChar;
		if (judgeCType(crtChar) != F_QUOTE_TYPE)
		{
			cout << "Error in position 1" << endl;
			_err_report;
		}
		crtString += crtChar;

		srcCode >> crtChar;
		crtToken.tokenType = CHAR_TYPE;
		crtToken.tokenValue = 1;
		break;

	//string
	case D_QUOTE_TYPE:
		crtString += crtChar;
		srcCode >> crtChar;

		while (judgeCType(crtChar) != D_QUOTE_TYPE)
		{
			crtString += crtChar;
			if (!(srcCode >> crtChar))
			{
				cout << "Error in position 5" << endl;
				_err_report;
			}
		}
		crtString += crtChar;
		srcCode >> crtChar;
		crtToken.tokenType = STRING_TYPE;
		crtToken.tokenValue = 2;
		break;

	//delimiter
	case OTHER_TYPE:
		crtString += crtChar;
		if (!(srcCode >> crtChar))
		{
			crtChar = ' ';
		}

		//if next character is also OTHER_TYPE
		if (judgeCType(crtChar) == OTHER_TYPE)
		{
			crtString += crtChar;

			//match delimit type
			for (unsigned int i = 0; i < SIZE_OF_DELIMIT_LIST; i++)
			{
				if (crtString == delimitList[i])
				{
					crtToken.tokenType = DELIMIT_TYPE;
					pushBackNoRepeat(delimitSet, crtString);
					crtToken.tokenValue = SIZE_OF_KEYWD_LIST + 4 + i;
					srcCode.seekg(1, srcCode.cur);
					break;
				}
			}
			if (crtToken.tokenType != DELIMIT_TYPE)
			{
				crtString.pop_back();
			}
		}
		srcCode.seekg(-1, srcCode.cur);

		//match delimit type
		for (unsigned int i = 0; i < SIZE_OF_DELIMIT_LIST; i++)
		{
			if (crtString == delimitList[i])
			{
				crtToken.tokenType = DELIMIT_TYPE;
				pushBackNoRepeat(delimitSet, crtString);
				crtToken.tokenValue = SIZE_OF_KEYWD_LIST + 4 + i;
				srcCode.seekg(1, srcCode.cur);
				break;
			}
		}
		if (crtToken.tokenType != DELIMIT_TYPE)
		{
			cout << "Error in position 2" << endl;
			_err_report;
		}
		break;

	default:
		cout << "Error in position 3" << endl;
		_err_report;
	}

	if (crtToken.tokenType != NONE_TYPE)
	{
		crtToken.tokenWord = crtString;
		srcCode.seekg(-1, srcCode.cur);
		return crtToken;
	}
	else
	{
		cout << "Error in position 4" << endl;
		_err_report;
	}

#undef _err_report
}

void Tokenizer::findAllToken()
{
	token nextToken;
	tokenSet.clear();

	while (nextToken.tokenType != NONE_TYPE)
	{
		nextToken = findNextToken();
		if (nextToken.tokenType != NONE_TYPE && nextToken.tokenType != ERROR_TYPE)
		{
			tokenSet.push_back(nextToken);
			cout << setiosflags(ios::left) << setw(9) << nextToken.tokenWord;
			cout << setiosflags(ios::left) << setw(4) << nextToken.tokenType;
			cout << setiosflags(ios::left) << setw(4) << nextToken.tokenValue;
			cout << endl;
		}
	}
	nextToken.tokenValue = 100;
	nextToken.tokenWord = "#";
	tokenSet.push_back(nextToken);
}

void Tokenizer::outputList(vector<string> list)
{
	for (size_t i = 0; i < list.size(); i++)
	{
		cout << list[i] << endl;
	}
}

void Tokenizer::pushBackNoRepeat(vector<string> &list, string str)
{
	for (size_t i = 0; i < list.size(); i++)
	{
		if (list[i] == str) { return; }
	}
	list.push_back(str);
}