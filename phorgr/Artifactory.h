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

