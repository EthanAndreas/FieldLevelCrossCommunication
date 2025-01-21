#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <io.h> 

#include "XCom_encode.h"

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
	struct sockaddr_in serveraddr;
	XCom_Frame_t message;
	memset(&message, 0, sizeof(message));
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
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = INADDR_ANY;
	serveraddr.sin_port = htons(PORT);

	// Construct the message to send and encode it
	message.id = 10;
	message.length = 3;
	message.type = 2;
	message.section.Conf.type = 7;
	message.section.Conf.section.Cycle.time.value = 1000;
	message.section.Conf.section.Cycle.time.unit = 1;
	message.section.Conf.section.Cycle.frameLoss = 8;
	uint8_t *buffer;
	CHK_PTR_INT(buffer = XCom_encode(message, xcom_len));

	// Send message
	sendto(socketfd, (const uint8_t *)buffer, sizeof(message), 0, (const struct sockaddr *)&serveraddr,
		sizeof(serveraddr));

	// Clean context
	closesocket(socketfd);
	WSACleanup();

	return EXIT_SUCCESS;
}