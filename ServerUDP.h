#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment ( lib, "ws2_32.lib" )
#include <stdio.h>
#include <winsock2.h>

class ServerUDP
{
public:
	ServerUDP(int port);
	//~server();

	void initLib();
	void createSocket();
	void bindLocalAddress();

	const int PORT;
	char buff[1024];
	SOCKET Socket;
	sockaddr_in local_addr;


};

