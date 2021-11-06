#include <string>
#include "LogServer.h"
#include "Utils.h"

LogServer::LogServer(const char *port)
{
    m_Port = port;

    // Start listener thread
    m_ListenerThread = std::thread(&LogServer::ListenerThread, this);
}

LogServer::~LogServer()
{
    printf("Shutting down server.");
    int iResult = shutdown(m_ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR)
    {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
    }

	// cleanup
	closesocket(m_ClientSocket);
	WSACleanup();
}

bool LogServer::SendString(const char* format, ...)
{
    char buf[256];
    va_list args;
    va_start(args, format);
    vsprintf_s(buf, format, args);
    va_end(args);

    int iSendResult = send(m_ClientSocket, buf, (int)strlen(buf), 0);
    if (iSendResult == SOCKET_ERROR)
    {
        printf("Send failed with error: %d\n", WSAGetLastError());
        return false;
    }
	printf("Sent: %s", buf);
    return true;
}

void LogServer::ListenerThread()
{
	WSADATA wsaData;
	int iResult;

    struct addrinfo* result = NULL;
    struct addrinfo hints;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        printf("WSAStartup failed with error: %d\n", iResult);
        return;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, m_Port, &hints, &result);
    if (iResult != 0)
    {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return;
    }

    // Create a SOCKET for connecting to server
    m_ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (m_ListenSocket == INVALID_SOCKET)
    {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return;
    }

    // Setup the TCP listening socket
    iResult = bind(m_ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR)
    {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(m_ListenSocket);
        WSACleanup();
        return;
    }

    freeaddrinfo(result);

    iResult = listen(m_ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR)
    {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(m_ListenSocket);
        WSACleanup();
        return;
    }

	printf("Started server listening on port %s\n", m_Port);
    printf("Waiting for connection...\n");

	struct sockaddr_in clientAddr = { 0 };
	socklen_t socklen = sizeof(sockaddr_in);
    while (m_ClientSocket = accept(m_ListenSocket, (struct sockaddr *) &clientAddr, &socklen))
    {
		if (m_ClientSocket == INVALID_SOCKET)
		{
			printf("accept failed with error: %d\n", WSAGetLastError());
			closesocket(m_ListenSocket);
			WSACleanup();
			return;
		}
		printClientInfo(clientAddr);
    }

    // No longer need server listening socket
    closesocket(m_ListenSocket);
}
