#ifndef PACKETS_H
#define PACKETS_H
#include <string.h>
#include "ABPacket.h"
// ABPacket holds the enum of PacketType

#define MESSAGE_SIZE 1024
#define USER_NAME_SIZE 32
#define ROOM_NAME_SIZE 32
#define PASSWORD_SIZE 32

#define MAX_NUM_ROOMS 256
#define MAX_USERS_PER_ROOM 256

#pragma pack (push, 1)

#pragma region Shared packets
// Shared Packets
struct PacketAcknowledge : ABPacket
{
public:
	PacketAcknowledge()
	{
		mPacketType = PT_ACKNOWLEDGE;
	}
};
struct PacketHeartBeat : ABPacket
{
public:
	PacketHeartBeat()
	{
		mPacketType = PT_HEART_BEAT;
	}
};
struct PacketMessage : ABPacket
{
public:
	PacketMessage()
	{
		mPacketType = PT_MESSAGE;
	}
	char userName[USER_NAME_SIZE];
	void SetUserName(const char* name)
	{
		strcpy_s(userName, name);
	}
	char message[MESSAGE_SIZE];
	void SetMessage(const char* msg)
	{
		strcpy_s(message, msg);
	}
};
struct PacketDetectServer : ABPacket
{
public:
	PacketDetectServer()
	{
		mPacketType = PT_DETECT_SERVER;
	}
};
#pragma endregion

#pragma region Server specific packets
// Server Packets
struct PacketChangeRoom : ABPacket
{
public:
	PacketChangeRoom()
	{
		mPacketType = PT_CHANGE_ROOM;
	}
	char newRoomName[ROOM_NAME_SIZE];
};
struct PacketChangeUserName : ABPacket
{
public:
	PacketChangeUserName()
	{
		mPacketType = PT_CHANGE_USER_NAME;
	}
	char newUserName[USER_NAME_SIZE];
};
struct PacketRoomList : ABPacket
{
public:
	PacketRoomList()
	{ 
		mPacketType = PT_ROOM_LIST;
	}
	char roomList[MAX_NUM_ROOMS*ROOM_NAME_SIZE];
};
struct PacketUserList : ABPacket
{
public:
	PacketUserList()
	{
		mPacketType = PT_USER_LIST;
	}
	char roomList[MAX_USERS_PER_ROOM*USER_NAME_SIZE];
};

struct PacketCreateRoomResponse : ABPacket
{
public:
	PacketCreateRoomResponse()
	{
		mPacketType = PT_CREATE_ROOM;
	}
	bool wasCreated;
};
struct ConnectToServerResponce : ABPacket
{
public:
	ConnectToServerResponce()
	{
		mPacketType = PT_CONNECT_TO_SERVER;
	}
	bool isAccepted;
};
#pragma endregion

#pragma region Client specific packets
// Client Packets
struct PacketChangeRoomRequest : ABPacket
{
public:
	PacketChangeRoomRequest()
	{
		mPacketType = PT_CHANGE_ROOM_REQUEST;
	}
	char newRoomName[ROOM_NAME_SIZE];
	char roomPassword[PASSWORD_SIZE];
};
struct PacketChangeUserNameRequest : ABPacket
{
public:
	PacketChangeUserNameRequest()
	{
		mPacketType = PT_CHANGE_USER_NAME_REQUEST;
	}
	char newUserName[USER_NAME_SIZE];
};

struct PacketCreateRoomRequest : ABPacket
{
public:
	PacketCreateRoomRequest()
	{
		mPacketType = PT_CREATE_ROOM_REQUEST;
	}
	char newRoomName[ROOM_NAME_SIZE];
	char roomPassword[PASSWORD_SIZE];
};

struct ConnectToServerRequest : ABPacket
{
public:
	ConnectToServerRequest()
	{
		mPacketType = PT_CONNECT_TO_SERVER_REQUEST;
	}
	char Username[USER_NAME_SIZE];
	// Bcrypt;
};
#pragma endregion

#pragma pack (pop)

#endif//PACKETS_H
