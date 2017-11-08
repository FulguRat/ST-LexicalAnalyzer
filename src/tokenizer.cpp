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
token Tokenizer::findNextToken(void)
{
	crtToken.tokenWord.clear();
	crtToken.tokenType = NONE_TYPE;
	crtToken.tokenValue = -1;

	crtString.clear();

	srcCode >> crtChar;
	if (srcCode.eof()) { return crtToken; }

	while (judgeCType(crtChar) == NONE_TYPE || judgeCType(crtChar) == BLANK_TYPE)
	{
		srcCode >> crtChar;
	}

	switch (judgeCType(crtChar))
	{
	//keyword or identifier
	case LETTER_TYPE:
		crtString += crtChar;
		srcCode >> crtChar;

		while (judgeCType(crtChar) != BLANK_TYPE)
		{
			if (judgeCType(crtChar) == LETTER_TYPE ||
				judgeCType(crtChar) == NUMBER_TYPE)
			{
				crtString += crtChar;
				srcCode >> crtChar;
					
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
				crtToken.tokenType = KEYWD_TYPE;
				crtToken.tokenValue = 4 + i;
				break;
			}
		}
		if (crtToken.tokenType != KEYWD_TYPE)
		{
			crtToken.tokenType = IDENT_TYPE;
			crtToken.tokenValue = 0;
		}

		break;

	//const
	case NUMBER_TYPE:
		while (judgeCType(crtChar) == NUMBER_TYPE)
		{
			crtString += crtChar;
			srcCode >> crtChar;
		}

		if (crtChar == '.')
		{
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

		crtToken.tokenType = CONST_TYPE;
		crtToken.tokenValue = 3;
		break;

	//string
	case D_QUOTE_TYPE:
		srcCode >> crtChar;

		while (judgeCType(crtChar) != D_QUOTE_TYPE)
		{
			crtString += crtChar;
			srcCode >> crtChar;
		}
		srcCode >> crtChar;
		crtToken.tokenType = STRING_TYPE;
		crtToken.tokenValue = 2;
		break;

	//character
	case F_QUOTE_TYPE:
		srcCode >> crtChar;
		crtString += crtChar;

		srcCode >> crtChar;
		srcCode >> crtChar;
		crtToken.tokenType = CHAR_TYPE;
		crtToken.tokenValue = 1;
		break;

	//delimiter
	case OTHER_TYPE:
		if (crtChar == '>' || crtChar == '<' || crtChar == '=')
		{
			crtString += crtChar;
			srcCode >> crtChar;
		}
		if (judgeCType(crtChar) == OTHER_TYPE)
		{
			crtString += crtChar;
			srcCode >> crtChar;
		}
		for (unsigned int i = 0; i < 18; i++)
		{
			if (crtString == delimitStr[i])
			{
				crtToken.tokenType = DELIMIT_TYPE;
				crtToken.tokenValue = 10 + i;
				break;
			}
		}
		break;
	default:
		break;
	}

	if (crtToken.tokenType != NONE_TYPE)
	{
		crtToken.tokenWord = crtString;
		srcCode.seekg(-1, srcCode.cur);

		return crtToken;
	}
	else
	{
		crtToken.tokenType = ERROR_TYPE;
		cout << "Judge token type error (¤Ã ¡ã§¥ ¡ã;)¤Ã" << endl;
		return crtToken;
	}
}

void Tokenizer::findAllToken()
{
	token nextToken;
	while (!srcCode.eof())
	{
		nextToken = findNextToken();
		cout << setiosflags(ios::left) << setw(9) << nextToken.tokenWord;
		cout << setiosflags(ios::left) << setw(4) << nextToken.tokenType;
		cout << setiosflags(ios::left) << setw(4) << nextToken.tokenValue;
		cout << endl;
	}
}