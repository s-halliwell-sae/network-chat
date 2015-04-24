#include "CBE.h"
#include "User.h"
#include <string>
#include "Packets.h"

CBE::CBE()
{
	mServer = new User("WAITINGFORSERVER");
	
	FunctionPointer fp = std::bind(&CBE::AppendColour, this, std::placeholders::_1);
	parser.GetCommandManager()->AddFunction("AppendColour", fp);
	fp = std::bind(&CBE::SendCreateRoom, this, std::placeholders::_1);
	parser.GetCommandManager()->AddFunction("CreateRoom", fp);
	fp = std::bind(&CBE::AppendColour, this, std::placeholders::_1);
	parser.GetCommandManager()->AddFunction("AppendColour", fp);
	fp = std::bind(&CBE::AppendColour, this, std::placeholders::_1);
	parser.GetCommandManager()->AddFunction("AppendColour", fp);
	fp = std::bind(&CBE::AppendColour, this, std::placeholders::_1);
	parser.GetCommandManager()->AddFunction("AppendColour", fp);


//	mSocket = SocketWrapper;
	mPacketHandler = PacketHandler(&mSocket);

	SetServerAddr(IPAddress("127.0.0.1"));
};
CBE::~CBE()
{
	delete mServer;
	mServer = nullptr;
};

//FunctionPointer CBE::BindFunction(void(CBE::*)(std::string&) function)
//{
//	return std::bind(function, this, std::placeholders::_1);
//}

void CBE::Update()
{
	mSocket.Update();
	mPacketHandler.Update();
}

//AppendColour(&mChatBox);
void CBE::AppendColour(std::vector<std::string> &values)
{
	std::string colour = "";
	colour.append("[");
	colour.append(std::to_string(mcurBG));
	colour.append("][");
	colour.append(std::to_string(mCurFG));
	colour.append("]");
	values[0] = colour.append(values[0]);
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

void CBE::RequestRoomChange(std::vector<std::string> &values)
{
	PacketChangeRoomRequest* p = new PacketChangeRoomRequest();
	strncpy_s(p->newRoomName, values[0].c_str(), ROOM_NAME_SIZE);
	mSocket.Send(mServerAddr, (ABPacket*)p, sizeof(PacketChangeRoomRequest));
}

void CBE::SendCreateRoom(std::vector<std::string> &values)
{
	PacketCreateRoomRequest* p = new PacketCreateRoomRequest();
	strncpy_s(p->newRoomName, values[0].c_str(), ROOM_NAME_SIZE);
	mSocket.Send(mServerAddr, (ABPacket*)p, sizeof(PacketCreateRoomRequest));
}

void CBE::SendSetName(std::vector<std::string> &values)
{
	//PacketHandler?
	PacketChangeUserNameRequest* p = new PacketChangeUserNameRequest();
	strncpy_s(p->newUserName, values[0].c_str(), USER_NAME_SIZE);
	//p->newUserName(newName.c_str());
	mSocket.Send(mServerAddr, (ABPacket*)p, sizeof(PacketChangeUserNameRequest));
}

void CBE::SetRooms(std::vector<std::string> rooms)
{
	mRooms = rooms;
}

void CBE::SetUsers(std::vector<std::string> users)
{
	mUsers = users;
}

void CBE::SetServersFound(std::vector<ServerInfo> serversFound)
{
	mServersFound = serversFound;
}

void CBE::SetServerAddr(IPAddress addr)
{
	mServerAddr = addr;
}