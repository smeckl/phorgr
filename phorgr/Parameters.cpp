#include "stdafx.h"
#include "Parameters.h"


Parameters::Parameters()
{

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
