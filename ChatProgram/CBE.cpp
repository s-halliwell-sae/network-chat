#include "CBE.h"
#include "User.h"
#include <string>

CBE::CBE()
{
	mServer = new User("WAITINGFORSERVER");
	
	auto fp = std::bind(&CBE::AppendColour, this);
	parser.mFunctionMap.insert(std::make_pair("AppendColour", fp));
};
CBE::~CBE()
{
	delete mServer;
	mServer = nullptr;
};

//AppendColour(&mChatBox);
void CBE::AppendColour(std::string &msg)
{
	std::string colour = "";
	colour.append("[");
	colour.append(std::to_string(mcurBG));
	colour.append("][");
	colour.append(std::to_string(mCurFG));
	colour.append("]");
	msg = colour.append(msg);
}

void CBE::BroadcastForServers()
{
	//PacketHandler?
}

void CBE::SendExit()
{
	//PacketHandler?
}

void CBE::ExitServer()
{
	//PacketHandler?
}

bool CBE::IsServerDown()
{
	//PacketHandler?
	return false;
}

void CBE::RequestRoomChange(std::string roomName)
{
	//PacketHandler?
}

void CBE::SendCreateRoom(std::string roomName)
{
	//PacketHandler?
}

void CBE::SendCreateRoom(std::string roomName, std::string password)
{
	//PacketHandler?
}

void CBE::SendSetName(std::string newName)
{
	//PacketHandler?
}

void CBE::UpdateRooms(std::vector<std::string> rooms)
{
	mRooms = rooms;
}

void CBE::UpdateUsers(std::vector<std::string> users)
{
	mUsers = users;
}