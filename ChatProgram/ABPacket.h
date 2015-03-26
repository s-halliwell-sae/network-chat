#ifndef AB_PACKET_H
#define AB_PACKET_H

enum PacketType
{
	PT_ACKNOWLEDGE,
	PT_MESSAGE,

	PT_CHANGE_ROOM,
	PT_CHANGE_ROOM_REQUEST,

	PT_CHANGE_USER_NAME,
	PT_CHANGE_USER_NAME_REQUEST,

	PT_USER_LIST,
	PT_ROOM_LIST


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
