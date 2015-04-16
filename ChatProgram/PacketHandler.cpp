#include "PacketHandler.h"
#include "SocketWrapper.h"


PacketHandler::PacketHandler(SocketWrapper* sock)
{
	mSocket = sock;
}

PacketHandler::~PacketHandler()
{

}

void PacketHandler::Update()
{

}
void PacketHandler::SendAck()
{

}

uint PacketHandler::GetNumPacketsSent()
{
	return mNumPacketsSent;
}
uint PacketHandler::GetNumAcksReceived()
{
	return mNumAcksReceived;
}

void PacketHandler::SetSocket(SocketWrapper* sock)
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
