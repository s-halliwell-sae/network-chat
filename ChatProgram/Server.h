#pragma once

#include <string>
#include <vector>

#include "Room.h"
#include "User.h"

using std::string;
using std::vector;

class Server
{
	public:

	Server(const string& name, unsigned short port);
	~Server();

	void createRoom(const string& name, bool indestructible);
	void createUser(const string& name);
	Room& getLobby() const;
	void echoToRoom(const Room& room, const string& message) const;
	void moveUser(User& user, Room& room);
	int run();

	private:

	string		   m_name;
	PacketHandler* m_packetHandler;
	unsigned short m_port;
	vector<Room*>  m_rooms;
	bool m_running;
	SocketWrapper* m_socket;
	vector<User*>  m_users;

};