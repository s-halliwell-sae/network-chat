#include "Room.h"

Room::Room(std::string roomName)
{
	mRoomName = roomName;
}
//Room::Room(std::string roomName/*, bCrypt password*/)
//{
//	mRoomName = roomName;
//	//mPassword = password;
//}
Room::Room(std::string roomName, bool indestructible)
{
	mRoomName = roomName;
	mIndestructible = indestructible;
}
//Room::Room(std::string roomName, bool indestructible/*, bCrypt password*/)
//{
//	mRoomName = roomName;
//	mIndestructible = indestructible;
//	//mPassword = password;
//}

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

#ifdef NC_SERVER

void Room::RemoveUser(User* user)
{
	for (size_t i = 0; i < mUsers.size(); ++i)
		if (mUsers[i] == user)
			mUsers.erase(mUsers.begin() + i);
}

clock_t Room::GetLastContactTime() const
{
	return mLastContactTime;
}
void Room::SetLastContactTime(clock_t time)
{
	mLastContactTime = time;
}

#endif

std::vector<User*> Room::GetUsers()
{
	return mUsers;
}
