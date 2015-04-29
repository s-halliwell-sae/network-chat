#pragma once

#include <map>
#include <string>
#include <vector>

#include "Room.h"
#include "User.h"

using std::map;
using std::string;
using std::vector;

class Server
{
	public:

	Server(const string& name, unsigned short port);
	~Server();

	void createRoom(const string& name, bool indestructible);
	void createUser(const string& name);
	Room* getRoom(const string& room) const;
	void echoToRoom(const Room& room, const string& message) const;
	void moveUser(User& user, Room& room);
	int run();
	User* getUser(const string& room) const;

	private:

	string		   m_name;
	PacketHandler* m_packetHandler;
	unsigned short m_port;
	bool m_running;
	SocketWrapper* m_socket;

	vector<Room*> m_rooms;
	vector<User*> m_users;

};