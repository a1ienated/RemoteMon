#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <stdint.h>
#include <mutex>

class ClientTCP
{
public:
	ClientTCP(int port);
	
private:
	char buff[1024];
	char buffAddr[1024];
	const int PORT;
	const int versionWinSock;
	SOCKET my_sock;
	sockaddr_in dest_addr;
	std::mutex mt;

	bool initLib();
	bool createSocket();
	void closeSocket();
	bool createConnect();
	void sendMessage(double value, const char *descriptor);
	int_least8_t *createMessage(); //реализиция протокола
	void parsingMessage(int_least8_t *val); //для примера
	void receiveMessage();
};

