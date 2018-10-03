#include "stdafx.h"
#include "Utils.h"
#include <iostream>
#include <sstream> 

Utils::Utils()
{
}


Utils::~Utils()
{
}

std::wstring Utils::asciiToWideString(const char *szStr)
{
	size_t newsize = strlen(szStr) + 1;
	std::wstring res;

	wchar_t* wcstring = new wchar_t[newsize];

	if (NULL != wcstring)
	{
		size_t convertedChars = 0;
		mbstowcs_s(&convertedChars, wcstring, newsize, szStr, _TRUNCATE);

		res = wcstring;

		delete wcstring;
	}

	return res;
}

std::wstring Utils::concatenatePath(const std::wstring &left, const std::wstring &right)
{
	std::wstringstream newString;

	newString << left;

	if (left.back() != L'\\')
		newString << L'\\';

	newString << right;

	return newString.str();
}
