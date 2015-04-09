#include "PacketHandler.h"
#include "SocketWrapper.h"


<<<<<<< HEAD
PacketHander::PacketHander(SocketWrapper* sock)
=======
PacketHandler::PacketHandler(SocketWrapper sock) : mSocket(sock)
>>>>>>> 69e27d014b8c42f51f6cbd06948a4a56dd659121
{
	mSocket = sock;
}
<<<<<<< HEAD
PacketHander::~PacketHander()
=======

PacketHandler::~PacketHandler()
>>>>>>> 69e27d014b8c42f51f6cbd06948a4a56dd659121
{

}

void PacketHandler::Update()
{

}
void PacketHandler::SendAck()
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
