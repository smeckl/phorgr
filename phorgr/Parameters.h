#pragma once

#include <iostream>

class Parameters
{
public:
	static Parameters& getInstance()
	{
		static Parameters    instance;
		return instance;
	}
private:
	Parameters();                    // Constructor? (the {} brackets) are needed here.

	//Parameters(Parameters const&);     // Don't Implement
	//void operator=(Parameters const&); // Don't implement

public:
	Parameters(Parameters const&) = delete;
	void operator=(Parameters const&) = delete;

	const std::wstring &GetJSONFile() const;
	void SetJSONFile(const std::wstring &jsonFile);

	const std::wstring &GetPluginDir() const;
	void SetPluginDir(const std::wstring &jsonFile);

	const std::wstring &GetPluginDLLWildcard() const;
	void SetPluginDLLWildcard(const std::wstring &jsonFile);

private:
	std::wstring m_jsonFile = L"";
	std::wstring m_pluginDir = L".\\plugins";
	std::wstring m_pluginDLLWildcard = L"ph*.dll";
};

