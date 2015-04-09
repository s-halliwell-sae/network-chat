#ifndef CBE_H
#define CBE_H

#include <string>
#include <vector>
#include "User.h"
#include "ServerInfo.h"

class CBE
{
public:
	CBE(){};
	~CBE(){};
	
	void BroadcastForServers();							//Serverinfo[]?
	void SendExit();									//
	void ExitServer();									//
	void AppendColour(std::string *msg);				//
	void SendCreateRoom();								//
	void SendSetName();									//
	void RequestRoomChange();							//
	void UpdateRooms(std::vector<std::string> rooms);	//
	bool IsServerDown();								//
	void UpdateUsers(std::vector<std::string> users);	//
private:
	std::string mCurrentRoom;				//
	int mCurFG;								//
	int mcurBG;								//
	std::vector<std::string> mRooms;		//
	std::vector<std::string> mUsers;		//
	User* mServer;							//
	std::vector<ServerInfo> mServersFound;	//
	std::string mChatBox;					//
};

#endif //CBE_H