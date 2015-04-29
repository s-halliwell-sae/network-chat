#ifndef IP_ADDRESS_H
#define IP_ADDRESS_H

#include <iostream>
#include <string>

class IPAddress
{
public:
	// ctors
	IPAddress();
	IPAddress(unsigned long IPAddr);
	IPAddress(const char * IPAddr);
	// dtor
	~IPAddress();
	// setters
	void SetAddress(const char * IPAddr);
	void SetAddress(unsigned long IPAddr);

	// getters
	unsigned long GetIPAddress()  const;

	
	friend std::ostream& operator<< (std::ostream& o, const IPAddress& ip)
	{

		std::string bytes[4];

		bytes[0] = std::to_string(ip.GetIPAddress() & 0xFF);
		bytes[1] = std::to_string((ip.GetIPAddress() >> 8) & 0xFF);
		bytes[2] = std::to_string((ip.GetIPAddress() >> 16) & 0xFF);
		bytes[3] = std::to_string((ip.GetIPAddress() >> 24) & 0xFF);

		return o << bytes[0] << "." << bytes[1] << "." << bytes[2] << "." << bytes[3];
	}
private:
	unsigned long mIPAddress;
};

#endif//IP_ADDRESS_H
