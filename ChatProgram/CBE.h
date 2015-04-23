#ifndef CBE_H
#define CBE_H

#include <string>
#include <vector>
#include "User.h"
#include "ServerInfo.h"
#include "ChatParser.h"

#include "PacketHandler.h"
#include "SocketWrapper.h"

#include "IPAddress.h"

class CBE
{
public:
	CBE();
	~CBE();
	
	void BroadcastForServers();									//std::vector<Serverinfo>?
	void SendExit();											//Send a message to the server telling it that you're disconnecting
	void ExitServer();											//^?
	void AppendColour(std::vector<std::string> &values);		//Add the previosly used colours onto the start of the message
	void SendCreateRoom(std::vector<std::string> &values);		//Send a create room request without a password
	void SendSetName(std::vector<std::string> &values);			//Send a name change request
	void RequestRoomChange(std::vector<std::string> &values);	//Send a room change request
	void SetRooms(std::vector<std::string> values);				//
	bool IsServerDown();										//
	void SetUsers(std::vector<std::string> users);				//
	std::string mChatBox;										//The chatbox which will be directly modified through the gui

	void Update();
	void SetServersFound(std::vector<ServerInfo> serversFound);
	void SetServerAddr(IPAddress addr);
private:
	std::string mCurrentRoom;				//The current room this client is connected to
	int mCurFG;								//The most recent foreground colour used
	int mcurBG;								//The most recent background colour used
	std::vector<std::string> mRooms;		//A list of the names of all the available rooms
	std::vector<std::string> mUsers;		//A list of all the users connected to the current room
	User* mServer;							//The server the client is connected to
	std::vector<ServerInfo> mServersFound;	//A list of all the servers that respond to the broadcast

	ChatParser parser;

	SocketWrapper mSocket;
	PacketHandler mPacketHandler;

	IPAddress mServerAddr;
};

#endif //CBE_H