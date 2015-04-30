#include "User.h"

User::User(std::string name)
{
	mUsername = name;
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

#ifdef NC_SERVER

IPAddress User::GetIP() const
{
	return mAddress;
}

unsigned short User::GetPort() const
{
	return mPort;
}

Room* User::GetRoom() const
{
	return mRoom;
}

clock_t User::GetLastContactTime() const
{
	return mLastContactTime;
}

void User::SetIP(IPAddress add)
{
	mAddress = add;
}

void User::SetUserPort(unsigned short port)
{
	mPort = port;
}

void User::SetRoom(Room* room)
{
	mRoom = room;
}

void User::SetLastContactTime(clock_t time)
{
	mLastContactTime = time;
}

#endif