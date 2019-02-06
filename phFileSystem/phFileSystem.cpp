// phFileSystem.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "IArtifact.h"
#include "phFileSystem.h"
#include "RegistryArtifact.h"


EXPORT ArtifactFactoryMap* getArtifactFactoryMap()
{
	ArtifactFactoryMap *pMap = new ArtifactFactoryMap();;

	pMap->emplace(RegistryArtifact::REGISTRY, &RegistryArtifact::createArtifact);

	return pMap;
}