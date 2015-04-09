#ifndef IP_ADDRESS_H
#define IP_ADDRESS_H

//#include <winsock2.h>
//#include <MSWSock.h>
#include <WS2tcpip.h>

class IPAddress
{
public:
	// ctors
	IPAddress()
	{
		inet_pton(AF_INET, "127.0.0.1", &mIPAddress);
	}
	IPAddress(ULONG IPAddr)
	{
		mIPAddress = IPAddr;
	}
	IPAddress(const char * IPAddr)
	{
		inet_pton(AF_INET, IPAddr, &mIPAddress);
	}
	// dtor
	~IPAddress(){};

	// setters
	void SetAddress(const char * IPAddr)
	{
		mIPAddress = inet_pton(AF_INET, IPAddr, &mIPAddress);;
	}
	void SetAddress(ULONG IPAddr)
	{
		mIPAddress = IPAddr;
	}

	// getter
	ULONG GetIPAddress()
	{
		return mIPAddress;
	}
private:
	ULONG mIPAddress;
};


#endif//IP_ADDRESS_H
