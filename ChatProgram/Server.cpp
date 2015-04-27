

#include <iostream>

#include "Logger.h"
#include "Server.h"
#include "SocketWrapper.h"
#include <Windows.h>

Server::Server(const string& name, unsigned short port)
{
	m_name = name;

	LOG(("Constructing server: " + name + ".").c_str());

	createRoom("Lobby", true);

	m_socket = new SocketWrapper();
	m_packetHandler = new PacketHandler(m_socket);
}

void Server::createRoom(const string& name, bool indestructible)
{
	LOG(("Created Room: " + name + " on server: " + m_name + ".").c_str());

	m_rooms.push_back(new Room(name, indestructible));
}

int Server::run()
{
	m_running = true;

	while (m_running)
	{
		// Process all packets
		while (m_socket->CheckForWaitingData())
		{
			//ABPacket recv = m_socket;
			//IPAddress userIP = ;// Get IP of sender
			
			//switch (recv->mPacketType)
			//{
			//	case PT_DETECT_SERVER:
				{

				}
			//}
		}

		Sleep(1);
	}

	return EXIT_SUCCESS;
}


void Server::createUser(const string& name)
{
	LOG(("Created User: " + name + " on server: " + m_name + ".").c_str());

	User* newUser = new User(name);

	m_users.push_back(newUser);
	getLobby().AddUser(newUser); // Push user to lobby
}

void Server::echoToRoom(const Room& room, const string& message) const
{
	// for (size_t i = 0; i < room.GetUsers().size(); ++i)
	//     room.GetUsers()[i].sendMessage(message);
}

Room& Server::getLobby() const
{
	for (size_t i = 0; i < m_rooms.size(); ++i)
		if (m_rooms[i]->GetName().compare("Lobby") == 0)
			return *m_rooms[i];
}

void Server::moveUser(User& user, Room& room)
{
	// --- What if two users have the same name?
	// user.getRoom().RemoveUser(user.GetName());
	// room.AddUser(user);
	// user.setRoom(room);
}

Server::~Server()
{
	LOG(("Destructing server: " + m_name + ".").c_str());

	for (size_t i = 0; i < m_users.size(); ++i)
		delete m_users[i];

	for (size_t i = 0; i < m_rooms.size(); ++i)
		delete m_rooms[i];

	delete m_packetHandler;
	delete m_socket;
}