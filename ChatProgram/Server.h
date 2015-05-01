#pragma once
#ifdef NC_SERVER

#include <map>
#include <string>
#include <vector>

#include "Room.h"
#include "SocketWrapper.h"
#include "Renderer.h"
#include "User.h"

using std::map;
using std::string;
using std::vector;

class Server
{
	public:

	Server(const string& name, unsigned short port);
	~Server();

	int run();

	private:

	const long ROOM_TIMEOUT_MS = 60000;
	const long UPDATE_RATE = 16;

	void CreateRoom(const string& name, bool indestructible);
	void CreateUser(const string& name, const IPAddress& ip, unsigned short port);

	void DeleteRoom(Room* room);
	void DeleteUser(User* user);

	void MoveUser(User* user, Room* room);

	Room* const GetRoom(const string& room) const;
	User* const GetUser(const string& name) const;
	User* const GetUser(const IPAddress& ip, unsigned short port) const;

	void ChangeCurrentRoom(Room* room);

	void SendAcknowledge(const IPAddress& ip, unsigned short port) const;
	void SendChangeRoomResponse(/* const */ PacketChangeRoom& resp, const IPAddress& ip, unsigned short port) const;
	void SendChangeUserNameResponse(/* const */ PacketChangeUserName& resp, const IPAddress& ip, unsigned short port) const;
	void SendConnectServerResponse(/* const */ ConnectToServerResponce& resp, const IPAddress& ip, unsigned short port) const;
	void SendCreateRoomResponse(/* const */ PacketCreateRoomResponse& resp, const IPAddress& ip, unsigned short port) const;
	void SendRoomList() const;
	void SendRoomMessage(/* const */ Room& room, const string& user, const string& msg) const;
	void SendServerInfo(const IPAddress& ip, unsigned short port) const;
	void SendUserList(/* const */ Room& room) const;

	void DefaultSend(/* const */ ABPacket& packet, unsigned int size, const IPAddress& ip, unsigned short port) const;

	void UpdateChatGUI();
	void UpdateRoomGUI();
	void UpdateUserGUI();

	void ProcessCommand(string cmd);
	
	bool	 mRunning;
	Renderer mRenderer;

	string		   mName;
	unsigned short mPort;
	SocketWrapper& mSocket;

	vector<Room*> mRooms;
	vector<User*> mUsers;

	Room*		   mCurrRoom;
	vector<string> mCurrRoomChat;
};

#endif