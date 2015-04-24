#include "PacketHandler.h"
#include "SocketWrapper.h"

#include "IPAddress.h"

PacketHandler::PacketHandler(SocketWrapper* sock)
{
	mSocket = sock;
	mSocket->SetHandler(this);
}

//PacketHandler::PacketHandler()
//{
//}

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

void PacketHandler::PushPacket(ABPacket* pack)
{
	mCurrentPacket = pack;

	switch (mCurrentPacket->mPacketType)
	{
	case PT_ACKNOWLEDGE:
	{
		std::cout << "Ack recieved" << std::endl;
		break;
	}
	case PT_HEART_BEAT:
		break;
	case PT_MESSAGE:
	{
		PacketMessage* pk = (PacketMessage*)mCurrentPacket;
		std::cout << pk->userName << ": " << pk->message << std::endl;
		ABPacket* ack = new PacketAcknowledge();
		mSocket->Send(IPAddress("127.0.0.1"), ack, sizeof(PacketAcknowledge));
		//mSocket->Send(IPAddress("127.0.0.1"), mCurrentPacket, sizeof(PacketMessage));
		break;
	}
	case PT_DETECT_SERVER:
	{
		std::cout << "Recieved Broadcast\n";
		ABPacket* ack = new PacketAcknowledge();
		mSocket->Send(IPAddress("127.0.0.1"), ack, sizeof(PacketAcknowledge));
		break;
	}
	case PT_CHANGE_ROOM:
		break;
	case PT_CHANGE_ROOM_REQUEST:
		break;

	case PT_CHANGE_USER_NAME:
		break;
	case PT_CHANGE_USER_NAME_REQUEST:
		break;

	case PT_USER_LIST:
		break;
	case PT_ROOM_LIST:
		break;

	case PT_CREATE_ROOM:
		break;
	case PT_CREATE_ROOM_REQUEST:
		break;

	case PT_CONNECT_TO_SERVER:
		break;
	case PT_CONNECT_TO_SERVER_REQUEST:
		break;

	default:
		break;
	}
	// or map with ints as key and std::functions
	// or std::vector of std::functions using mPacketType as index
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
/*
c++
c#
lua
javascript
php

*/