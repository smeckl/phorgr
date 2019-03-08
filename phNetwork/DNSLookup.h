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
#include "..\headers\IArtifact.h"
#include "..\headers\PhorgrErrors.h"

class DNSLookup :
	public IArtifact
{
public:
	DNSLookup();
	~DNSLookup();

	static const std::wstring DNS_LOOKUP;
	static const std::wstring FQDN;

	static std::shared_ptr<IArtifact> createArtifact(const std::wstring &jsonData);

	// Methods required for IArtifact interface
	DNSLookup(const std::wstring &jsonData);
	PhorgrError Phorge(const std::wstring &strKillEvent); // Plants artifact on the system
	PhorgrError Erase(const std::wstring &strKillEvent);  // Removes artifact from the system (Undo)
	const std::wstring &getType() { return DNS_LOOKUP; }

	// Accessors
	void fqdn(const std::wstring &fqdn);
	const std::wstring &fqdn() const;

private:
	std::wstring m_fqdn = L"";
};

