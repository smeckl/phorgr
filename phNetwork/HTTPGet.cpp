#include "stdafx.h"
#include "HTTPGet.h"
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>

using namespace web::json;
using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams

const std::wstring HTTPGet::URL_GET = L"networkHTTPGet";
const std::wstring HTTPGet::URL = L"url";

std::shared_ptr<IArtifact> HTTPGet::createArtifact(const std::wstring &jsonData)
{
	std::shared_ptr<IArtifact> pArt(reinterpret_cast<IArtifact*>(new HTTPGet(jsonData)));

	return pArt;
}

HTTPGet::HTTPGet()
{
}


HTTPGet::~HTTPGet()
{
}

void HTTPGet::Url (const std::wstring &url)
{
	m_url = url;
}

const std::wstring &HTTPGet::Url() const
{
	return m_url;
}

HTTPGet::HTTPGet(const std::wstring &jsonData)
{
	utility::stringstream_t ss;
	ss << jsonData.c_str();
	value v = value::parse(ss);

	if (v.has_field(URL))
		Url(v[URL].as_string());
}

PhorgrError HTTPGet::Phorge(const std::wstring &strKillEvent)
{
	PhorgrError ret(PhorgrError::Success);

	auto fileStream = std::make_shared<ostream>();

	// Open stream to output file.
	pplx::task<void> requestTask = fstream::open_ostream(L"results.html").then([=](ostream outFile)
	{
		*fileStream = outFile;

		// Create http_client to send the request.
		http_client client(Url());

		// Build request URI and start the request.
		uri_builder builder(L"");
		return client.request(methods::GET, builder.to_string());
	})
	.then([=](http_response response)
	{
		printf("Received response status code:%u\n", response.status_code());

		// Write response body into the file.
		return response.body().read_to_end(fileStream->streambuf());
	})
	.then([=](size_t)
	{
		return fileStream->close();
	});

	// Wait for all the outstanding I/O to complete and handle any exceptions
	try
	{
		requestTask.wait();
	}
	catch (const std::exception &e)
	{
		printf("Error exception:%s\n", e.what());
	}

	return ret;
}

PhorgrError HTTPGet::Erase(const std::wstring &strKillEvent)
{
	PhorgrError ret(PhorgrError::Success);


	return ret;
}
