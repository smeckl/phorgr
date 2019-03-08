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
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cwctype>
#include <string>
#include <cpprest\json.h>
#include "NetworkListener.h"

using namespace web::json;

const std::wstring NetworkListener::NETWORK_LISTENER = L"networkListener";
const std::wstring NetworkListener::PROTOCOL = L"protocol";
const std::wstring NetworkListener::PORT = L"port";
const std::wstring NetworkListener::TCP = L"tcp";
const std::wstring NetworkListener::UDP = L"udp";

typedef struct accept_thread_params_t
{
	HANDLE killEvent;
	NetworkListener *pListenerArtifact;
} AcceptParams;

DWORD WINAPI AcceptConnection(LPVOID lpParam);

std::shared_ptr<IArtifact> NetworkListener::createArtifact(const std::wstring &jsonData)
{
	std::shared_ptr<IArtifact> pArt(reinterpret_cast<IArtifact*>(new NetworkListener(jsonData)));

	return pArt;
}

NetworkListener::NetworkListener()
{

}


NetworkListener::~NetworkListener()
{

}

NetworkListener::NetworkListener(const std::wstring &jsonData)
{
	utility::stringstream_t ss;
	ss << jsonData.c_str();
	value v = value::parse(ss);

	if (v.has_field(PROTOCOL))
		Protocol(v[PROTOCOL].as_string());

	if (v.has_field(PORT))
	{
		value port = v[PORT];

		if (port.is_integer())
			Port(port.as_integer());
	}
}

void NetworkListener::Protocol(const std::wstring &protocol)
{
	m_protocol = protocol;
}

const std::wstring &NetworkListener::Protocol() const
{
	return m_protocol;
}

void NetworkListener::Port(const int port)
{
	m_port = port;
}

int NetworkListener::Port() const
{
	return m_port;
}

PhorgrError NetworkListener::Phorge(const std::wstring &strKillEvent)
{
	PhorgrError retVal(PhorgrError::Success);

	HANDLE killEvent = CreateEventExW(NULL, strKillEvent.c_str(), 0, EVENT_ALL_ACCESS);

	AcceptParams *pParams(new AcceptParams);

	pParams->killEvent = killEvent;
	pParams->pListenerArtifact = new NetworkListener(*this);

	DWORD dwThreadID(0);

	// We assume the threadproc will free the parameters
	CreateThread(NULL, 0, AcceptConnection, (LPVOID)pParams, 0, &dwThreadID);

	return retVal;
}

