#include "PacketHandler.h"
#include "SocketWrapper.h"

#include "IPAddress.h"

PacketHandler::PacketHandler(SocketWrapper* sock)
{
	mSocket = sock;
	mSocket->SetHandler(this);

//	mPacketReceiveCallbacks.insert(PT_MESSAGE, )

}

PacketHandler::PacketHandler()
{
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


void PacketHandler::SetSocket(SocketWrapper* sock)
{
	mSocket = sock;
}

void PacketHandler::PushPacket(ABPacket* pack)
{
	mCurrentPacket = pack;

//	std::function<void()> f = mPacketReceiveCallbacks.find(mCurrentPacket->mPacketType);
//	f();
	/*
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
		break;
	}
	case PT_DETECT_SERVER:
	{
		std::cout << "Recieved Broadcast\n";
		ABPacket* ack = new PacketAcknowledge();
		mSocket->Send(IPAddress("127.0.0.1"), ack, sizeof(PacketAcknowledge));
		break;
	}

	// or map with ints as key and std::functions
	// or std::vector of std::functions using mPacketType as index
	*/
}
#pragma region Packet Functions
void PacketHandler::Acknowledge()
{
	std::cout << "Ack recieved" << std::endl;
}

void PacketHandler::Heartbeat()
{
	std::cout << "HB recieved" << std::endl;
}

// Bind this message function if client
void PacketHandler::CMessage()
{
	PacketMessage* pk = (PacketMessage*)mCurrentPacket;
	std::cout << pk->userName << ": " << pk->message << std::endl;
	ABPacket* ack = new PacketAcknowledge();
	mSocket->Send(IPAddress("127.0.0.1"), ack, sizeof(PacketAcknowledge));
}

// Bind this message function if server
void PacketHandler::SMessage()
{

}

void PacketHandler::DetectServer()
{
	std::cout << "Recieved Broadcast\n";
	ABPacket* ack = new PacketAcknowledge();
	mSocket->Send(IPAddress("127.0.0.1"), ack, sizeof(PacketAcknowledge));
	// Send server info packet
}


void PacketHandler::ChangeRoom()
{

}

void PacketHandler::ChangeRoomRequest()
{

}


void PacketHandler::ChangeUsername()
{

}

void PacketHandler::ChangeUsernameRequest()
{

}


void PacketHandler::UserList()
{

}

void PacketHandler::RoomList()
{

}


void PacketHandler::CreateRoom()
{

}

void PacketHandler::CreateRoomRequest()
{

}


void PacketHandler::ConnectToserver()
{

}

void PacketHandler::ConnectToserverRequest()
{

}


#pragma endregion

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