#include "PacketHandler.h"
#include "SocketWrapper.h"

#include "IPAddress.h"

PacketHandler::PacketHandler(SocketWrapper* sock)
{
	mSocket = sock;
	mSocket->SetHandler(this);
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

time_t PacketHandler::GetLastPacketTime()
{
	return mLastPacketTime;
}

void PacketHandler::PushPacket(ABPacket* pack)
{
	// Temporarily save the current packet
	mCurrentPacket = pack;		 	mCurrentPacket = pack;
	// Find the relvant function based on current packets type
	std::function<void()> f = mPacketReceiveCallbacks.at(mCurrentPacket->mPacketType);
	// Call the function.
	f();
}

// The function that assigns the client specific functions to the callback map
void PacketHandler::AssignAsClient()
{
	// Acknowledge
	std::function<void()> f = std::bind(&PacketHandler::Acknowledge, this);
	mPacketReceiveCallbacks.insert(std::pair<PacketType, std::function<void()>>(PT_ACKNOWLEDGE, f));

	// Heart Beat
	f = std::bind(&PacketHandler::Heartbeat, this);
	mPacketReceiveCallbacks.insert(std::pair<PacketType, std::function<void()>>(PT_HEART_BEAT, f));

	// Message
	f = std::bind(&PacketHandler::CMessage, this);
	mPacketReceiveCallbacks.insert(std::pair<PacketType, std::function<void()>>(PT_MESSAGE, f));

	// Detect Server
	f = std::bind(&PacketHandler::DetectServer, this);
	mPacketReceiveCallbacks.insert(std::pair<PacketType, std::function<void()>>(PT_DETECT_SERVER, f));

	// Server Info
	f = std::bind(&PacketHandler::ServerInfo, this);
	mPacketReceiveCallbacks.insert(std::pair<PacketType, std::function<void()>>(PT_SERVER_INFO, f));

	// Change Room
	f = std::bind(&PacketHandler::ChangeRoom, this);
	mPacketReceiveCallbacks.insert(std::pair<PacketType, std::function<void()>>(PT_CHANGE_ROOM, f));

	// Change Username
	f = std::bind(&PacketHandler::ChangeUsername, this);
	mPacketReceiveCallbacks.insert(std::pair<PacketType, std::function<void()>>(PT_CHANGE_USER_NAME, f));

	// User List
	f = std::bind(&PacketHandler::UserList, this);
	mPacketReceiveCallbacks.insert(std::pair<PacketType, std::function<void()>>(PT_USER_LIST, f));

	// Room List
	f = std::bind(&PacketHandler::RoomList, this);
	mPacketReceiveCallbacks.insert(std::pair<PacketType, std::function<void()>>(PT_ROOM_LIST, f));

	// Create Room
	f = std::bind(&PacketHandler::CreateRoom, this);
	mPacketReceiveCallbacks.insert(std::pair<PacketType, std::function<void()>>(PT_CREATE_ROOM, f));

	// Connect to Server
	f = std::bind(&PacketHandler::ConnectToserver, this);
	mPacketReceiveCallbacks.insert(std::pair<PacketType, std::function<void()>>(PT_CONNECT_TO_SERVER, f));
}
// The function that assigns the server specific functions to the callback map
void PacketHandler::AssignAsServer()
{
	// Acknowledge
	std::function<void()> f = std::bind(&PacketHandler::Acknowledge, this);
	mPacketReceiveCallbacks.insert(std::pair<PacketType, std::function<void()>>(PT_ACKNOWLEDGE, f));

	// Heart Beat
	f = std::bind(&PacketHandler::Heartbeat, this);
	mPacketReceiveCallbacks.insert(std::pair<PacketType, std::function<void()>>(PT_HEART_BEAT, f));

	// Message
	f = std::bind(&PacketHandler::SMessage, this);
	mPacketReceiveCallbacks.insert(std::pair<PacketType, std::function<void()>>(PT_MESSAGE, f));

	// Detect Server
	f = std::bind(&PacketHandler::DetectServer, this);
	mPacketReceiveCallbacks.insert(std::pair<PacketType, std::function<void()>>(PT_DETECT_SERVER, f));

	// Server Info
	f = std::bind(&PacketHandler::ServerInfo, this);
	mPacketReceiveCallbacks.insert(std::pair<PacketType, std::function<void()>>(PT_SERVER_INFO, f));

	// Change Room Request
	f = std::bind(&PacketHandler::ChangeRoomRequest, this);
	mPacketReceiveCallbacks.insert(std::pair<PacketType, std::function<void()>>(PT_CHANGE_ROOM_REQUEST, f));

	// Change Username Request
	f = std::bind(&PacketHandler::ChangeUsernameRequest, this);
	mPacketReceiveCallbacks.insert(std::pair<PacketType, std::function<void()>>(PT_CHANGE_USER_NAME_REQUEST, f));

	// User List Request
	f = std::bind(&PacketHandler::UserList, this);
	mPacketReceiveCallbacks.insert(std::pair<PacketType, std::function<void()>>(PT_USER_LIST, f));

	// Room List Request
	f = std::bind(&PacketHandler::RoomList, this);
	mPacketReceiveCallbacks.insert(std::pair<PacketType, std::function<void()>>(PT_ROOM_LIST, f));

	// Create Room Request
	f = std::bind(&PacketHandler::CreateRoomRequest, this);
	mPacketReceiveCallbacks.insert(std::pair<PacketType, std::function<void()>>(PT_CREATE_ROOM_REQUEST, f));

	// Connect to Server Request
	f = std::bind(&PacketHandler::ConnectToserverRequest, this);
	mPacketReceiveCallbacks.insert(std::pair<PacketType, std::function<void()>>(PT_CONNECT_TO_SERVER_REQUEST, f));
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
	// Change to relevant IP address
	mSocket->Send(IPAddress("127.0.0.1"), ack, sizeof(PacketAcknowledge));
}

// Bind this message function if server
void PacketHandler::SMessage()
{
	ABPacket* ack = new PacketAcknowledge();
	// Change to relevant IP address
	mSocket->Send(IPAddress("127.0.0.1"), ack, sizeof(PacketAcknowledge));
}

void PacketHandler::DetectServer()
{
	std::cout << "Recieved Broadcast\n";
	ABPacket* ack = new PacketAcknowledge();
	// Change to relevant IP address
	mSocket->Send(IPAddress("127.0.0.1"), ack, sizeof(PacketAcknowledge));
}
void PacketHandler::ServerInfo()
{
	std::cout << "Recieved ServerInfo\n";
	ABPacket* ack = new PacketAcknowledge();
	// Change to relevant IP address
	mSocket->Send(IPAddress("127.0.0.1"), ack, sizeof(PacketAcknowledge));
	// Send server info packet
}


void PacketHandler::ChangeRoom()
{
	// Move the client to another room
}

void PacketHandler::ChangeRoomRequest()
{
	// the user to another room
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
