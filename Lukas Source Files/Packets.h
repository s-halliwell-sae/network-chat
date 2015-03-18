#ifndef PACKETS_H
#define PACKETS_H

#include "ABPacket.h"

// Shared Packets

struct PacketAcknowledge : ABPacket
{
public:
	PacketType mPacketType = PT_ACKNOWLEDGE;
};

struct PacketMessage : ABPacket
{
public:
	PacketType mPacketType = PT_MESSAGE;
	char message[1024];
};

// Server Packets
struct PacketChangeRoom : ABPacket
{
public:
	PacketType mPacketType = PT_CHANGE_ROOM;
};
struct PacketChangeUserName : ABPacket
{
public:
	PacketType mPacketType = PT_CHANGE_USER_NAME;
};
struct PacketRoomList : ABPacket
{
public:
	PacketType mPacketType = PT_ROOM_LIST;
};
struct PacketUserList : ABPacket
{
public:
	PacketType mPacketType = PT_USER_LIST;
};

// Client Packets
struct PacketChangeRoomRequest : ABPacket
{
public:
	PacketType mPacketType = PT_CHANGE_ROOM_REQUEST;
};
struct PacketChangeUserNameRequest : ABPacket
{
public:
	PacketType mPacketType = PT_CHANGE_USER_NAME_REQUEST;
};
#endif PACKETS_H
