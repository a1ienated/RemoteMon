#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include "General.h"

class ServerTCP
{
public:
	ServerTCP(int port);
	
private:
	const int PORT;
	const int versionWinSock;
	
	
	char buff[1024];
	SOCKET mysocket;
	sockaddr_in local_addr;
	SOCKET client_socket;
	sockaddr_in client_addr;

	int client_addr_size = sizeof(client_addr);

	bool initLib();
	bool createSocket();
	bool bindLocalAddress();
	bool listenConnect();
	void processingRequest();
};

