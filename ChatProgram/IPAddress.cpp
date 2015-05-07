#include "IPAddress.h"
#include <WS2tcpip.h>


// ctors
IPAddress::IPAddress()
{
	inet_pton(AF_INET, "127.0.0.1", &mIPAddress);
}
IPAddress::IPAddress(unsigned long IPAddr, unsigned short port)
{
	mIPAddress = IPAddr;
	mPort = htons(port);
}
IPAddress::IPAddress(const char * IPAddr, unsigned short port)
{
	inet_pton(AF_INET, IPAddr, &mIPAddress);
	mPort = htons(port);
}
// dtor
IPAddress::~IPAddress(){};

// setters
void IPAddress::SetAddress(const char * IPAddr)
{
	mIPAddress = inet_pton(AF_INET, IPAddr, &mIPAddress);;
}
void IPAddress::SetAddress(unsigned long IPAddr)
{
	mIPAddress = IPAddr;
}

// getters
unsigned long IPAddress::GetIPAddress() const
{
	return mIPAddress;
}

bool IPAddress::compare(const IPAddress& other)
{
	return mIPAddress == other.mIPAddress;
}