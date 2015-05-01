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

	IPAddress GetIP() const;
	unsigned short GetPort() const;
	Room* GetRoom() const;

	void SetIP(IPAddress add);
	void SetUserPort(unsigned short port);
	void SetRoom(Room* room);

private:
	std::string mUsername;				//The name of the user

	IPAddress mAddress;
	Room* mRoom;
	unsigned short mPort;
};

#endif //USER_H