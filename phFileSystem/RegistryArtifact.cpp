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
#include <memory>
#include "RegistryArtifact.h"
#include <cpprest\json.h>

using namespace web::json;

const std::wstring RegistryArtifact::REGISTRY = L"registry";
const std::wstring RegistryArtifact::KEY = L"key";
const std::wstring RegistryArtifact::VALUES = L"values";
const std::wstring RegistryArtifact::NAME = L"name";
const std::wstring RegistryArtifact::VALUE = L"value";
const std::wstring RegistryArtifact::CREATE = L"create";
const std::wstring RegistryArtifact::ROOT_KEY = L"rootKey";
const std::wstring RegistryArtifact::TYPE = L"type";

std::map<std::wstring, HKEY> RegistryArtifact::m_rootKeys = 
{
	{ L"HKEY_CLASSES_ROOT", HKEY_CLASSES_ROOT },
	{ L"HKEY_CURRENT_CONFIG", HKEY_CURRENT_CONFIG },
	{ L"HKEY_CURRENT_USER", HKEY_CURRENT_USER },
	{ L"HKEY_LOCAL_MACHINE", HKEY_LOCAL_MACHINE },
	{ L"HKEY_USERS", HKEY_USERS }
};

std::map<std::wstring, DWORD> RegistryArtifact::m_valueTypes = 
{
	{ L"REG_DWORD", REG_DWORD },
	{ L"REG_BINARY", REG_BINARY },
	{ L"REG_EXPAND_SZ", REG_EXPAND_SZ },
	{ L"REG_QWORD", REG_QWORD },
	{ L"REG_SZ", REG_SZ }
};

std::shared_ptr<IArtifact> RegistryArtifact::createArtifact(const std::wstring &jsonData)
{
	std::shared_ptr<IArtifact> pArt(reinterpret_cast<IArtifact*>(new RegistryArtifact(jsonData)));

	return pArt;
}

RegistryArtifact::RegistryArtifact()
{

}

RegistryArtifact::RegistryArtifact(const std::wstring &jsonData)
{
	try
	{
		// Parse value
		utility::stringstream_t ss;
		ss << jsonData.c_str();
		value v1 = value::parse(ss);

		// Get root key
		std::wstring rootKey = v1[ROOT_KEY].as_string();
		if (m_rootKeys.find(rootKey) != m_rootKeys.end())
			m_hRootKey = m_rootKeys[rootKey];
		else
			throw new web::json::json_exception(L"Invalid root key name.");

		// Get key name
		Key(v1[KEY].as_string().c_str());

		// Get create value
		m_create = v1[CREATE].as_bool();

		// Get all values
		auto values = v1[VALUES];

		for (int i = 0; i < values.size(); i++)
		{
			auto value = values[i];

			// Make sure the value type is valid
			if (m_valueTypes.find(value[TYPE].as_string()) != m_valueTypes.end())
			{
				RegVal val;

				val.type = m_valueTypes[value[TYPE].as_string()];
				val.name = value[NAME].as_string();

				switch (val.type)
				{
				case REG_SZ:
					val.strValue = value[VALUE].as_string();
					break;

				case REG_DWORD:
					val.dwValue = value[VALUE].as_integer();
					break;
				}

				AddValue(val);
			}
			else
				throw new web::json::json_exception(L"Invalid value type.");
		}
	}
	catch (web::json::json_exception e)
	{
		std::cout << e.what() << std::endl;
	}
}

void RegistryArtifact::RootKey(HKEY root)
{
	m_hRootKey = root;
}

HKEY RegistryArtifact::RootKey() const
{
	return m_hRootKey;
}

void RegistryArtifact::Key(std::wstring strKey)
{
	m_strKey = strKey;
}

std::wstring RegistryArtifact::Key() const
{
	return m_strKey;
}

void RegistryArtifact::AddValue(RegVal value)
{
	m_values.push_back(value);
}

RegistryArtifact::~RegistryArtifact()
{
}

PhorgrError RegistryArtifact::Phorge(const std::wstring &strKillEvent)
{
	PhorgrError res(PhorgrError::ErrorUnknown);

	HKEY hKey = NULL;
	LSTATUS ret = ERROR_SUCCESS;
	DWORD dwDisp = NULL;

	if (!m_create)
		ret = RegOpenKeyEx(RootKey(), Key().c_str(), 0, KEY_SET_VALUE, &hKey);
	else
		ret = RegCreateKeyEx(RootKey(), Key().c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKey, &dwDisp);

	// If succeeded, then create values
	if (ERROR_SUCCESS == ret && NULL != hKey)
	{
		for (RegVal val : m_values)
		{
			switch (val.type)
			{
			case REG_SZ:
				ret = RegSetValueEx(hKey, val.name.c_str(), 0, REG_SZ,
									(LPBYTE)val.strValue.c_str(), (DWORD)((val.strValue.size() + 1) * sizeof(wchar_t)));
				break;

			case REG_DWORD:
				ret = RegSetValueEx(hKey, val.name.c_str(), 0, REG_DWORD,
						(LPBYTE)&val.dwValue, sizeof(val.dwValue));
				break;
			}
		}

		RegCloseKey(hKey);
	}

	return res;
}

PhorgrError RegistryArtifact::Erase(const std::wstring &strKillEvent)
{
	PhorgrError res(PhorgrError::ErrorUnknown);

	HKEY hKey = NULL;
	LSTATUS ret = ERROR_SUCCESS;
	DWORD dwDisp = NULL;

	// Open the key
	ret = RegOpenKeyEx(RootKey(), Key().c_str(), 0, KEY_SET_VALUE, &hKey);

	// Delete values
	if (ERROR_SUCCESS == ret && NULL != hKey)
	{
		for (RegVal val : m_values)
		{
			ret = RegDeleteValue(hKey, val.name.c_str());

			if (ERROR_SUCCESS != ret)
				std::cout << "Failed to delete Registry value: " << val.name.c_str() << std::endl;
		}

		RegCloseKey(hKey);
	}

	if(m_create)
		ret = RegDeleteKeyEx(RootKey(), Key().c_str(), KEY_ALL_ACCESS, 0);

	if (ERROR_SUCCESS != ret)
		std::cout << "Failed to delete Registry key: " << Key().c_str() << std::endl;

	return res;
}