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