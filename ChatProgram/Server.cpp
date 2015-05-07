#ifdef NC_SERVER

#include "IniManager.h"
#include "Server.h"
#include "Logger.h"
#include <Windows.h>

using std::cout;
using std::endl;

Server::Server(const string& name, unsigned short port) 
	: mRunning(false), 
	  mSocket(*new SocketWrapper())
{
	mName = name;
	mPort = port;
	
	LOG("Constructing server: " + name + ".");

	IniManager::getInstance().Init("Config/config.ini");
	mRenderer.SetupLayout(Renderer::CLIENT_CONNECTED);

	CreateRoom("Lobby", true);
	m_handler = new PacketHandler(&mSocket);
	mSocket.Bind();

	mCurrRoom = GetRoom("Lobby");

	UpdateRoomGUI();
}


void Server::detectServer(void)
{

	ABPacket ptm = *m_handler->mCurrentPacket;
	std::cout << "det" << std::endl;
	IPAddress senderIP(mSocket.GetClientData());

	//SendAcknowledge(senderIP, senderPort);
	SendServerInfo(senderIP, senderIP.GetPort());
}

void Server::message(void)
{
	std::cout << "msg" << std::endl;
	IPAddress senderIP(mSocket.GetClientData());
	unsigned short senderPort = mSocket.getSenderPort();

	PacketMessage ptm = (PacketMessage&)m_handler->mCurrentPacket;

	User* senderUser = GetUser(senderIP, senderPort);

	if (senderUser)
	{
		Room* senderRoom = senderUser->GetRoom();
		//if (senderRoom != GetRoom("Lobby"))
		{
			string logOut = senderUser->GetName() + " sent message: ";
			logOut.append(ptm.message);
			LOG(logOut);

			if (mCurrRoom == senderRoom)
			{
				mCurrRoomChat.push_back(senderUser->GetName() + ": " + ptm.message);
				UpdateChatGUI();
			}

			//SendAcknowledge(senderIP, senderPort);
			SendRoomMessage(*senderRoom, senderUser->GetName(), ptm.message);
		}
	}
}


void Server::changeRoom(void)
{
	std::cout << "CHANGE ROOM" << std::endl;
	IPAddress senderIP(mSocket.GetClientData());
	User* senderUser = GetUser(senderIP, senderIP.GetPort());

	if (senderUser)
	{
		PacketChangeRoomRequest ptcrr = (PacketChangeRoomRequest&)m_handler->mCurrentPacket;
		Room* toRoom = GetRoom(ptcrr.newRoomName);
		PacketChangeRoom resp = PacketChangeRoom();

		bool toRoomValid = toRoom != nullptr;

		if (toRoomValid)
			strncpy_s(resp.newRoomName, 32, toRoom->GetName().c_str(), 32);
		else
			strncpy_s(resp.newRoomName, 32, "-1", 32);

		//SendAcknowledge(senderIP, senderPort);
		SendChangeRoomResponse(resp, senderIP, senderIP.GetPort());

		if (toRoomValid)
		{
			string logOut = senderUser->GetName() + " moved to: ";
			logOut.append(toRoom->GetName());
			LOG(logOut);

			MoveUser(senderUser, toRoom);
			UpdateUserGUI();
		}
	}
}

void Server::changeName(void)
{
	std::cout << "change name" << std::endl;
	IPAddress senderIP(mSocket.GetClientData());
	PacketChangeUserNameRequest ptcunr = (PacketChangeUserNameRequest&)m_handler->mCurrentPacket;

	User* user = GetUser(ptcunr.newUserName);
	PacketChangeUserName packet_change_name;

	if (user)
	{
		memcpy(packet_change_name.newUserName, user->GetName().c_str(), 32);
	}
	else
	{
		string logOut = "Name changed: ";
		logOut.append(GetUser(senderIP, senderIP.GetPort())->GetName());
		logOut += " -> ";
		logOut.append(ptcunr.newUserName);
		LOG(logOut);

		GetUser(senderIP, senderIP.GetPort())->SetName(ptcunr.newUserName);
		memcpy(packet_change_name.newUserName, ptcunr.newUserName, 32);
	}

	//SendAcknowledge(senderIP, senderPort);
	SendChangeUserNameResponse(packet_change_name, senderIP, senderIP.GetPort());

	if (!user)
		SendUserList(*GetUser(senderIP, senderIP.GetPort())->GetRoom());

}


