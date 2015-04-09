#include "SocketWrapper.h"

#include "IPAddress.h"

#define BUFFER_SIZE 1024

// ctor
SocketWrapper::SocketWrapper()
{
	mIPAddress = INADDR_ANY;
	Init();
}

// ctor
SocketWrapper::SocketWrapper(IPAddress addr, unsigned short port)
{
	mPort = port;
	mIPAddress = addr.GetIPAddress;
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

	mSocket = socket(AF_INET, SOCK_DGRAM, 0);
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
	CheckForWaitingData();
}
void SocketWrapper::Close()
{
	closesocket(mSocket);
	WSACleanup();
}

void SocketWrapper::Recieve()
{
	char buf[BUFFER_SIZE];

	if (recvfrom(mSocket, buf, BUFFER_SIZE, 0, (struct sockaddr*) &mSourceAddress, &mRecvLength) == SOCKET_ERROR)
	{
		std::cout << "recvfrom() failed: Error " << WSAGetLastError() << std::endl;
	}
}
void SocketWrapper::Send(struct sockaddr_in address, char* packet)
{

}

void SocketWrapper::PushData()
{

}
bool SocketWrapper::CheckForWaitingData()
{
	fd_set mCheckSockets;

	mCheckSockets.fd_count = 1;
	mCheckSockets.fd_array[0] = mSocket;

	struct timeval t;
	t.tv_sec = 0;
	t.tv_usec = 0;

	if (select(NULL, &mCheckSockets, NULL, NULL, &t) > 0)
	{
		Recieve();
	}
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