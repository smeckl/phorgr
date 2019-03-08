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
#include "IArtifact.h"
#include "PhorgrErrors.h"
#include <string>
#include <memory>
#include <list>

typedef struct regval_t
{
	std::wstring name;
	DWORD type;
	std::wstring strValue;
	DWORD dwValue;
} RegVal;

class RegistryArtifact : public IArtifact
{
public:
	static std::shared_ptr<IArtifact> createArtifact(const std::wstring &jsonData);

	static const std::wstring REGISTRY;
	static const std::wstring KEY;
	static const std::wstring VALUES;
	static const std::wstring NAME;
	static const std::wstring VALUE;
	static const std::wstring CREATE;
	static const std::wstring ROOT_KEY;
	static const std::wstring TYPE;

	RegistryArtifact();
	~RegistryArtifact();

	// Methods required for IArtifact interface
	RegistryArtifact(const std::wstring &jsonData);
	PhorgrError Phorge(const std::wstring &strKillEvent); // Plants artifact on the system
	PhorgrError Erase(const std::wstring &strKillEvent);  // Removes artifact from the system (Undo)
	const std::wstring &getType() { return REGISTRY; }

	// Accessors
	void Key(std::wstring strKey);
	std::wstring Key() const;

	void RootKey(HKEY root);
	HKEY RootKey() const;

	void AddValue(RegVal value);

protected:
	HKEY m_hRootKey = NULL;
	std::wstring m_strKey;
	std::list<RegVal> m_values;
	bool m_create;

	static std::map<std::wstring, HKEY> m_rootKeys;
	static std::map<std::wstring, DWORD> m_valueTypes;
};

