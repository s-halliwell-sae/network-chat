#ifndef USER_H
#define USER_H

#include <ctime>
#include <string>

#include "IPAddress.h"

class Room;

class User
{
public:
	User(std::string name);				//ctor
	~User();							//dtor

	std::string GetName();				//return mUsername
	void SetName(std::string name);		//set mUsername

#ifdef NC_SERVER
	IPAddress GetIP() const;
	unsigned short GetPort() const;
	Room* GetRoom() const;
	clock_t GetLastContactTime() const;

	void SetIP(IPAddress add);
	void SetUserPort(unsigned short port);
	void SetRoom(Room* room);
	void SetLastContactTime(clock_t time);
#endif

private:
	std::string mUsername;				//The name of the user

#ifdef NC_SERVER
	IPAddress mAddress;
	Room* mRoom;
	unsigned short mPort;
	clock_t mLastContactTime;
#endif
};

#endif //USER_H