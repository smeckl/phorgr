#pragma once

#include <iostream>
#include "PhorgrErrors.h"

class Parameters
{
public:
	static const std::wstring CONFIG;
	static const std::wstring PLUGIN_DIR;
	static const std::wstring PLUGIN_DLL_WILDCARD;
	static const std::wstring RUN_SECONDS;
	static const std::wstring INSTANCE_MUTEX;

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

	PhorgrError InitializeFromJSON(const std::wstring &jsonString);

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

