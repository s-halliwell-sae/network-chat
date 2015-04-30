#ifdef NC_SERVER

#include "Server.h"
#include "Logger.h"
#include <Windows.h>

using std::cout;
using std::endl;

Server::Server(const string& name, unsigned short port) 
	: mRunning(false), 
	  mSocket(*new SocketWrapper(IPAddress("127.0.0.1"), port))
{
	mName = name;
	mPort = port;
	
	LOG("Constructing server: " + name + ".");

	CreateRoom("Lobby", true);
	mSocket.Bind();


}

int Server::run()
{
	mRunning = true;

	while (mRunning)
	{
		if (mSocket.CheckForWaitingData())
		{
			ABPacket& packet = *mSocket.getLatestPacket();

			IPAddress senderIP(mSocket.getSenderIP());
			unsigned short senderPort = mSocket.getSenderPort();

			switch (packet.mPacketType)
			{
				case PT_MESSAGE:
				{
					PacketMessage ptm = (PacketMessage&) packet;
					User* senderUser = GetUser(senderIP, senderPort);

					if (senderUser)
					{
						SendAcknowledge(senderIP, senderPort);

						Room* senderRoom = senderUser->GetRoom();
						if (senderRoom != GetRoom("Lobby"))
							SendRoomMessage(*senderRoom, senderUser->GetName(), ptm.message);
					}

					break;
				}
				case PT_DETECT_SERVER:
				{
					SendServerInfo(senderIP, senderPort);

					break;
				}
				case PT_CHANGE_ROOM_REQUEST:
				{
					User* senderUser = GetUser(senderIP, senderPort);
					
					if (senderUser)
					{					
						PacketChangeRoomRequest ptcrr = (PacketChangeRoomRequest&) packet;
						Room* toRoom = GetRoom(ptcrr.newRoomName);

						if (toRoom)
						{
							Room* fromRoom = senderUser->GetRoom();

							PacketChangeRoom resp = PacketChangeRoom();
							strncpy_s(resp.newRoomName, 32, toRoom->GetName().c_str(), 32);
							SendChangeRoomResponse(resp, senderIP, senderPort);

							MoveUser(senderUser, toRoom);

							SendAcknowledge(senderIP, senderPort);
							SendChangeRoomResponse(resp, senderIP, senderPort);
						}
						else
						{
							PacketChangeRoom resp = PacketChangeRoom();
							strncpy_s(resp.newRoomName, 32, "-1", 32);

							SendAcknowledge(senderIP, senderPort);
							SendChangeRoomResponse(resp, senderIP, senderPort);
						}
					}

					break;
				}
				case PT_CHANGE_USER_NAME_REQUEST:
				{
					PacketChangeUserNameRequest ptcunr = (PacketChangeUserNameRequest&) packet;

					User* user = GetUser(ptcunr.newUserName);
					PacketChangeUserName packet_change_name;

					if (user)
					{
						memcpy(packet_change_name.newUserName, user->GetName().c_str(), 32);
					}
					else
					{
						user->SetName(ptcunr.newUserName);
						memcpy(packet_change_name.newUserName, ptcunr.newUserName, 32);
						
						SendChangeUserNameResponse(packet_change_name, senderIP, senderPort);
						SendUserList(*user->GetRoom());
					}

					break;
				}
				case PT_CREATE_ROOM_REQUEST:
				{
					PacketCreateRoomRequest ptcrr = (PacketCreateRoomRequest&) packet;
					PacketCreateRoomResponse resp;

					if (GetRoom(ptcrr.newRoomName))
					{
						resp.wasCreated = false;
						SendAcknowledge(senderIP, senderPort);
						SendCreateRoomResponse(resp, senderIP, senderPort);
					}
					else
					{
						CreateRoom(ptcrr.newRoomName, false);
						resp.wasCreated = true;

						SendAcknowledge(senderIP, senderPort);
						SendCreateRoomResponse(resp, senderIP, senderPort);
						SendRoomList();
					}

					break;
				}
				case PT_CONNECT_TO_SERVER_REQUEST:
				{
					ConnectToServerRequest ptcsr = (ConnectToServerRequest&) packet;
					ConnectToServerResponce resp;

					if (GetUser(ptcsr.Username))
					{
						resp.isAccepted = false;
					}
					else
					{
						resp.isAccepted = true;
						CreateUser(ptcsr.Username, senderIP, senderPort);
					}

					SendAcknowledge(senderIP, senderPort);
					SendConnectServerResponse(resp, senderIP, senderPort);

					break;
				}
				default:
					break;
			}
		}

		clock_t currTime = clock();

		for (size_t i = 0; i < mUsers.size(); ++i)
			if (currTime - mUsers[i]->GetLastContactTime() > USER_TIMEOUT_MS)
				DeleteUser(mUsers[i]);

		for (size_t i = 0; i < mRooms.size(); ++i)
		{
			if (mRooms[i]->GetUsers().size() != 0)
				mRooms[i]->SetLastContactTime(clock());
			else if (currTime - mRooms[i]->GetLastContactTime() > ROOM_TIMEOUT_MS)
				DeleteRoom(mRooms[i]);
		}

		Sleep(UPDATE_RATE);
	}

	return EXIT_SUCCESS;
}

void AddUserToRoom(Room& room, /* const */ User& user)
{
	room.AddUser(&user);
}

void RemoveUserFromRoom(Room& room, /* const */ User& user)
{
	room.RemoveUser(&user);
}

void SetUserRoom(User& user, /* const */ Room& room)
{
	user.SetRoom(&room);
}

void Server::CreateRoom(const string& name, bool indestructible)
{
	mRooms.push_back(new Room(name, indestructible));
}

