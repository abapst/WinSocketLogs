#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN        512

class LogClient
{
public:
	LogClient(const char *ip_address, const char *port);
	~LogClient();

	bool SendString(const char* buf);

private:

	SOCKET m_ConnectSocket = INVALID_SOCKET;

};
