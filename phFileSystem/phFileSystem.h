#pragma once
#include <memory>
#include <map>
#include "IArtifact.h"

#ifdef DLL_EXPORT
#  define EXPORT __declspec(dllexport)
#else
#  define EXPORT __declspec(dllimport)
#endif

extern "C"
{
	EXPORT ArtifactFactoryMap* getArtifactFactoryMap();

}