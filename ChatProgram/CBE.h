#ifndef CBE_H
#define CBE_H

#include <string>
#include <vector>
//#include "User.h"
#include "ServerInfo.h"
#include "ChatParser.h"

#include "PacketHandler.h"
#include "SocketWrapper.h"

#include "IPAddress.h"
#include "Renderer.h"

class CBE
{
public:
	CBE();
	~CBE();

	void JoinServer(std::vector<std::string> &values);
	void BroadcastForServers(std::vector<std::string> &values);	//Broadcast a looking for server packet
	void SendExitRoom(std::vector<std::string> &values);		//Exit the current room out to the lobby (if already in the lobby this will do nothing)
	void SendExitServer(std::vector<std::string> &values);		//Safe disconnect from the server
	void AppendColour(std::vector<std::string> &values);		//Add the previosly used colours onto the start of the message
	void SendCreateRoom(std::vector<std::string> &values);		//Send a create room request without a password
	void SendSetName(std::vector<std::string> &values);			//Send a name change request
	void RequestRoomChange(std::vector<std::string> &values);	//Send a room change request
	void SetRooms(std::vector<std::string> values);				//Updates the current list of rooms
	bool IsServerDown();										//Checks if the server has timed out
	void SetUsers(std::vector<std::string> users);				//Updates the list of users in the current room
	std::string mChatBox;										//The chatbox which will be directly modified through the gui
	std::vector<std::string> mChatLog;

	void Update();
	void SetServersFound(std::vector<ServerInfo> serversFound);
	void SetServerAddr(IPAddress addr);
	void SubmitTextBox();
	void SubmitTextBox(std::string text);

	void RecieveMessage(std::string user, std::string message);

	void SendChatMessage(std::vector<std::string> &values);
	void Run();
private:
	std::string mCurrentRoom;					//The current room this client is connected to
	int mCurFG;									//The most recent foreground colour used
	int mcurBG;									//The most recent background colour used
	std::vector<std::string> mRooms;			//A list of the names of all the available rooms
	std::vector<std::string> mUsers;			//A list of all the users connected to the current room
	std::vector<ServerInfo> mServersFound;		//A list of all the servers that respond to the broadcast

	ChatParser mParser;							//The Chat Parser

	SocketWrapper *mSocket;						//The active Socket
	PacketHandler *mPacketHandler;				//The Packet Handler
	IPAddress mServerAddr;						//The IPAddress of the server

	float mKillTime = 5.0f;						//Amount of time that can pass before server times out
	std::string mChatCommand = " sendmessage";	//The key to the send message function
	std::string mAppendCommand = " appendColour";


	Renderer mRenderer;
};

#endif //CBE_H