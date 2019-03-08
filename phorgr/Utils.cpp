/*Copyright 2019 Steve Meckl

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http ://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

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
