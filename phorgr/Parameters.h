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

