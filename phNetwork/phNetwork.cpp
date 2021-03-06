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

