#include "ServerUDP.h"

ServerUDP::ServerUDP(int port) : PORT(port)
{
	initLib();
	createSocket();
	bindLocalAddress();

	// обработка пакетов, присланных клиентами
	while (1)
	{
		sockaddr_in client_addr;
		int client_addr_size = sizeof(client_addr);
		int bsize = recvfrom(Socket, &buff[0], sizeof(buff) - 1, 0, (sockaddr *)&client_addr, &client_addr_size);
		if (bsize == SOCKET_ERROR) {
			printf("recvfrom() error: %d\n", WSAGetLastError());
		};
		
		HOSTENT *hst;
		hst = gethostbyaddr((char *)&client_addr.sin_addr, 4, AF_INET);
		//printf("+%s [%s:%d] new DATAGRAM!\n", (hst) ? hst->h_name : "Unknown host", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

		// добавление завершающего нуля
		buff[bsize] = 0;

		printf("%s\n", &buff[0]);

		sendto(Socket, &buff[0], bsize, 0, (sockaddr *)&client_addr, sizeof(client_addr));
	}
}

void ServerUDP::initLib()
{
	if (WSAStartup(0x202, (WSADATA *)&buff[0]))
		printf("WSAStartup error: %d\n", WSAGetLastError());
}

void ServerUDP::createSocket()
{
	Socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (Socket == INVALID_SOCKET)
	{
		printf("Socket() error: %d\n", WSAGetLastError());
		WSACleanup();
	}
}

void ServerUDP::bindLocalAddress()
{
	local_addr.sin_family = AF_INET;
	local_addr.sin_addr.s_addr = INADDR_ANY;
	local_addr.sin_port = htons(PORT);

	if (bind(Socket, (sockaddr *)&local_addr, sizeof(local_addr)))
	{
		printf("bind error: %d\n", WSAGetLastError());
		system("pause");
		closesocket(Socket);
		WSACleanup();
	}
}
