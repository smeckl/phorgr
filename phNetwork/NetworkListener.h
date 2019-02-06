#pragma once
#include <string>
#include "..\headers\IArtifact.h"
#include "..\headers\PhorgrErrors.h"

class NetworkListener : public IArtifact
{
public:
	NetworkListener();
	~NetworkListener();

	static const std::wstring NETWORK_LISTENER;
	static const std::wstring PROTOCOL;
	static const std::wstring PORT;
	static const std::wstring TCP;
	static const std::wstring UDP;

	static std::shared_ptr<IArtifact> createArtifact(const std::wstring &jsonData);

	// Methods required for IArtifact interface
	NetworkListener(const std::wstring &jsonData);
	PhorgrError Phorge(const std::wstring &strKillEvent); // Plants artifact on the system
	PhorgrError Erase(const std::wstring &strKillEvent);  // Removes artifact from the system (Undo)
	const std::wstring &getType() { return NETWORK_LISTENER; }

	// Accessors
	void Protocol(const std::wstring &protocol);
	const std::wstring &Protocol() const;

	void Port(const int port);
	int Port() const;

private:
	std::wstring m_protocol = L"";
	int m_port = 0;
};

