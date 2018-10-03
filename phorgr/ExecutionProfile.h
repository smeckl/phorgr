#pragma once
#include <memory>
#include <list>
#include "IArtifact.h"
#include "Artifactory.h"
#include "PhorgrErrors.h"

class ExecutionProfile
{
public:
	const std::wstring EXECUTION_PROFILE = L"executionProfile";
	const std::wstring ARTIFACTS = L"artifacts";

	ExecutionProfile();
	~ExecutionProfile();

	ExecutionProfile(const std::wstring &jsonData, const Artifactory &pArtifactory);
	PhorgrError Phorge();
	PhorgrError Erase();

protected:
	std::list<std::shared_ptr<IArtifact>> m_artifacts;
};

