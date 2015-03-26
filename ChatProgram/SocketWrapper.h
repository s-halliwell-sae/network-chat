#ifndef SOCKET_WRAPPER_H
#define SOCKET_WRAPPER_H

// This is temporary
#include <iostream>
#include <winsock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>
// \This is temporary


#include "PacketHandler.h"

class SocketWrapper
{
public:
	// ctors
	SocketWrapper();
	SocketWrapper(char* IPAddress, u_short port);
	// dtor
	~SocketWrapper();

	void Init();
	void Update();
	void Close();

	void Recieve();
	void Send(struct sockaddr_in address, char* packet);

	void PushData();
	bool CheckForWaitingData();
	void PopWaitingData();

	void SendToHandler();

private:
	SOCKET mSocket;
	struct sockaddr_in mAddress;
	struct sockaddr_in mSourceAddress;
	int mRecvLength;
	WSADATA mWSA;
	ULONG mIPAddress;

	u_short mPort;


	bool Bind();
};


#endif SOCKET_WRAPPER_H