#ifndef USER_H
#define USER_H

#include <string>

#include "IPAddress.h"
#include "PacketHandler.h"
//#include "Room.h"

class User
{
public:
	User(std::string name);				//ctor
	~User();							//dtor

	std::string GetName();				//return mUsername
	void SetName(std::string name);		//set mUsername
	SocketWrapper* m_socket;
private:
	std::string mUsername;				//The name of the user
	//PacketHandler mPacketHandler;
	IPAddress m_address;
	//Room* m_room;
};

#endif //USER_H