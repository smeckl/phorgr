#pragma once

#include <string>
#include <map>
#include <memory>
#include "PhorgrErrors.h"

class IArtifact
{
public:

	IArtifact() {}
	IArtifact(const std::wstring &jsonData) {} // Build artifact class from serialized JSON data
	virtual ~IArtifact() {}

	virtual PhorgrError Phorge(const std::wstring &strKillEvent) = 0; // Plants artifact on the system
	virtual PhorgrError Erase(const std::wstring &strKillEvent) = 0;  // Removes artifact from the system (Undo)

	virtual const std::wstring &getType() = 0;
};

// Function pointer definition for artifact factory function
typedef std::shared_ptr<IArtifact> (*pfnArtifactFactory)(const std::wstring &);

// Object mapping artifact type to artifact factory function
typedef std::map<const std::wstring, pfnArtifactFactory> ArtifactFactoryMap;

// Function pointer definition for building factory map
typedef ArtifactFactoryMap*(*pfnGetArtifactFactoryMap)();