DWORD WINAPI AcceptConnection(LPVOID lpParam)
{
	DWORD dwRet = ERROR_SUCCESS;
	PhorgrError retVal(PhorgrError::Success);

	WSADATA wsaData;
	int iResult;

	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;

	struct addrinfo *result = NULL;
	struct addrinfo hints;

	AcceptParams *pParams = NULL;

	int iSendResult;

	for (;;)
	{
		if (NULL != lpParam)
			pParams = (AcceptParams*)lpParam;
		else
		{
			dwRet = ERROR_NOT_FOUND;
			break;
		}

		// Initialize Winsock
		iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (0 != iResult)
		{
			std::wcout << "WSAStartup() failed with error: " << iResult << std::endl;
			retVal = PhorgrError::NetworkSetup;
			break;
		}

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;

		if (0 == pParams->pListenerArtifact->Protocol().compare(pParams->pListenerArtifact->TCP))
		{
			hints.ai_protocol = IPPROTO_TCP;
			hints.ai_socktype = SOCK_STREAM;
		}
		else if (0 == pParams->pListenerArtifact->Protocol().compare(pParams->pListenerArtifact->UDP))
		{
			hints.ai_protocol = IPPROTO_UDP;
			hints.ai_socktype = SOCK_DGRAM;
		}

		hints.ai_flags = AI_PASSIVE;

		// Resolve the server address and port
		iResult = getaddrinfo(NULL, std::to_string(pParams->pListenerArtifact->Port()).c_str(), &hints, &result);
		if (iResult != 0)
		{
			std::wcout << "getaddrinfo() failed with error: %d\n" << iResult << std::endl;
			retVal = PhorgrError::NetworkSetup;
			break;
		}

		// Create a SOCKET for connecting to server
		ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (INVALID_SOCKET == ListenSocket)
		{
			std::wcout << "socket failed with error: " << WSAGetLastError() << std::endl;
			retVal = PhorgrError::NetworkSetup;
			break;
		}

		// Setup the TCP listening socket
		iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
		if (SOCKET_ERROR == iResult)
		{
			std::wcout << "bind failed with error: %d\n" << WSAGetLastError() << std::endl;
			retVal = PhorgrError::NetworkSetup;
			break;
		}

		iResult = listen(ListenSocket, SOMAXCONN);
		if (iResult == SOCKET_ERROR)
		{
			std::wcout << "listen failed with error: %d\n" << WSAGetLastError() << std::endl;
			retVal = PhorgrError::NetworkSetup;
			break;
		}

		HANDLE socketEventHandle = WSACreateEvent();
		WSAEventSelect(ListenSocket, socketEventHandle, FD_ACCEPT | FD_CLOSE);

		HANDLE events[2] = { pParams->killEvent, socketEventHandle };

		DWORD dwEvent = WaitForMultipleObjects(2, events, false, INFINITE);

		if (WAIT_OBJECT_0 == dwEvent)  // Kill Event Triggered
		{

		}
		else if (WAIT_OBJECT_0 + 1 == dwEvent) // Socket event triggered
		{
			// Accept a client socket
			ClientSocket = accept(ListenSocket, NULL, NULL);
			if (ClientSocket == INVALID_SOCKET)
			{
				std::wcout << "accept failed with error: %d\n" << WSAGetLastError() << std::endl;
				retVal = PhorgrError::NetworkSetup;
				break;
			}

			char recvbuf[1024];

			// Receive until the peer shuts down the connection
			do {
				DWORD dwEvt = WaitForSingleObject(pParams->killEvent, 0);

				if (WAIT_OBJECT_0 == dwEvt)
					iResult = 0;
				else
					iResult = recv(ClientSocket, recvbuf, 1024, 0);

				if (iResult > 0)
				{
					std::wcout << "Bytes received: " << iResult << std::endl;

					// Echo the buffer back to the sender
					iSendResult = send(ClientSocket, recvbuf, iResult, 0);
					if (SOCKET_ERROR == iSendResult)
					{
						std::wcout << "send failed with error: " << WSAGetLastError() << std::endl;
						retVal = PhorgrError::NetworkSend;
						break;
					}

					std::wcout << "Bytes sent: %d\n" << iSendResult << std::endl;
				}
				else if (-1 == iResult)
				{
					if (WSAEWOULDBLOCK == WSAGetLastError())
					{
						iResult = 1;
					}
				}
				else if (0 == iResult)
				{
					std::wcout << "Connection closing...\n" << std::endl;
					break;
				}
				else
				{
					std::wcout << "recv failed with error: %d\n" << WSAGetLastError() << std::endl;
					retVal = PhorgrError::NetworkReceive;
					break;
				}

			} while (iResult > 0 && PhorgrError::Success == retVal);

			// shutdown the connection since we're done
			iResult = shutdown(ClientSocket, SD_SEND);

			if (SOCKET_ERROR == iResult)
			{
				std::wcout << "shutdown failed with error: " << WSAGetLastError() << std::endl;
				retVal = PhorgrError::NetworkReceive;
				break;
			}
		}

		break;
	}

	// cleanup
	if (NULL != result)
		freeaddrinfo(result);

	if (NULL != pParams)
	{
		if (NULL != pParams->pListenerArtifact)
			delete pParams->pListenerArtifact;

		delete pParams;
	}

	closesocket(ClientSocket);
	closesocket(ListenSocket);
	WSACleanup();

	return dwRet;
}

PhorgrError NetworkListener::Erase(const std::wstring &strKillEvent)
{
	PhorgrError retVal(PhorgrError::Success);


	return retVal;
}