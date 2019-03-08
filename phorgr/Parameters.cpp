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
#include "Parameters.h"
#include "PhorgrErrors.h"
#include <memory>
#include <cpprest\json.h>

using namespace web::json;

const std::wstring Parameters::CONFIG = L"config";
const std::wstring Parameters::PLUGIN_DIR = L"pluginDir";
const std::wstring Parameters::PLUGIN_DLL_WILDCARD = L"pluginDLLWildcard";
const std::wstring Parameters::RUN_SECONDS = L"runSeconds";
const std::wstring Parameters::INSTANCE_MUTEX = L"instanceMUTEX";

Parameters::Parameters()
{

}

PhorgrError Parameters::InitializeFromJSON(const std::wstring &jsonString)
{
	PhorgrError retVal = PhorgrError::Success;

	utility::stringstream_t ss;
	ss << jsonString.c_str();
	value config = value::parse(ss);
	
	if (config.has_field(CONFIG))
	{
		value v1 = config[CONFIG];

		// Get plugin dir
		if (v1.has_field(PLUGIN_DIR))
			SetPluginDir(v1[PLUGIN_DIR].as_string());

		if (v1.has_field(PLUGIN_DLL_WILDCARD))
			SetPluginDLLWildcard(v1[PLUGIN_DLL_WILDCARD].as_string());
	}

	return retVal;
}

const std::wstring &Parameters::GetJSONFile() const
{
	return m_jsonFile;
}

void Parameters::SetJSONFile(const std::wstring &jsonFile)
{
	m_jsonFile = jsonFile;
}

const std::wstring &Parameters::GetPluginDir() const
{
	return m_pluginDir;
}

void Parameters::SetPluginDir(const std::wstring &pluginDir)
{
	m_pluginDir = pluginDir;
}

const std::wstring &Parameters::GetPluginDLLWildcard() const
{
	return m_pluginDLLWildcard;
}

void Parameters::SetPluginDLLWildcard(const std::wstring &pluginDir)
{
	m_pluginDLLWildcard = pluginDir;
}
