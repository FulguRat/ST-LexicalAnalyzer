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
		crtToken.tokenType = NONE_TYPE;
		crtToken.tokenValue = -1;

		crtString.clear();
		_read_crt(crtChar);

		switch (judgeCType(crtChar))
		{
		//keyword or identifier
		case LETTER_TYPE:
			crtString += crtChar;
			//cout << crtChar;
			_read_next(crtChar);

			while (judgeCType(crtChar) != BLANK_TYPE)
			{
				if (judgeCType(crtChar) == LETTER_TYPE ||
					judgeCType(crtChar) == NUMBER_TYPE)
				{
					crtString += crtChar;
					//cout << crtChar;
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
					crtToken.tokenType = KEYWD_TYPE;
					crtToken.tokenValue = 4 + i;
					//cout << "   keyword" << i << endl;
					break;
				}
			}
			if (crtToken.tokenType != KEYWD_TYPE)
			{
				crtToken.tokenType = IDENT_TYPE;
				crtToken.tokenValue = 0;
				//cout << "   identifier" << endl;
			}

			break;

		//const
		case NUMBER_TYPE:
			while (judgeCType(crtChar) == NUMBER_TYPE)
			{
				//cout << crtChar;
				_read_next(crtChar);
			}
			crtToken.tokenType = CONST_TYPE;
			crtToken.tokenValue = 3;
			//cout << "   const" << endl;
			break;

		//string
		case D_QUOTE_TYPE:
			_read_next(crtChar);
			while (judgeCType(crtChar) != D_QUOTE_TYPE)
			{
				//cout << crtChar;
				_read_next(crtChar);
			}
			_read_next(crtChar);
			crtToken.tokenType = STRING_TYPE;
			crtToken.tokenValue = 2;
			//cout << "   string" << endl;
			break;

		//character
		case F_QUOTE_TYPE:
			_read_next(crtChar);
			//cout << crtChar;
			_read_next(crtChar);
			_read_next(crtChar);
			crtToken.tokenType = CHAR_TYPE;
			crtToken.tokenValue = 1;
			//cout << "   character" << endl;
			break;

		//delimiter
		case OTHER_TYPE:
			if (crtChar == '>' || crtChar == '<' || crtChar == '=')
			{
				crtString += crtChar;
				//cout << crtChar;
				_read_next(crtChar);
			}
			if (judgeCType(crtChar) == OTHER_TYPE)
			{
				crtString += crtChar;
				//cout << crtChar;
				_read_next(crtChar);
			}
			for (unsigned int i = 0; i < 18; i++)
			{
				if (crtString == delimitStr[i])
				{
					crtToken.tokenType = DELIMIT_TYPE;
					crtToken.tokenValue = 10 + i;
					//cout << "   delimiter" << i << endl;
					break;
				}
			}
			break;
		default:
			break;
		}

		if (!crtString.empty())
		{
			cout << setw(5) << setfill(' ') << setiosflags(ios::left) << crtString;
			cout << setw(5) << setfill(' ') << setiosflags(ios::left) << crtToken.tokenType;
			cout << setw(2) << setfill('0') << crtToken.tokenValue << endl;
		}
	}

#undef _read_crt
#undef _read_next
}