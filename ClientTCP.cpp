#include "ClientTCP.h"
#include <sstream>
#include <string>
#include "CPU_Monitor.h"
#include <thread>

ClientTCP::ClientTCP(int port) : PORT(port), versionWinSock(0x202)
{
	std::thread recv(&ClientTCP::receiveMessage, this);
	
	initLib();
	createSocket();
	createConnect();

	CPU_Monitor mon(500);
	recv.join();
	while (1)
	{
		mon.paramRequestCPU();
		sendMessage(mon.getCPU_Usage(), "UsageCPU: ");
	}

		//parsingMessage(createMessage());
}

bool ClientTCP::initLib()
{
	if (WSAStartup(versionWinSock, (WSADATA *)&buff[0]))
	{
		printf("Error WSAStartup %d\n", WSAGetLastError());
		return false;
	}

	return true;
}

bool ClientTCP::createSocket()
{
	my_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (my_sock < 0)
	{
		printf("Socket() error %d\n", WSAGetLastError());
		return false;
	}

	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(PORT);

	return true;
}

void ClientTCP::closeSocket()
{
	printf("Receive error %d\n", WSAGetLastError());
	closesocket(my_sock);
	WSACleanup();
}

bool ClientTCP::createConnect()
{
	//printf("Address server: "); fgets(&buffAddr[0], sizeof(buff) - 1, stdin);
	
	HOSTENT *hst;
	char buffAddr[] = "127.0.0.1";
	
	if (inet_addr(buffAddr) != INADDR_NONE)
		dest_addr.sin_addr.s_addr = inet_addr(buffAddr);
	else
	{
		if (hst = gethostbyname(buffAddr))
			((unsigned long *)&dest_addr.sin_addr)[0] =
			((unsigned long **)hst->h_addr_list)[0][0];
		else
		{
			printf("Invalid address %s\n", buffAddr);
			closesocket(my_sock);
			WSACleanup();
			return false;
		}
	}

	if (connect(my_sock, (sockaddr *)&dest_addr, sizeof(dest_addr)))
	{
		printf("Connect error %d\n", WSAGetLastError());
		return false;
	}

	printf("Connect to %s successful\n", buffAddr);

	return true;
}

void ClientTCP::sendMessage(double value, const char *descriptor)
{
	mt.lock();
	//const int size= sizeof(descriptor);
	//char head[size] = head & 0b100;
	std::ostringstream strs;
	strs << descriptor << value;
	std::string str = strs.str();
	char *c = const_cast<char*>(str.c_str());
	strncpy_s(buff, c, strlen(c));

	int iResult = 0;
	send(my_sock, &buff[0], strlen(&buff[0]), 0);
	if (iResult == SOCKET_ERROR)
		closeSocket();
	mt.unlock();
}

int_least8_t * ClientTCP::createMessage()
{
	int_least8_t data[2] = {0};
	data[0] = (1 << 2) & 0xFF;

	return data;
}

void ClientTCP::parsingMessage(int_least8_t * val)
{
	bool cpu_data = val[0] & 0b1;
	bool overall_data = (val[0] >> 1) & 0b1;
	bool busy_data = (val[0] >> 2) & 0b1;
}

void ClientTCP::receiveMessage()
{
	std::cout << "ID thread = " << std::this_thread::get_id() << std::endl;
	
	char buffer[1024];
	
	int nsize = 0;
	while ((nsize = recv(my_sock, &buffer[0], sizeof(buffer) - 1, 0)) != SOCKET_ERROR)
	{
		mt.lock();
		buffer[nsize] = 0;

		printf("%s", buff);
		mt.unlock();
	}

}