void Server::CreateUser(const string& name, const IPAddress& ip, unsigned short port)
{
	User* newUser = new User(name);

	newUser->SetIP(ip);
	newUser->SetUserPort(port);
	newUser->SetRoom(GetRoom("Lobby"));
	newUser->SetLastContactTime(clock());

	mUsers.push_back(newUser);
}

void Server::DeleteRoom(Room* room)
{
	for (size_t i = 0; i < mUsers.size(); i++)
		if (mRooms[i] == room)
			mRooms.erase(mRooms.begin() + i);

	SendRoomList();

	delete room;
}

void Server::DeleteUser(User* user)
{
	Room* userRoom = user->GetRoom();

	for (size_t i = 0; i < mUsers.size(); i++)
		if (mUsers[i] == user)
			mUsers.erase(mUsers.begin() + i);

	userRoom->RemoveUser(user);
	SendUserList(*userRoom);

	delete user;
}

void Server::MoveUser(User* user, Room* room)
{
	Room* lastRoom = user->GetRoom();

	lastRoom->RemoveUser(user);
	user->SetRoom(room);
	user->GetRoom()->AddUser(user);

	SendUserList(*lastRoom);
	SendUserList(*room);
}

Room* Server::GetRoom(const string& room) const
{
	for (size_t i = 0; i < mRooms.size(); ++i)
		if (mRooms[i]->GetName().compare(room) == 0)
			return mRooms[i];

	return nullptr;
}

User* Server::GetUser(const string& name) const
{
	for (size_t i = 0; i < mUsers.size(); ++i)
		if (mUsers[i]->GetName().compare(name) == 0)
			return mUsers[i];

	return nullptr;
}

User* Server::GetUser(const IPAddress& ip, unsigned short port) const
{
	for (size_t i = 0; i < mUsers.size(); ++i)
		if (mUsers[i]->GetIP().compare(ip) && mUsers[i]->GetPort() == port)
			return mUsers[i];

	return nullptr;
}

void Server::SendAcknowledge(const IPAddress& ip, unsigned short port) const
{
	DefaultSend(PacketAcknowledge(), sizeof(PacketAcknowledge), ip, port);
}

void Server::SendChangeRoomResponse(/* const */ PacketChangeRoom& resp, const IPAddress& ip, unsigned short port) const
{
	DefaultSend(resp, sizeof(PacketChangeRoom), ip, port);
}

void Server::SendChangeUserNameResponse(/* const */ PacketChangeUserName& resp, const IPAddress& ip, unsigned short port) const
{
	DefaultSend(resp, sizeof(PacketChangeUserName), ip, port);
}

void Server::SendConnectServerResponse(/* const */ ConnectToServerResponce& resp, const IPAddress& ip, unsigned short port) const
{
	DefaultSend(resp, sizeof(ConnectToServerResponce), ip, port);
}

void Server::SendCreateRoomResponse(/* const */ PacketCreateRoomResponse& resp, const IPAddress& ip, unsigned short port) const
{
	DefaultSend(resp, sizeof(PacketCreateRoomResponse), ip, port);
}

void Server::SendServerInfo(const IPAddress& ip, unsigned short port)  const
{
	PacketServerInfo packet_server_info;
	packet_server_info.info.mPort = mPort;
	packet_server_info.info.mName = mName.c_str();

	DefaultSend(packet_server_info, sizeof(PacketServerInfo), ip, port);
}

void Server::SendRoomMessage(/*const*/ Room& room, const string& user, const string& msg) const
{
	PacketMessage packet_message;

	strncpy_s(packet_message.userName, 32, user.c_str(), 32);
	strncpy_s(packet_message.message, 1024, msg.c_str(), 1024);

	for (size_t i = 0; i < room.GetUsers().size(); ++i)
	{
		User* user = room.GetUsers()[i];
		DefaultSend(packet_message, sizeof(PacketMessage), user->GetIP(), user->GetPort());
	}
}

void Server::SendRoomList() const
{
	string room_list_string;

	for (size_t i = 0; i < mRooms.size(); ++i)
		room_list_string += mRooms[i]->GetName();

	PacketRoomList packet_room_list;

	strncpy_s(packet_room_list.roomList, 8192, room_list_string.c_str(), 8192);

	Room* lobby = GetRoom("Lobby");

	for (size_t i = 0; i < lobby->GetUsers().size(); ++i)
	{
		User* user = lobby->GetUsers()[i];
		mSocket.Send(user->GetIP(), user->GetPort(), (ABPacket*) &packet_room_list, sizeof(PacketRoomList));
	}
}

void Server::SendUserList(/* const */ Room& room) const
{
	string user_list_string;

	for (size_t i = 0; i < room.GetUsers().size(); ++i)
		user_list_string += room.GetUsers()[i]->GetName();

	PacketUserList user_room_list;

	strncpy_s(user_room_list.roomList, 8192, user_list_string.c_str(), 8192);
	
	for (size_t i = 0; i < room.GetUsers().size(); ++i)
	{
		User* user = room.GetUsers()[i];
		mSocket.Send(user->GetIP(), user->GetPort(), (ABPacket*) &user_room_list, sizeof(PacketUserList));
	}
}

void Server::DefaultSend(/* const */ ABPacket& packet, unsigned int size, const IPAddress& ip, unsigned short port) const
{
	mSocket.Send(ip, port, &packet, size);
}

Server::~Server()
{
	LOG("Destructing server: " + mName + ".");

	for (size_t i = 0; i < mUsers.size(); ++i)
		delete mUsers[i];

	mUsers.clear();

	for (size_t i = 0; i < mRooms.size(); ++i)
		delete mRooms[i];

	mRooms.clear();

	delete &mSocket;
}

#endif