void Server::createRoom(void)
{

	std::cout << "create room" << std::endl;
	IPAddress senderIP(mSocket.GetClientData());
	PacketCreateRoomRequest ptcrr = (PacketCreateRoomRequest&)m_handler->mCurrentPacket;
	PacketCreateRoomResponse resp;
	resp.wasCreated = GetRoom(ptcrr.newRoomName) == nullptr;

	//SendAcknowledge(senderIP, senderPort);
	SendCreateRoomResponse(resp, senderIP, senderIP.GetPort());

	if (resp.wasCreated)
	{
		CreateRoom(ptcrr.newRoomName, false);
		SendRoomList();
	}
}

void Server::connect(void)
{
	std::cout << "CONNECT" << std::endl;
	IPAddress senderIP(mSocket.GetClientData());
	std::cout << senderIP << '\n';
	ConnectToServerRequest ptcsr = (ConnectToServerRequest&)m_handler->mCurrentPacket;
	ConnectToServerResponce resp;

	resp.isAccepted = GetUser(ptcsr.Username) == nullptr;

	//SendAcknowledge(senderIP, senderPort);
	SendConnectServerResponse(resp, senderIP, senderIP.GetPort());

	if (resp.isAccepted)
	{
		CreateUser(ptcsr.Username, senderIP, senderIP.GetPort());
		SendUserList(*GetRoom("Lobby"));
		SendRoomList();
		UpdateUserGUI();
	}
}

