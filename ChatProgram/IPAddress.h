#ifndef IP_ADDRESS_H
#define IP_ADDRESS_H

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
	unsigned long GetIPAddress();
	
private:
	unsigned long mIPAddress;
};


#endif//IP_ADDRESS_H
