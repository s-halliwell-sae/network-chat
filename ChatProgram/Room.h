#ifndef ROOM_H
#define ROOM_H

#include <ctime>
#include <vector>

class User;

class Room
{
public:
	//ctors
	Room(std::string roomName);
//	Room(std::string roomName/*, bCrypt password*/);
	Room(std::string roomName, bool indestructible);
//	Room(std::string roomName, bool indestructible/*, bCrypt password*/);

	//dtor
	~Room();

	void AddUser(User*);						//Add User to mUsers
	//void RemoveUser(std::string username);		//Remove User from mUsers

#ifdef NC_SERVER
	void RemoveUser(User* user);
	clock_t GetLastContactTime() const;
	void SetLastContactTime(clock_t time);
	bool isIndestructible();
#endif


	std::vector<User*> GetUsers();				//Return mUsers
	std::string GetName();						//Return mRoomName
	void SetName(std::string name);				//Set mRoomName
private:
	bool mIndestructible;						//If the room will be destroyed when all the users leave
	std::vector<User*> mUsers;					//List of all the users in the Room
	float mKillTime;							//The amount of time before the room destroys itself when empty
	std::string mRoomName;						//The name of the room
	//bCrypt mPassword;							//The password for the room

#ifdef NC_SERVER
	clock_t mLastContactTime;
#endif
};

#endif //ROOM_H