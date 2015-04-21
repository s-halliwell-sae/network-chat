#ifndef CBE_H
#define CBE_H

#include <string>
#include <vector>
#include "User.h"
#include "ServerInfo.h"
#include "ChatParser.h"

class CBE
{
public:
	CBE();
	~CBE();
	
	void BroadcastForServers();											//std::vector<Serverinfo>?
	void SendExit();													//Send a message to the server telling it that you're disconnecting
	void ExitServer();													//^?
	void AppendColour(std::string &msg);								//Add the previosly used colours onto the start of the message
	void SendCreateRoom(std::string roomName);							//Send a create room request without a password
	void SendCreateRoom(std::string roomName, std::string password);	//Send a create room request with a password
	void SendSetName(std::string newName);								//Send a name change request
	void RequestRoomChange(std::string roomName);						//Send a room change request
	void UpdateRooms(std::vector<std::string> rooms);					//
	bool IsServerDown();												//
	void UpdateUsers(std::vector<std::string> users);					//
	std::string mChatBox;												//The chatbox which will be directly modified through the gui
private:
	std::string mCurrentRoom;				//The current room this client is connected to
	int mCurFG;								//The most recent foreground colour used
	int mcurBG;								//The most recent background colour used
	std::vector<std::string> mRooms;		//A list of the names of all the available rooms
	std::vector<std::string> mUsers;		//A list of all the users connected to the current room
	User* mServer;							//The server the client is connected to
	std::vector<ServerInfo> mServersFound;	//A list of all the servers that respond to the broadcast

	ChatParser parser;
};

#endif //CBE_H