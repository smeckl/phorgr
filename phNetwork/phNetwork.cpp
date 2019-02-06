// phNetwork.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "..\headers\IArtifact.h"
#include "phNetwork.h"
#include "NetworkListener.h"
#include "HTTPGet.h"
#include "DNSLookup.h"

EXPORT ArtifactFactoryMap* getArtifactFactoryMap()
{
	ArtifactFactoryMap *pMap = new ArtifactFactoryMap();;

	// Add Aftifact class factories to the ArtifactoryMap
	pMap->emplace(NetworkListener::NETWORK_LISTENER, &NetworkListener::createArtifact);
	pMap->emplace(HTTPGet::URL_GET, &HTTPGet::createArtifact);
	pMap->emplace(DNSLookup::DNS_LOOKUP, &DNSLookup::createArtifact);

	return pMap;
}

