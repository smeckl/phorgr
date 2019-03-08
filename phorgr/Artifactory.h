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

#include "IArtifact.h"
#include <string>
#include <memory>

class Artifactory
{
public:
	Artifactory();
	~Artifactory();

	void AddToArtifactoryMap(const std::shared_ptr<ArtifactFactoryMap> &map);

	std::shared_ptr<IArtifact> GetArtifact(std::wstring jsonData) const;

protected:
	std::shared_ptr<ArtifactFactoryMap> m_pArtifactoryMap;

	static const std::wstring TYPE;
};

