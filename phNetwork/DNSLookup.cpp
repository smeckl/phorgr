#include "stdafx.h"
#include "DNSLookup.h"
#include <cpprest\json.h>
#include <windns.h>   //DNS api's

using namespace web::json;

const std::wstring DNSLookup::DNS_LOOKUP = L"networkDNSLookup";
const std::wstring DNSLookup::FQDN = L"fqdn";

DNSLookup::DNSLookup()
{
}


DNSLookup::~DNSLookup()
{
}

std::shared_ptr<IArtifact> DNSLookup::createArtifact(const std::wstring &jsonData)
{
	std::shared_ptr<IArtifact> pArt(reinterpret_cast<IArtifact*>(new DNSLookup(jsonData)));

	return pArt;
}

// Methods required for IArtifact interface
DNSLookup::DNSLookup(const std::wstring &jsonData)
{
	utility::stringstream_t ss;
	ss << jsonData.c_str();
	value v = value::parse(ss);

	if (v.has_field(FQDN))
		fqdn(v[FQDN].as_string());
}

PhorgrError DNSLookup::Phorge(const std::wstring &strKillEvent)
{
	PhorgrError ret(PhorgrError::Success);
	
	PDNS_RECORD pRecord;

	if(DnsQuery_W(fqdn().c_str(), DNS_TYPE_A, DNS_QUERY_WIRE_ONLY, NULL, &pRecord, NULL))
	{
		ret = PhorgrError::NetworkDNS;
	}

	return ret;
}

PhorgrError DNSLookup::Erase(const std::wstring &strKillEvent)
{
	PhorgrError ret(PhorgrError::Success);

	// There is no way to Erase this artifact type, as once the request occurs
	// the evidence is gone from the host making the query.

	return ret;
}

// Accessors
void DNSLookup::fqdn(const std::wstring &fqdn)
{
	m_fqdn = fqdn;
}

const std::wstring &DNSLookup::DNSLookup::fqdn() const
{
	return m_fqdn;
}
