#include "PacketHandler.h"
#include "SocketWrapper.h"


PacketHander::PacketHander(SocketWrapper* sock)
{
	mSocket = sock;
}
PacketHander::~PacketHander()
{

}

void PacketHander::Update()
{

}
void PacketHander::SendAck()
{

}

uint PacketHander::GetNumPacketsSent()
{
	return mNumPacketsSent;
}
uint PacketHander::GetNumAcksReceived()
{
	return mNumAcksReceived;
}

void PacketHander::SetSocket(SocketWrapper* sock)
{
	mSocket = sock;
}

/*
PacketHandler has Room[] Rooms
Room has a User[]

PacketHandler rec message
	Message says from
		Room X, User Y, Type Z
		PacketHandler Checks Rooms
			If Room X exists
			PacketHandler Checks Users in Room X
				If User Y exists
				PacketHandler Checks if packet Type Z can be handled
					PacketHandler Handles Packet of type Z
*/