#include "Room.h"
#include <ctime>

Room::Room(std::string roomName)
{
	mRoomName = roomName;
}
Room::Room(std::string roomName/*, bCrypt password*/)
{
	mRoomName = roomName;
	//mPassword = password;
}
Room::Room(std::string roomName, bool indestructible)
{
	mRoomName = roomName;
	mIndestructible = indestructible;
}
Room::Room(std::string roomName, bool indestructible/*, bCrypt password*/)
{
	mRoomName = roomName;
	mIndestructible = indestructible;
	//mPassword = password;
}

Room::~Room(){}

void Room::SetName(std::string name)
{
	mRoomName = name;
}

std::string Room::GetName()
{
	return mRoomName;
}

void Room::AddUser(User* user)
{
	mUsers.push_back(user);
}

std::vector<User*> Room::GetUsers()
{
	return mUsers;
}