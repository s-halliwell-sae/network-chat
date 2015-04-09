#include "User.h"
//Temp
//#include "SocketWrapper.h"
//
User::User(std::string name)
{
	mUsername = name;
//	SocketWrapper sWrapper = SocketWrapper();
//	mPacketHandler = PacketHander(sWrapper);
	
}
User::~User(){}

void User::SetName(std::string name)
{
	mUsername = name;
}

std::string User::GetName()
{
	return mUsername;
}