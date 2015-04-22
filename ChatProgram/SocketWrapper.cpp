#include "SocketWrapper.h"

#include "IPAddress.h"

#define BUFFER_SIZE 1024 // note this is the maximum size of a packet before an error is thrown

// ctor
SocketWrapper::SocketWrapper()
{
	mPort = 1234;
	mIPAddress = INADDR_ANY;
	Init();
}

// ctor
SocketWrapper::SocketWrapper(IPAddress addr, unsigned short port)
{
	mPort = port;
	mIPAddress = addr.GetIPAddress();
	Init();
}
// dtor
SocketWrapper::~SocketWrapper()
{
	Close();
}

void SocketWrapper::Init()
{
	// Initialize Winsock
	if (WSAStartup(MAKEWORD(2, 2), &mWSA) != 0)
	{
		printf("WSAStartup failed with Error: %d", WSAGetLastError());
		return;
	}
	printf("Initialised Winsock.\n");

	mSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (mSocket == SOCKET_ERROR)
	{
		printf("Opening socket failed with Error: %d", WSAGetLastError());
		return;
	}
	printf("Socket Created.\n");

	//Fix a bug with windows
	DWORD dwBytesReturned = 0;
	BOOL bNewBehavior = FALSE;
	WSAIoctl(mSocket, SIO_UDP_CONNRESET, &bNewBehavior, sizeof(bNewBehavior), NULL, 0, &dwBytesReturned, NULL, NULL);

	memset((char *)&mSourceAddress, 0, sizeof(mSourceAddress));

	//Prepare the sockaddr_in structure
	mAddress.sin_family = AF_INET;
	mAddress.sin_addr.s_addr = mIPAddress;
	mAddress.sin_port = htons(mPort);

	//Bind
	if (Bind())
	{
		printf("Binding done.\n");
	}
	mRecvLength = sizeof(mSourceAddress);
}
void SocketWrapper::Update()
{
	if (CheckForWaitingData())
	{
		// Debugging
		printf("Something was recieved \n");
	}
}
void SocketWrapper::Close()
{
	closesocket(mSocket);
	WSACleanup();
}

void SocketWrapper::Recieve()
{
	char buffer[BUFFER_SIZE];
	memset(buffer, '\0', BUFFER_SIZE);
	if (recvfrom(mSocket, buffer, BUFFER_SIZE, 0, (struct sockaddr*) &mSourceAddress, &mRecvLength) == SOCKET_ERROR)
	{
		// If there's an error print it (we'll probably also want to log it)
		std::cout << "recvfrom() failed: Error " << WSAGetLastError() << std::endl;
	}
}
void SocketWrapper::Send(IPAddress addr, ABPacket &packet)
{
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = addr.GetIPAddress();
	address.sin_port = htons(mPort);
	// Send the packet
	/*
	if (sendto(mSocket, packet, sizeof(packet), 0, (struct sockaddr *) &address, mRecvLength) == SOCKET_ERROR)
	{
		// If there's an error print it (we'll probably also want to log it)
		printf("sendto() failed with error code : %d", WSAGetLastError());
	}*/
}

void SocketWrapper::Send(IPAddress addr, const char* packet)
{
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = addr.GetIPAddress();
	address.sin_port = htons(mPort);

	// Send the packet
	if (sendto(mSocket, packet, strlen(packet), 0, (struct sockaddr *) &address, mRecvLength) == SOCKET_ERROR)
	{
		// If there's an error print it (we'll probably also want to log it)
		printf("sendto() failed with error code : %d", WSAGetLastError());
	}
	printf("sent: %s %c", packet, '\n');
}

void SocketWrapper::PushData()
{
	// Push data to the PacketHandler
}

bool SocketWrapper::CheckForWaitingData()
{
	fd_set mCheckSockets;
	mCheckSockets.fd_count = 1;
	mCheckSockets.fd_array[0] = mSocket;
	struct timeval t;
	t.tv_sec = 0;
	t.tv_usec = 0;

	// If there is a packet, recvfrom it in the Recieve() function.
	if (select(NULL, &mCheckSockets, NULL, NULL, &t) > 0)
	{
		// Recieve data
		Recieve();
		// Return true to inform that data has been received
		return true;
	}
	// Return false to inform that data has not been received this update
	return false;
}
void SocketWrapper::PopWaitingData()
{

}

void SocketWrapper::SendToHandler()
{

}

bool SocketWrapper::Bind()
{
	if (bind(mSocket, (struct sockaddr *)&mAddress, sizeof(mAddress)) == SOCKET_ERROR)
	{
		printf("Bind failed with Error: %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	return true;
}