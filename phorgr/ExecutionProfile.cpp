#include "stdafx.h"
#include <memory>
#include <iostream>
#include <string>
#include <stdlib.h>
#include "ExecutionProfile.h"
#include <cpprest\json.h>
#include "Artifactory.h"

using namespace web::json;

const std::wstring ExecutionProfile::PHORGE = L"phorge";
const std::wstring ExecutionProfile::ERASE = L"erase";
const std::wstring ExecutionProfile::EXECUTION_PROFILE = L"executionProfile";
const std::wstring ExecutionProfile::ARTIFACTS = L"artifacts";
const std::wstring ExecutionProfile::RUN_SECONDS = L"runSeconds";
const std::wstring ExecutionProfile::INSTANCE_MUTEX = L"instanceMUTEX";
const std::wstring ExecutionProfile::KILL_PROCESS_EVENT = L"killProcessEvent";

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

		if (root.has_field(RUN_SECONDS))
		{
			value runSeconds = root[RUN_SECONDS];

			if (runSeconds.is_integer())
				SetRunSeconds(runSeconds.as_integer());
		}

		if (root.has_field(INSTANCE_MUTEX))
		{
			SetInstanceMUTEX(root[INSTANCE_MUTEX].as_string());
		}

		if (root.has_field(KILL_PROCESS_EVENT))
		{
			SetKillProcessEvent(root[KILL_PROCESS_EVENT].as_string());
		}

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
	if(NULL != m_hMutex)
		CloseHandle(m_hMutex);
}

// Method responsible for creating artifacts
PhorgrError ExecutionProfile::Phorge()
{
	PhorgrError res(PhorgrError::Success);

	if (LockMUTEX(true))
	{
		for (std::shared_ptr<IArtifact> artifact : m_artifacts)
		{
			artifact->Phorge(GetKillProcessEvent());
		}

		// Run for specified number of seconds
		if (GetRunSeconds() > 0 && GetKillProcessEvent().length() > 0)
		{
			HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, GetKillProcessEvent().c_str());

			if (NULL != hEvent)
			{
				WaitForSingleObject(hEvent, GetRunSeconds() * 1000);
			}
		}

		UnlockMUTEX();
	}

	return res;
}

PhorgrError ExecutionProfile::Erase()
{
	PhorgrError res(PhorgrError::Success);

	if (LockMUTEX(false))
	{
		// If there is a kill proc event in the config file, then signal it
		if (GetKillProcessEvent().length() > 0)
		{
			HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, GetKillProcessEvent().c_str());

			if (NULL != hEvent)
			{
				SetEvent(hEvent);
			}
		}

		// Reverse list
		m_artifacts.reverse();

		for (std::shared_ptr<IArtifact> artifact : m_artifacts)
		{
			artifact->Erase(GetKillProcessEvent());
		}

		UnlockMUTEX();
	}

	return res;
}

const int ExecutionProfile::GetRunSeconds()
{
	return m_runSeconds;
}

void ExecutionProfile::SetRunSeconds(int runSeconds)
{
	m_runSeconds = runSeconds;
}

const std::wstring &ExecutionProfile::GetInstanceMUTEX()
{
	return m_instanceMUTEX;
}

void ExecutionProfile::SetInstanceMUTEX(const std::wstring &instanceMUTEX)
{
	m_instanceMUTEX = instanceMUTEX;
}

const std::wstring &ExecutionProfile::GetKillProcessEvent()
{
	return m_killProcessEvent;
}

void ExecutionProfile::SetKillProcessEvent(const std::wstring &killProcessEvent)
{
	m_killProcessEvent = killProcessEvent;
}

bool ExecutionProfile::LockMUTEX(bool bPhorge)
{
	bool bRet(false);

	// Check for MUTEX before continuing so we don't get multiple copies running
	if (0 != GetInstanceMUTEX().length())
	{
		std::wstringstream strMutex;

		strMutex << GetInstanceMUTEX().c_str();

		if (bPhorge)
			strMutex << L"-" << PHORGE;
		else
			strMutex << L"-" << ERASE;

		m_hMutex = CreateMutex(NULL, FALSE, strMutex.str().c_str());

		if (NULL == m_hMutex || WAIT_OBJECT_0 != WaitForSingleObject(m_hMutex, 0))
		{
			std::wcout << L"Another copy of this execution profile is running.  Exiting..." << std::endl;
		}
		else
			bRet = true;
	}

	return bRet;
}

void ExecutionProfile::UnlockMUTEX()
{
	if (NULL != m_hMutex)
	{
		ReleaseMutex(m_hMutex);
	}
}