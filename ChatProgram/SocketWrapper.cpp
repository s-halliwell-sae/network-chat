#include "SocketWrapper.h"

#include "IPAddress.h"

#define BUFFER_SIZE 9001 // note this is the maximum size of a packet before an error is thrown

// ctor
SocketWrapper::SocketWrapper()
{
	mIPAddress = INADDR_ANY;
	Init();
}

// ctor
SocketWrapper::SocketWrapper(IPAddress addr, unsigned short port)
{
	mPort = htons(port);
	mIPAddress = addr.GetIPAddress();
	Init();
}

SocketWrapper::SocketWrapper(IPAddress addr)
{
	mIPAddress = addr.GetIPAddress();
	mPort = addr.GetPort();
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

	BOOL bOptVal = TRUE;
	setsockopt(mSocket,
		SOL_SOCKET,
		SO_BROADCAST,
		(const char *)&bOptVal,
		sizeof(BOOL));

	memset((char *)&mSourceAddress, 0, sizeof(mSourceAddress));

	//Prepare the sockaddr_in structure
	mAddress.sin_family = AF_INET;
	mAddress.sin_addr.s_addr = mIPAddress;
	mAddress.sin_port = mPort;

	mRecvLength = sizeof(mSourceAddress);
}
void SocketWrapper::Update()
{
	if (CheckForWaitingData())
	{
		// Recieve data
		Recieve();
		// If there is a packet waiting, proceed to send that to the packethandler
		SendToHandler();
	}
}
void SocketWrapper::Close()
{
	closesocket(mSocket);
	// WSACleanup();
}

void SocketWrapper::Recieve()
{
	char* buffer = new char[BUFFER_SIZE];
	memset(buffer, '\0', BUFFER_SIZE);

	int recv = 0;


	if ((recv = recvfrom(mSocket, buffer, BUFFER_SIZE, 0, (struct sockaddr*) &mSourceAddress, &mRecvLength)) == SOCKET_ERROR)
	{
		// If there's an error print it (we'll probably also want to log it)
		std::cout << "recvfrom() failed: Error " << WSAGetLastError() << std::endl;
	}
	mLatestRecieved = (ABPacket*)buffer;
	std::cout << "PacketRec " << mLatestRecieved->mPacketNumber << "\n";
}

void SocketWrapper::Resend(IPAddress addr, ABPacket *packet, size_t size)
{
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = addr.GetIPAddress();
	address.sin_port = mPort;

	//std::cout << "pn: " << packet->mPacketNumber << " pn2: " << mHandler->GetPacketNumber() << std::endl;
	mLatestSent = packet;

	// Set the contents of the packet
	char* buffer = new char[size];
	memset(buffer, '\0', size);
	memcpy(buffer, packet, size);
	// Send the packet
	if (sendto(mSocket, buffer, size, 0, (struct sockaddr *) &address, mRecvLength) == SOCKET_ERROR)
	{
		// If there's an error print it (we'll probably also want to log it)
		printf("sendto() failed with error code : %d", WSAGetLastError());
	}
}
void SocketWrapper::Send(IPAddress addr, ABPacket *packet, size_t size)
{
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = addr.GetIPAddress();
	address.sin_port = mPort;

	if (packet->mPacketType != PT_ACKNOWLEDGE)
	{
		packet->mPacketNumber = mHandler->GetPacketNumber();
		packet->mPacketNumber++;

		mHandler->SetPacketNumber(packet->mPacketNumber);

		mHandler->PushPending(addr, packet, size);
	}

	//std::cout << "pn: " << packet->mPacketNumber << " pn2: " << mHandler->GetPacketNumber() << std::endl;
	mLatestSent = packet;

	// Set the contents of the packet
	char* buffer = new char[size];
	memset(buffer, '\0', size);
	memcpy(buffer, packet, size);
	// Send the packet
	if (sendto(mSocket, buffer, size, 0, (struct sockaddr *) &address, mRecvLength) == SOCKET_ERROR)
	{
		// If there's an error print it (we'll probably also want to log it)
		printf("sendto() failed with error code : %d", WSAGetLastError());
	}
}

#ifdef NC_SERVER
void SocketWrapper::Send(IPAddress addr, unsigned short port, ABPacket *packet, size_t size)
{
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = addr.GetIPAddress();
	address.sin_port = port;

	if (packet->mPacketType != PT_ACKNOWLEDGE)
	{
		packet->mPacketNumber = mHandler->GetPacketNumber();
		packet->mPacketNumber++;

		mHandler->SetPacketNumber(packet->mPacketNumber);

		mHandler->PushPending(addr, packet, size);
	}

	//std::cout << "pn: " << packet->mPacketNumber << " pn2: " << mHandler->GetPacketNumber() << std::endl;
	mLatestSent = packet;

	// Set the contents of the packet
	char* buffer = new char[size];
	memset(buffer, '\0', size);
	memcpy(buffer, packet, size);
	// Send the packet
	if (sendto(mSocket, buffer, size, 0, (struct sockaddr *) &address, mRecvLength) == SOCKET_ERROR)
	{
		// If there's an error print it (we'll probably also want to log it)
		printf("sendto() failed with error code : %d", WSAGetLastError());
	}
}
#endif

void SocketWrapper::Broadcast()
{
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = IPAddress("255.255.255.255", 40000).GetIPAddress();
	address.sin_port = IPAddress("255.255.255.255", 40000).GetPort();

	PacketDetectServer* pds = new PacketDetectServer();

	ABPacket* packet = (ABPacket*)pds;

	packet->mPacketNumber = 0;

	std::cout << "Send BC\n";
	mHandler->SetPacketNumber(packet->mPacketNumber);

	size_t size = sizeof(PacketDetectServer);

	if (sendto(mSocket, (const char*)packet, size, 0, (struct sockaddr *) &address, mRecvLength) == SOCKET_ERROR)
	{
		// If there's an error print it (we'll probably also want to log it)
		printf("sendto() failed with error code : %d", WSAGetLastError());
	}
}

void SocketWrapper::SetHandler(PacketHandler* handler)
{
	mHandler = handler;
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
	mHandler->PushPacket(mLatestRecieved);
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


ABPacket* SocketWrapper::getLatestPacket()
{
	return mLatestSent;
}

unsigned short SocketWrapper::getSenderPort()
{
	return (mSourceAddress.sin_port);
}

unsigned long SocketWrapper::getSenderIP()
{
	return mSourceAddress.sin_addr.S_un.S_addr;
}

unsigned long SocketWrapper::getSocketPort()
{
	struct sockaddr_in sin;
	int addrlen = sizeof(sin);
	if (getsockname(mSocket, (struct sockaddr *)&sin, &addrlen) == 0 &&
		sin.sin_family == AF_INET &&
		addrlen == sizeof(sin))
	{
		int local_port = ntohs(sin.sin_port);
		return local_port;
	}

	return ntohs(mSourceAddress.sin_port);
}