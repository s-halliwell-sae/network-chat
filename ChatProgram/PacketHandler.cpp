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
	for (int i = 0; i < mPendingAcks.size(); ++i)
	{
		if (mPendingAcks[i]->GetTimeStamp() < time(0) - 1)
		{
			if (mPendingAcks[i]->GetAttempts() >= mMaxResendAttempts)
			{
				std::cout << "Packet was not acknowledged after " << mMaxResendAttempts << " attempts.\n";
				mPendingAcks.erase(mPendingAcks.begin() + i);
			}
			else
			{
				std::cout << "trying to send " << mPendingAcks[i]->GetPacket()->mPacketNumber << ", Attempt: " << mPendingAcks[i]->GetAttempts() << '\n';
				mSocket->Resend(mPendingAcks[i]->GetAddress(), mPendingAcks[i]->GetPacket(), mPendingAcks[i]->GetSize());
				mPendingAcks[i]->TrySend();
				return;
			}
		}
	}
}
void PacketHandler::SetPacketNumber(unsigned short packetNumber)
{
	mPacketNumber = packetNumber;
}
unsigned short PacketHandler::GetPacketNumber()
{
	return mPacketNumber;
}

void PacketHandler::PushPending(IPAddress addr, ABPacket* pack, size_t size)
{
	mPendingAcks.push_back(new Pending(pack));
	mPendingAcks.back()->SetAddress(addr);
	mPendingAcks.back()->SetSize(size);
	mPendingAcks.back()->SetPacketNumber(pack->mPacketNumber);
}

std::vector<Pending*>* PacketHandler::GetPendingAcks()
{
	return &mPendingAcks;
}

void PacketHandler::SendAck()
{
	PacketAcknowledge* ack = new PacketAcknowledge();
	ABPacket* pack = (ABPacket*)ack;
	std::cout << "Send Ack number: " << mCurrentPacket->mPacketNumber << '\n';
	ack->mPacketNumber = mCurrentPacket->mPacketNumber;
	mSocket->Send(mLatestAddress, pack, sizeof(PacketAcknowledge));
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
	mCurrentPacket = pack;
	
	// Find the relvant function based on current packets type
	std::function<void()> f = mPacketReceiveCallbacks.at(mCurrentPacket->mPacketType);
	// Call the function.
	f();
	if (pack->mPacketType != PT_ACKNOWLEDGE)
	{
//		std::cout << "Packet Number in PushPacket: " << pack->mPacketNumber << '\n';
		SendAck();
	}
}
// Bind functions to the map externally
void PacketHandler::AddFunctionToMap(std::function<void()> func, PacketType packetType)
{
	std::function<void()> f = std::bind(func);
	mPacketReceiveCallbacks.erase(packetType);

	mPacketReceiveCallbacks.insert(std::pair<PacketType, std::function<void()>>(packetType, f));
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
//	std::cout << "Rec ACK PK num: " << mCurrentPacket->mPacketNumber << '\n';
	PacketAcknowledge* pk = (PacketAcknowledge*)mCurrentPacket;
//	std::cout << "Ack recieved: " << pk->mPacketNumber << std::endl;

//	std::cout << "mPendingAcks.size(): " << mPendingAcks.size() << '\n';

	for (int i = 0; i < mPendingAcks.size(); ++i)
	{
		if (pk->mPacketNumber == mPendingAcks[i]->GetPacketNumber())
		{
			mPendingAcks.erase(mPendingAcks.begin() + i);
		}
	}
//	std::cout << '\n';
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
}

// Bind this message function if server
void PacketHandler::SMessage()
{
	PacketMessage* pk = (PacketMessage*)mCurrentPacket;

}

void PacketHandler::DetectServer()
{
	std::cout << "Rec BC PK num: " << mCurrentPacket->mPacketNumber << '\n';
}
void PacketHandler::ServerInfo()
{
	std::cout << "Recieved ServerInfo\n";
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
