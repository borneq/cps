#pragma once
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

class Socket
{
public:
	SOCKET connectSocket = INVALID_SOCKET;
	Socket();
	~Socket();
};