int Server::run()
{
	mRunning = true;
	m_handler->AssignAsServer();
	m_handler->AddFunctionToMap(std::bind(&Server::detectServer, this), PT_DETECT_SERVER);
	m_handler->AddFunctionToMap(std::bind(&Server::changeRoom, this), PT_CHANGE_ROOM_REQUEST);
	m_handler->AddFunctionToMap(std::bind(&Server::message, this), PT_MESSAGE);
	m_handler->AddFunctionToMap(std::bind(&Server::changeName, this), PT_CHANGE_USER_NAME_REQUEST);
	m_handler->AddFunctionToMap(std::bind(&Server::createRoom, this), PT_CREATE_ROOM_REQUEST);
	m_handler->AddFunctionToMap(std::bind(&Server::connect, this), PT_CONNECT_TO_SERVER_REQUEST);

	m_handler->SetPacketNumber(0);

	while (mRunning && !TCODConsole::isWindowClosed())
	{
		mSocket.Update();
		m_handler->Update();

		clock_t currTime = clock();

		for (size_t i = 0; i < mRooms.size(); ++i)
		{
			if (mRooms[i]->GetUsers().size() != 0)
				mRooms[i]->SetLastContactTime(clock());
			else if (!mRooms[i]->isIndestructible() && currTime - mRooms[i]->GetLastContactTime() > ROOM_TIMEOUT_MS)
				DeleteRoom(mRooms[i]);
		}

		mRenderer.Update();

		if (mRenderer.PressedEnter())
		{
			string message = mRenderer.RetrieveDynamicField();
			ProcessCommand(message);
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
	string logOut = "Room created: " + name;
	LOG(logOut);

	mRooms.push_back(new Room(name, indestructible));

	UpdateRoomGUI();
}

void Server::CreateUser(const string& name, const IPAddress& ip, unsigned short port)
{
	string logOut = "User created: " + name;
	LOG(logOut);

	User* newUser = new User(name);

	newUser->SetIP(ip);
	newUser->SetUserPort(port);
	newUser->SetRoom(GetRoom("Lobby"));
	
	//Need to add users to room
	GetRoom("Lobby")->AddUser(newUser);

	mUsers.push_back(newUser);
}

void Server::DeleteRoom(Room* room)
{
	for (size_t i = 0; i < mRooms.size(); i++)
	{
		if (mRooms[i] == room)
		{
			cout << "Deleted Room: " << mRooms[i]->GetName() << endl;
			mRooms.erase(mRooms.begin() + i);
		}
	}

	SendRoomList();
	UpdateRoomGUI();

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

	//Need to add users to room
	room->AddUser(user);

	SendUserList(*lastRoom);
	SendUserList(*room);
}

Room* const Server::GetRoom(const string& room) const
{
	for (size_t i = 0; i < mRooms.size(); ++i)
		if (mRooms[i]->GetName().compare(room) == 0)
			return mRooms[i];

	return nullptr;
}

User* const Server::GetUser(const string& name) const
{
	for (size_t i = 0; i < mUsers.size(); ++i)
		if (mUsers[i]->GetName().compare(name) == 0)
			return mUsers[i];

	return nullptr;
}

User* const Server::GetUser(const IPAddress& ip, unsigned short port) const
{
	for (size_t i = 0; i < mUsers.size(); ++i)
		if (mUsers[i]->GetIP().compare(ip) && mUsers[i]->GetPort() == port)
			return mUsers[i];

	return nullptr;
}

void Server::ChangeCurrentRoom(Room* room)
{
	mCurrRoomChat.clear();
	mCurrRoom = room;
	UpdateChatGUI();
	UpdateUserGUI();
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
		mSocket.Send(user->GetIP(), user->GetPort(), (ABPacket*)&packet_room_list, sizeof(PacketRoomList));
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
		mSocket.Send(user->GetIP(), user->GetPort(), (ABPacket*)&user_room_list, sizeof(PacketUserList));
	}
}

void Server::DefaultSend(/* const */ ABPacket& packet, unsigned int size, const IPAddress& ip, unsigned short port) const
{
	std::cout << "DEFAULTSEND\n";
	mSocket.Send(ip, port, &packet, size);
}

void Server::UpdateChatGUI()
{
	mRenderer.SetContents("Chat Log", mCurrRoomChat);
}

void Server::UpdateRoomGUI()
{
	vector<string> rooms;

	for (size_t i = 0; i < mRooms.size(); ++i)
		rooms.push_back(mRooms[i]->GetName());

	mRenderer.SetContents("Rooms", rooms);
}

void Server::UpdateUserGUI()
{
	vector<string> users;

	for (size_t i = 0; i < mUsers.size(); ++i)
		if (mUsers[i]->GetRoom() == mCurrRoom)
			users.push_back(mUsers[i]->GetName());

	mRenderer.SetContents("Users", users);
}

void Server::ProcessCommand(string cmd)
{
	if (cmd.substr(0, 1).compare("/") == 0)
	{
		int cmdEndPos = cmd.find(" ");

		if (cmdEndPos != string::npos)
		{
			string command = cmd.substr(1, cmdEndPos - 1);
			string text = cmd.substr(cmdEndPos + 1, cmd.length() - 1);

			if (command.compare("joinroom") == 0)
			{
				if (GetRoom(text))
				{
					ChangeCurrentRoom(GetRoom(text));
					UpdateChatGUI();
					UpdateUserGUI();
				}
			}
			else if (command.compare("createroom") == 0)
			{
				if (!GetRoom(text))
				{
					CreateRoom(text, false);
					UpdateRoomGUI();
					SendRoomList();
				}
			}
		}
		else
		{
			string command = cmd.substr(1, cmdEndPos - 1);

			if (command.compare("leaveroom") == 0)
			{
				ChangeCurrentRoom(GetRoom("Lobby"));
				UpdateChatGUI();
				UpdateUserGUI();
			}
		}
	}
	else
	{
		SendRoomMessage(*mCurrRoom, "SERVER", cmd);
		mCurrRoomChat.push_back("SERVER: " + cmd);
		UpdateChatGUI();
	}
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