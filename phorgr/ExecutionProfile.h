#pragma once
#include "windows.h"
#include <memory>
#include <list>
#include "IArtifact.h"
#include "Artifactory.h"
#include "PhorgrErrors.h"

class ExecutionProfile
{
public:
	static const std::wstring EXECUTION_PROFILE;
	static const std::wstring ARTIFACTS;
	static const std::wstring PHORGE;
	static const std::wstring ERASE;
	static const std::wstring RUN_SECONDS;
	static const std::wstring INSTANCE_MUTEX;
	static const std::wstring KILL_PROCESS_EVENT;

	ExecutionProfile();
	~ExecutionProfile();

	ExecutionProfile(const std::wstring &jsonData, const Artifactory &pArtifactory);
	PhorgrError Phorge();
	PhorgrError Erase();

	const int GetRunSeconds();
	void SetRunSeconds(int runSeconds);

	const std::wstring &GetInstanceMUTEX();
	void SetInstanceMUTEX(const std::wstring &instanceMUTEX);

	const std::wstring &GetKillProcessEvent();
	void SetKillProcessEvent(const std::wstring &killProcessEvent);

protected:
	HANDLE m_hMutex = NULL;
	std::list<std::shared_ptr<IArtifact>> m_artifacts;
	int m_runSeconds = 0;
	std::wstring m_instanceMUTEX = L"";
	std::wstring m_killProcessEvent = L"";

	bool LockMUTEX(bool bPhorge);
	void UnlockMUTEX();
};

