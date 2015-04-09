#include "CBE.h"
#include "User.h"
#include <string>
CBE::CBE()
{
	mServer = new User("WAITINGFORSERVER");
};
CBE::~CBE()
{
	delete mServer;
	mServer = nullptr;
};

//AppendColour(&mChatBox);
void CBE::AppendColour(std::string *msg)
{
	std::string colour = "";
	colour.append("[");
	colour.append(std::to_string(mcurBG));
	colour.append("][");
	colour.append(std::to_string(mCurFG));
	colour.append("]");
//	std::string oldMsg = *msg;
	*msg = colour.append(*msg);
}

void CBE::BroadcastForServers()
{
	
}

void CBE::SendExit()
{
	//PacketHandler do the thing
}

void CBE::ExitServer()
{

}

bool CBE::IsServerDown()
{

}

void CBE::RequestRoomChange()
{

}

void CBE::SendCreateRoom()
{
	
}

void CBE::SendSetName()
{

}

void CBE::UpdateRooms(std::vector<std::string> rooms)
{
	mRooms = rooms;
}

void CBE::UpdateUsers(std::vector<std::string> users)
{
	mUsers = users;
}