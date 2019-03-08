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
