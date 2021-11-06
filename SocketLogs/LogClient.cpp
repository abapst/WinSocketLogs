#include "LogClient.h"

LogClient::LogClient(const char *ip_address, const char *port)
{
	WSADATA wsaData;
	struct addrinfo* result = NULL,
		* ptr = NULL,
		hints;
	const char* sendbuf = "this is a test";
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		printf("WSAStartup failed with error: %d\n", iResult);
		return;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(ip_address, port, &hints, &result);
	if (iResult != 0)
	{
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
	{
		// Create a SOCKET for connecting to server
		m_ConnectSocket = socket(
			ptr->ai_family,
			ptr->ai_socktype,
			ptr->ai_protocol);
		if (m_ConnectSocket == INVALID_SOCKET)
		{
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return;
		}

		// Connect to server.
		iResult = connect(m_ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR)
		{
			closesocket(m_ConnectSocket);
			m_ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (m_ConnectSocket == INVALID_SOCKET)
	{
		printf("Unable to connect to server!\n");
		WSACleanup();
		return;
	}

	printf("Started client connected to %s on port %s\n", ip_address, port);

	// shutdown the connection since no more data will be sent
	iResult = shutdown(m_ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR)
	{
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(m_ConnectSocket);
		WSACleanup();
		return;
	}

	// Receive until the peer closes the connection
	do
	{
		iResult = recv(m_ConnectSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
		{
			std::cout << "Received: " << std::string(recvbuf).substr(0, iResult);
		}
		else if (iResult == 0)
			continue;
		else
			printf("recv failed with error: %d\n", WSAGetLastError());

	} while (iResult >= 0);
}

LogClient::~LogClient()
{
	// cleanup
	closesocket(m_ConnectSocket);
	WSACleanup();
}

bool LogClient::SendString(const char* format, ...)
{
    char buf[256];
    va_list args;
    va_start(args, format);
    vsprintf_s(buf, format, args);
    va_end(args);

    int iSendResult = send(m_ConnectSocket, buf, (int)strlen(buf), 0);
    if (iSendResult == SOCKET_ERROR)
    {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(m_ConnectSocket);
        WSACleanup();
        return false;
    }
	printf("Sent: %s", buf);
    return true;
}

