#ifndef AB_PACKET_H
#define AB_PACKET_H

#include <iostream>

enum PacketType
{
	PT_ACKNOWLEDGE,
	PT_HEART_BEAT,
	PT_MESSAGE,
	PT_DETECT_SERVER,
	PT_SERVER_INFO,

	PT_CHANGE_ROOM,
	PT_CHANGE_ROOM_REQUEST,

	PT_CHANGE_USER_NAME,
	PT_CHANGE_USER_NAME_REQUEST,

	PT_USER_LIST,
	PT_ROOM_LIST,

	PT_CREATE_ROOM,
	PT_CREATE_ROOM_REQUEST,

	PT_CONNECT_TO_SERVER,
	PT_CONNECT_TO_SERVER_REQUEST
};

struct ABPacket
{
public:
	unsigned int mPacketNumber;
	PacketType mPacketType;
private:

protected:

};

#endif AB_PACKET_H
