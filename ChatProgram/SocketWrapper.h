#ifndef SOCKET_WRAPPER_H
#define SOCKET_WRAPPER_H

// This is temporary
#include <iostream>
#include <winsock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>
// \This is temporary

class IPAddress;

#include "PacketHandler.h"

class SocketWrapper
{
public:
	// ctors
	SocketWrapper();
	SocketWrapper(IPAddress addr, unsigned short port);
	SocketWrapper(IPAddress addr);
	// dtor
	~SocketWrapper();

	// Initialise the socket
	void Init();
	// Update function, to be called every update...
	void Update();
	// Close the socket and clean up winsock
	void Close();

	// Recieve data on the socket, decide what to do with that data
	void Recieve();
	// Send a packet to the address
	void Send(IPAddress addr, ABPacket *packet, size_t size);
	void Resend(IPAddress addr, ABPacket *packet, size_t size);
#ifdef NC_SERVER
	void Send(IPAddress addr, unsigned short port, ABPacket *packet, size_t size);
#endif
	// Broadcast to find servers
	void Broadcast();

	void SetHandler(PacketHandler* handler);

	bool CheckForWaitingData();
	void SendToHandler();
	void PopWaitingData();
	
	IPAddress GetClientData()
	{
		return mClientData;
	}

	ABPacket* getLatestPacket();
	unsigned short getSenderPort();
	unsigned long getSenderIP();

	bool Bind(); 
	unsigned long getSocketPort();

private:
	// The buffer to temporarily hold data before sending it to a handler
	ABPacket* mLatestSent;
	ABPacket* mLatestRecieved;
	PacketHandler* mHandler;

	IPAddress mClientData;

	// Data that is relevant to the WINSOCK socket setup process and dealing with
	// incoming and outgoing packets
	SOCKET mSocket;
	struct sockaddr_in mAddress;
	struct sockaddr_in mSourceAddress;
	int mRecvLength;
	WSADATA mWSA;
	ULONG mIPAddress;
	u_short mPort;
};


#endif SOCKET_WRAPPER_H