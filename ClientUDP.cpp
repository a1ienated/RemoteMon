#include "ClientUDP.h"
#include <sstream>
#include <string>
#include "CPU_Monitor.h"
#include <thread>

ClientUDP::ClientUDP(int port) : PORT(port), versionWinSock(0x202)
{
	std::thread recv(&ClientUDP::receiveMessage, this);

	initLib();
	createSocket();
	determineAddress();
	
	CPU_Monitor mon(500);


	while (1)
	{
		mon.paramRequestCPU();
		sendMessage(mon.getCPU_Usage(), "CPU_Usage: ");
	}
	recv.join();
}

void ClientUDP::initLib()
{
	if (WSAStartup(versionWinSock, (WSADATA *)&buff[0]))
		printf("WSAStartup error: %d\n", WSAGetLastError());
}

void ClientUDP::createSocket()
{
	my_sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (my_sock == INVALID_SOCKET)
	{
		printf("socket() error: %d\n", WSAGetLastError());
		WSACleanup();
	}
}

void ClientUDP::closeSocket()
{
	printf("Receive error %d\n", WSAGetLastError());
	closesocket(my_sock);
	WSACleanup();
}

void ClientUDP::determineAddress()
{
	//printf("Address server: "); fgets(&buffAddr[0], sizeof(buff) - 1, stdin);

	char buffAddr[] = "127.0.0.1";

	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(PORT);

	if (inet_addr(buffAddr))
		dest_addr.sin_addr.s_addr = inet_addr(buffAddr);
	else
		if (hst = gethostbyname(buffAddr))
			dest_addr.sin_addr.s_addr = ((unsigned long **)
				hst->h_addr_list)[0][0];
		else
		{
			printf("Unknown host: %d\n", WSAGetLastError());
			closesocket(my_sock);
			WSACleanup();
		}
}

void ClientUDP::sendMessage(double value, const char *descriptor)
{
	std::ostringstream strs;
	strs << descriptor << value;
	std::string str = strs.str();
	char *c = const_cast<char*>(str.c_str());
	strncpy_s(buff, c, strlen(c));

	int iResult = 0;
	iResult = sendto(my_sock, &buff[0], strlen(&buff[0]), 0, (sockaddr *)&dest_addr, sizeof(dest_addr));
	if (iResult == SOCKET_ERROR)
		closeSocket();
}

void ClientUDP::receiveMessage()
{
	std::cout << "ID thread = " << std::this_thread::get_id() << std::endl;
	int nsize = 0;
	while ((nsize = recv(my_sock, &buff[0], sizeof(buff) - 1, 0)) != SOCKET_ERROR)
	{
		buff[nsize] = 0;
		
		printf("%s", buff);
	}
}
