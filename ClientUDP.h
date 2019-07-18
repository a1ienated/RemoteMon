#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
//#include <stdio.h>
#pragma comment ( lib, "ws2_32.lib" )
#include <winsock2.h>
//#include <cstdio>
//#include <windows.h>
//#include <stdlib.h>
//#include <iostream>


class ClientUDP
{
public:
	ClientUDP(int port);

private:
	const int PORT;
	char buffAddr[1024];
	char buff[10 * 1014];

	SOCKET my_sock;
	HOSTENT *hst;
	sockaddr_in dest_addr;
	const int versionWinSock;

	void initLib();
	void createSocket();
	void closeSocket();
	void determineAddress();

	void sendMessage(double value, const char *descriptor);
	void receiveMessage();
	//static entryPoint
};

