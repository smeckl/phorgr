#pragma once
#include <string>

class Utils
{
public:
	Utils();
	~Utils();

	static std::wstring asciiToWideString(const char *szStr);
	static std::wstring concatenatePath(const std::wstring &left, const std::wstring &right);
};

