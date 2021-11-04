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
#define DEFAULT_IP_ADDRESS    "localhost"
#define DEFAULT_PORT          "27015"

class LogClient
{
public:
	LogClient();
	LogClient(const char *ip_address);
	~LogClient();

private:

	SOCKET m_ConnectSocket = INVALID_SOCKET;

};
