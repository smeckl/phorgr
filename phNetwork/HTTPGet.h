#pragma once
#include <string>
#include "..\headers\IArtifact.h"
#include "..\headers\PhorgrErrors.h"

class HTTPGet : public IArtifact
{
public:
	HTTPGet();
	~HTTPGet();

	static const std::wstring URL_GET;
	static const std::wstring URL;

	static std::shared_ptr<IArtifact> createArtifact(const std::wstring &jsonData);

	// Methods required for IArtifact interface
	HTTPGet(const std::wstring &jsonData);
	PhorgrError Phorge(const std::wstring &strKillEvent); // Plants artifact on the system
	PhorgrError Erase(const std::wstring &strKillEvent);  // Removes artifact from the system (Undo)
	const std::wstring &getType() { return URL_GET; }

	// Accessors
	void Url(const std::wstring &protocol);
	const std::wstring &Url() const;

private:
	std::wstring m_url = L"";
};

