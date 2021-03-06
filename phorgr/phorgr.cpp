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

// phorgr.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"
#include <iostream>
#include <memory>
#include <list>
#include <cpprest\json.h>
#include "Utils.h"
#include "Parameters.h"
#include "IArtifact.h"
#include "ExecutionProfile.h"
#include <string>

using namespace web;

static const std::wstring PHORGE = L"phorge";
static const std::wstring ERASE = L"erase";

void printUsage()
{

}

int main(int argc, char *argv[])
{
	// Collect command-line parameters
	if (argc < 3)
	{
		printUsage();
		exit(EXIT_FAILURE);
	}

	// The first paramater tells us if we need to Phorge or Erase
	bool bPhorge = true;
	std::wstring strAction = (Utils::asciiToWideString(argv[1]));

	if (0 == strAction.compare(PHORGE))
		bPhorge = true;
	else if (0 == strAction.compare(ERASE))
		bPhorge = false;
	else
	{
		printUsage();
		exit(EXIT_FAILURE);
	}

	// The first parameter is the JSON file containing the execution script
	Parameters::getInstance().SetJSONFile(Utils::asciiToWideString(argv[2]));

	try
	{
		// Read contents of json file into a string
		std::ifstream jsonFile(Parameters::getInstance().GetJSONFile().c_str());

		std::stringstream ss;

		ss << jsonFile.rdbuf();
		const std::string jsonData = ss.str();
		jsonFile.close();

		// Convert JSON data into a wstring
		std::wstring jsonDataW(Utils::asciiToWideString(jsonData.c_str()));

		Parameters::getInstance().InitializeFromJSON(jsonDataW);

		WIN32_FIND_DATA FindFileData;
		bool bContinue = true;
		bool bFirst = true;

		// Factory object for instantiating IArtifact-based objects
		Artifactory factory;

		HANDLE hFindFile = NULL;

		// Enumerate Plugins directory and load all plugin DLLs
		while (bContinue)
		{	
			std::wstring pluginDir = Utils::concatenatePath(Parameters::getInstance().GetPluginDir().c_str(), 
															Parameters::getInstance().GetPluginDLLWildcard().c_str());
			
			if (bFirst)
			{
				hFindFile = FindFirstFile(pluginDir.c_str(), &FindFileData);
				bFirst = false;
			}

			if (NULL != hFindFile)
			{
				std::wstring fullPath = Utils::concatenatePath(Parameters::getInstance().GetPluginDir().c_str(), FindFileData.cFileName);

				// Load the phFileSystem.dll dll into memory
				HMODULE hMod = LoadLibrary(fullPath.c_str());

				if (NULL != hMod)
				{
					// Get a pointer to the function "getArtifactFactoryMap"
					pfnGetArtifactFactoryMap pFunc = (pfnGetArtifactFactoryMap)GetProcAddress(hMod, "getArtifactFactoryMap");

					// Retrieve the plugin DLL's factory map
					std::shared_ptr<ArtifactFactoryMap> pMap(pFunc());

					// Add the plugin DLL's factory map to the Artifactory's map
					factory.AddToArtifactoryMap(pMap);
				}

				// Get the next plugin DLL filename
				bContinue = FindNextFile(hFindFile, &FindFileData);
			}
			else
				bContinue = false;
		}

		// TODO:  Reconsider whether to instantite thsi directly or to use the Artifactory
		// with validation that the root element is "Execution Profile"
		std::shared_ptr<ExecutionProfile> executionProfile(new ExecutionProfile(jsonDataW, factory));

		if (bPhorge)
			executionProfile->Phorge();
		else
			executionProfile->Erase();
	}
	catch (web::json::json_exception exception)
	{

	}
	
    return 0;
}

