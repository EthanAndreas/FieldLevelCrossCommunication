#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <ws2tcpip.h>

#include "XCom_decode.h"

#define PORT 3630

void print_usage(char *prog_name)
{
	printf("Usage: %s\n", prog_name);
}

int main(int argc, char *argv[])
{
	// Check if the IP of the client is given
	if (argc != 1)
	{
		print_usage(argv[0]);
		exit(EXIT_FAILURE);
	}

	// Needed network vars
	int socketfd, len, n = 0;
	struct sockaddr_in serveraddr, clientaddr;
	uint8_t buffer[BUFSIZ];
	XCom_Frame_t *message;
	size_t xcom_len = sizeof(message);

	// Init winsock
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		perror("WSAStartup failed. Error code : %d\n", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	// Creating socket
	if ((socketfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("Socket creation failed. Error code : %d\n", WSAGetLastError());
		WSACleanup();
		exit(EXIT_FAILURE);
	}

	// Fill struct
	memset(&serveraddr, 0, sizeof(serveraddr));
	memset(&clientaddr, 0, sizeof(clientaddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = INADDR_ANY;
	serveraddr.sin_port = htons(PORT);

	// Bind socket
	if (bind(socketfd, (const struct sockaddr *)&serveraddr,
		sizeof(serveraddr)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	(void)n;

	while (1)
	{
		// Reset vars
		n = 0;
		memset(&buffer, 0, BUFSIZ);

		// Wait for a message
		len = sizeof(clientaddr);
		n = recvfrom(socketfd, (uint8_t *)buffer, BUFSIZ,
			MSG_WAITALL, (struct sockaddr *)&clientaddr, (socklen_t *)&len);

		// Decode the message
		CHK_PTR_INT(message = XCom_decode(buffer, &xcom_len));
	}

	// Clean context
	closesocket(socketfd);
	WSACleanup();

	return EXIT_SUCCESS;
}