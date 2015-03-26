#ifndef SERVER_INFO_H
#define SERVER_INFO_H

#include <string>

struct ServerInfo
{
public:
	std::string mName;
	ULONG mIPAddress;
	u_short mPort;
};

#endif//SERVER_INFO_H