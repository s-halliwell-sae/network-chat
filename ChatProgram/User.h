#ifndef USER_H
#define USER_H

#include <string>
#include "PacketHandler.h"
class User
{
public:
	User(std::string name);				//ctor
	~User();							//dtor

	std::string GetName();				//return mUsername
	void SetName(std::string name);		//set mUsername
private:
	std::string mUsername;				//The name of the user
	//PacketHander mPacketHandler;
};

#endif //USER_H