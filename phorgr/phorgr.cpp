// phorgr.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <cpprest\json.h>

using namespace web;

int main()
{
	// Create a JSON object.
	json::value obj;
	obj[L"key1"] = json::value::boolean(false);
	obj[L"key2"] = json::value::number(44);
	obj[L"key3"] = json::value::number(43.6);
	obj[L"key4"] = json::value::string(U("str"));

	
	std::wcout << L"key1: " << obj[L"key1"].as_string() << std::endl;
	
    return 0;
}

