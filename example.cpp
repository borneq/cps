﻿#include "ClientSocket.h"
#include "ServerSocket.h"
#include "Exception.h"
#include <stdlib.h>
#include <stdio.h>

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

void make_client()
{
	const char* sendbuf = "this is a test";
	char recvbuf[DEFAULT_BUFLEN];
	
	int recvbuflen = DEFAULT_BUFLEN;

	// Validate the parameters
	ClientSocket sock;
	// Resolve the server address and port
	sock.resolve("localhost", DEFAULT_PORT);
	sock.connect();
	int iResult = sock.send(sendbuf, strlen(sendbuf));
	printf("Bytes Sent: %ld\n", iResult);
	sock.closeSend();
	// Receive until the peer closes the connection
	do {
		iResult = sock.receive(recvbuf, recvbuflen);
		if (iResult > 0)
			printf("Bytes received: %d\n", iResult);
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed with error: %d\n", WSAGetLastError());

	} while (iResult > 0);	
}

int make_server()
{
	int iSendResult;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	ServerSocket sock;
	sock.resolve(DEFAULT_PORT);
	sock.listen();
	   
	ClientSocket csock(&sock);
	
	// Receive until the peer shuts down the connection
	int iResult = 0;
	do {
		iResult = csock.receive(recvbuf, recvbuflen);
		if (iResult > 0) {
			printf("Bytes received: %d\n", iResult);
			// Echo the buffer back to the sender
			iSendResult = csock.send(recvbuf, iResult);			
			printf("Bytes sent: %d\n", iSendResult);
		}
		else if (iResult == 0)
			printf("Connection closing...\n");		

	} while (iResult > 0);	
	return 0;
}

int main(int argc, char** argv)
{
	if (argc < 2) return 1;
	try {
		if (strcmp(argv[1], "client") == 0)
			make_client();
		else if (strcmp(argv[1], "server") == 0)
			make_server();
	}
	catch (Exception &ex)
	{
		printf("%s\n", ex.what());
	}
}