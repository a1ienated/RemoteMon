#include "ServerTCP.h"
#include <string>
#define PRINT_COUNT_USERS if (clientsCount) printf("%d user on-line\n", clientsCount); \
                else printf("No User on-line\n");

DWORD WINAPI newClient(LPVOID client_socket);
int clientsCount = 0;

ServerTCP::ServerTCP(int port) : PORT(port), versionWinSock(0x202)
{	
	initLib();
	createSocket();
	bindLocalAddress();
	listenConnect();
	processingRequest();
}

bool ServerTCP::initLib()
{
	if(WSAStartup(versionWinSock, (WSADATA *)&buff[0]))
	{
		printf("Error WSAStartup %d\n", WSAGetLastError());
		return false;
	}

	return true;
}

bool ServerTCP::createSocket()
{
	
	if ((mysocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("Error socket %d\n", WSAGetLastError());
		WSACleanup();
		return false;
	}
	
	return true;
}

bool ServerTCP::bindLocalAddress()
{
	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(PORT);
	local_addr.sin_addr.s_addr = 0;

	if (bind(mysocket, (sockaddr *)&local_addr, sizeof(local_addr)))
	{
		printf("Error bind %d\n", WSAGetLastError());
		closesocket(mysocket); // ошибка, закрыть сокет
		WSACleanup();
		return false;
	}
	return true;
}

bool ServerTCP::listenConnect()
{
	if (listen(mysocket, 0x100))
	{
		printf("Error listen %d\n", WSAGetLastError());
		closesocket(mysocket);
		WSACleanup();
		return false;
	}

	printf("Listen...\n");
	return true;
}

void ServerTCP::processingRequest()
{
	while ((client_socket = accept(mysocket, (sockaddr *)&client_addr, \
		&client_addr_size)))
	{
		clientsCount++;

		HOSTENT *hst = gethostbyaddr((char *)&client_addr.sin_addr.s_addr, 4, AF_INET);

		printf("+%s [%s] new connect!\n",
			(hst) ? hst->h_name : "", inet_ntoa(client_addr.sin_addr));
		PRINT_COUNT_USERS

			DWORD thID;
		CreateThread(NULL, NULL, newClient, &client_socket, NULL, &thID);
	}
}

DWORD WINAPI newClient(LPVOID client_socket)
{
	SOCKET my_sock;
	my_sock = ((SOCKET *)client_socket)[0];
	char buff[20 * 1024];
	char msg[] = "Socket connected\r\n";

	send(my_sock, msg, sizeof(msg), 0);

	// цикл эхо-сервера
	int bytes_recv = 0;
	while ((bytes_recv = recv(my_sock, &buff[0], sizeof(buff), 0)) && bytes_recv != SOCKET_ERROR)
	{
		buff[bytes_recv] = 0;

		printf("%s\n", buff);
		send(my_sock, &buff[0], bytes_recv, 0);
	}
	clientsCount--; // уменьшаем счетчик активных клиентов
	printf("-disconnect\n"); PRINT_COUNT_USERS

		closesocket(my_sock);
	
	return 0;
}