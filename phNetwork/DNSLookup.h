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

