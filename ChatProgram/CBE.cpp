#include "CBE.h"
#include "User.h"
#include <string>
#include "Packets.h"

#include "include\libtcod.hpp"
#include "Logger.h"
//#include "Renderer.h"
#include "IniManager.h"

CBE::CBE()
{
//	mServer = new User("WAITINGFORSERVER");
	mParser = ChatParser(mChatCommand, mAppendCommand);

	FunctionPointer fp;
	fp = std::bind(&CBE::SendCreateRoom, this, std::placeholders::_1);
	mParser.GetCommandManager()->AddFunction("/createroom", fp);
	fp = std::bind(&CBE::SendSetName, this, std::placeholders::_1);
	mParser.GetCommandManager()->AddFunction("/setname", fp);
	fp = std::bind(&CBE::RequestRoomChange, this, std::placeholders::_1);
	mParser.GetCommandManager()->AddFunction("/joinroom", fp);

	fp = std::bind(&CBE::SendExitRoom, this, std::placeholders::_1);
	mParser.GetCommandManager()->AddFunction("/leaveroom", fp);
	fp = std::bind(&CBE::SendExitServer, this, std::placeholders::_1);
	mParser.GetCommandManager()->AddFunction("/leaveserver", fp);
	fp = std::bind(&CBE::BroadcastForServers, this, std::placeholders::_1);
	mParser.GetCommandManager()->AddFunction("/lookforservers", fp);

	//sendChatMessage
	fp = std::bind(&CBE::SendChatMessage, this, std::placeholders::_1);
	mParser.GetCommandManager()->AddFunction(mChatCommand, fp);

	//AppendColour
	fp = std::bind(&CBE::AppendColour, this, std::placeholders::_1);
	mParser.GetCommandManager()->AddFunction(mAppendCommand, fp);

	mPacketHandler = PacketHandler(&mSocket);

	SetServerAddr(IPAddress("127.0.0.1"));
};
CBE::~CBE()
{
	//delete mServer;
	//mServer = nullptr;
};

void CBE::Run()
{
	//Setup all the things
	IniManager::getInstance().Init("Config/config.ini");
	mRenderer.SetupLayout(Renderer::CLIENT_CONNECTED);

#pragma region TempStuffForTesting
	std::vector<std::string> fakeUsers;

	fakeUsers.push_back("Tommy");
	fakeUsers.push_back("Richard");
	fakeUsers.push_back("Harry");

	mRenderer.SetContents("Users", fakeUsers);

	std::vector<std::string> fakeRooms;

	fakeRooms.push_back("Living Room");
	fakeRooms.push_back("Bathroom");
	fakeRooms.push_back("Kitchen");

	mRenderer.SetContents("Rooms", fakeRooms);
#pragma endregion DeleteLater
	//Main loop
	while (!TCODConsole::isWindowClosed())
	{
		Update();
		//UpdateGUI
		if (mRenderer.PressedEnter())
		{
			SubmitTextBox(mRenderer.RetrieveDynamicField());
		}
		mRenderer.Update();
	}
}

void CBE::Update()
{
	//mSocket.Update();
	//mPacketHandler.Update();
	//if (IsServerDown())
	//{
	//	SendExitServer(std::vector<std::string>());
	//	//Set the state to looking for servers
	//	//Maybe display a server timed out message
	//	BroadcastForServers(std::vector<std::string>());
	//}
}

void CBE::SubmitTextBox(std::string text)
{
	mParser.Parse(text);
}

void CBE::SubmitTextBox()
{
	mParser.Parse(mChatBox);
}

void CBE::SendChatMessage(std::vector<std::string> &values)
{
	if (values.size() < 1)
	{
		values.push_back(" ");
	}
	PacketMessage* p = new PacketMessage();
	strncpy_s(p->message, values[0].c_str(), MESSAGE_SIZE);
	mSocket.Send(mServerAddr, (ABPacket*)p, sizeof(PacketMessage));
}

void CBE::AppendColour(std::vector<std::string> &values)
{
	if (values.size() < 1)
	{
		values.push_back(" ");
	}
	std::string colour = "";
	colour.append("[");
	colour.append(std::to_string(mcurBG));
	colour.append("][");
	colour.append(std::to_string(mCurFG));
	colour.append("]");
	mChatBox = colour.append(mChatBox);
}

void CBE::BroadcastForServers(std::vector<std::string> &values)
{
	PacketDetectServer* p = new PacketDetectServer();
	mSocket.Send(mServerAddr, (ABPacket*)p, sizeof(PacketDetectServer));
}

void CBE::SendExitRoom(std::vector<std::string> &values)
{
	PacketChangeRoomRequest* p = new PacketChangeRoomRequest();
	strncpy_s(p->newRoomName, "Lobby", ROOM_NAME_SIZE);
	mSocket.Send(mServerAddr, (ABPacket*)p, sizeof(PacketChangeRoomRequest));
}

void CBE::SendExitServer(std::vector<std::string> &values)
{
	//Sends a disconnect packet to the server informing it that the user is
	//leaving. After this the client is backed out to the searching for
	//servers state and a broadcast for servers packet is sent out
}

bool CBE::IsServerDown()
{
	if(time(0) - mPacketHandler.GetLastPacketTime() < mKillTime)
	{
		return true;
	}
	return false;
}

void CBE::RequestRoomChange(std::vector<std::string> &values)
{
	if (values.size() < 1)
	{
		LOG("Invalid parameters used when setting name.");
		return;
	}
	PacketChangeRoomRequest* p = new PacketChangeRoomRequest();
	strncpy_s(p->newRoomName, values[0].c_str(), ROOM_NAME_SIZE);
	mSocket.Send(mServerAddr, (ABPacket*)p, sizeof(PacketChangeRoomRequest));
}

void CBE::SendCreateRoom(std::vector<std::string> &values)
{
	if (values.size() < 1)
	{
		LOG("Invalid parameters used when trying to create room.");
	}
	PacketCreateRoomRequest* p = new PacketCreateRoomRequest();
	strncpy_s(p->newRoomName, values[0].c_str(), ROOM_NAME_SIZE);
	if (values.size() > 1)
	{
		strncpy_s(p->roomPassword, values[1].c_str(), PASSWORD_SIZE);
	}
	else
	{
		strncpy_s(p->roomPassword, std::string("").c_str(), PASSWORD_SIZE);
	}
	mSocket.Send(mServerAddr, (ABPacket*)p, sizeof(PacketCreateRoomRequest));
}

void CBE::SendSetName(std::vector<std::string> &values)
{
	if (values.size() < 1)
	{
		LOG("Invalid parameters used when setting name.");
		return;
	}
	PacketChangeUserNameRequest* p = new PacketChangeUserNameRequest();
	strncpy_s(p->newUserName, values[0].c_str(), USER_NAME_SIZE);
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
void CBE::RecieveMessage(std::string message)
{
	mRenderer.AddEntry("Chat Log", message);
}