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
#include "Artifactory.h"
#include <cpprest\json.h>
#include <memory>

using namespace web::json;

const std::wstring Artifactory::TYPE = L"type";

Artifactory::Artifactory()
{
	m_pArtifactoryMap.reset(new ArtifactFactoryMap());
}


Artifactory::~Artifactory()
{

}

void Artifactory::AddToArtifactoryMap(const std::shared_ptr<ArtifactFactoryMap> &map)
{
	ArtifactFactoryMap::iterator it;

	for (it = map->begin(); it != map->end(); it++)
	{
		m_pArtifactoryMap->emplace(it->first, it->second);
	}
}

std::shared_ptr<IArtifact> Artifactory::GetArtifact(std::wstring jsonData) const
{
	std::shared_ptr<IArtifact> pArt(NULL);

	try
	{
		// Parse value
		utility::stringstream_t ss;
		ss << jsonData.c_str();
		value v1 = value::parse(ss);

		// Get "type" value
		auto root = v1[TYPE];

		std::wstring strType(root.as_string().c_str());
		
		pfnArtifactFactory pFunc = m_pArtifactoryMap->at(strType.c_str());

		// Build object based on type
		if(NULL != pFunc)
		{
			pArt = pFunc(jsonData);
		}
	}
	catch (web::json::json_exception e)
	{
		std::cout << e.what() << std::endl;
	}

	return pArt;
}