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

		 Server(const string& name);
		~Server();

		void createRoom(const string& name, const bool indestructible);
		void createUser(const string& name);
		Room& getLobby() const;
		void echoToRoom(const Room& room, const string& message) const;
		void moveUser(User& user, Room& room);
		void update();

	private:

		string		  m_name;
		vector<Room*> m_rooms;
		vector<User*> m_users;
};