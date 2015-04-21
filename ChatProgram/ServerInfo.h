#ifndef SERVER_INFO_H
#define SERVER_INFO_H

#include <string>

using u_short = unsigned short;
using ULONG = unsigned long;

struct ServerInfo
{
public:
	std::string mName;
	ULONG mIPAddress;
	u_short mPort;
};

#endif//SERVER_INFO_H