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

	static const std::wstring REGISTRY_ARTIFACT;
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
	PhorgrError Phorge(); // Plants artifact on the system
	PhorgrError Erase();  // Removes artifact from the system (Undo)
	const std::wstring &getType() { return REGISTRY_ARTIFACT; }

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

