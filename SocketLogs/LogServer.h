#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <Windows.h>
#include <stdarg.h>
#include <thread>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN        512
#define MAX_CLIENTS           1

class LogServer
{
public:
	LogServer(const char *port);
	~LogServer();

	bool SendString(const char* buf, ...);

private:

	const char* m_Port;
	std::thread m_ListenerThread;

	SOCKET m_ListenSocket = INVALID_SOCKET;
	SOCKET m_ClientSocket = INVALID_SOCKET;

	void ListenerThread();
};
