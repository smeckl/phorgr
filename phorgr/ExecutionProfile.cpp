#include "stdafx.h"
#include <memory>
#include <iostream>
#include <string>
#include <stdlib.h>
#include "ExecutionProfile.h"
#include <cpprest\json.h>
#include "Artifactory.h"

using namespace web::json;

ExecutionProfile::ExecutionProfile()
{
}

ExecutionProfile::ExecutionProfile(const std::wstring &jsonData, const Artifactory &factory)
{
	try
	{
		utility::stringstream_t ss;
		ss << jsonData.c_str();
		value v = value::parse(ss);

		auto root = v[EXECUTION_PROFILE];
		auto artifacts = root[ARTIFACTS];

		for (int i = 0; i < artifacts.size(); i++)
		{
			auto artifact = artifacts[i];

			// Serialize artifact back onto string for use in generating new artifact objects
			utility::stringstream_t artifactData;
			artifact.serialize(artifactData);
			std::wstring artifactString(artifactData.str());

			std::shared_ptr<IArtifact> pArt = factory.GetArtifact(artifactString);

			m_artifacts.push_back(pArt);
		}
	}
	catch (web::json::json_exception e)
	{
		std::cout << e.what() << std::endl;
	}
}

ExecutionProfile::~ExecutionProfile()
{
}

// Method responsible for creating artifacts
PhorgrError ExecutionProfile::Phorge()
{
	PhorgrError res(PhorgrError::ErrorUnknown);

	for (std::shared_ptr<IArtifact> artifact : m_artifacts)
	{
		artifact->Phorge();
	}

	return res;
}

PhorgrError ExecutionProfile::Erase()
{
	PhorgrError res(PhorgrError::ErrorUnknown);

	// Reverse list
	m_artifacts.reverse();

	for (std::shared_ptr<IArtifact> artifact : m_artifacts)
	{
		artifact->Erase();
	}

	return res;
}
