#include "IPAddress.h"
#include <WS2tcpip.h>


// ctors
IPAddress::IPAddress()
{
	inet_pton(AF_INET, "127.0.0.1", &mIPAddress);
}
IPAddress::IPAddress(unsigned long IPAddr)
{
	mIPAddress = IPAddr;
}
IPAddress::IPAddress(const char * IPAddr)
{
	inet_pton(AF_INET, IPAddr, &mIPAddress);
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