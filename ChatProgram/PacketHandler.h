#ifndef PACKET_HANDLER_H
#define PACKET_HANDLER_H

#include <functional>
#include <vector>
#include <map>
#include <string>

#include "Pending.h"

class SocketWrapper;
using uint = unsigned int;

class PacketHandler
{
public:
	// ctor
	PacketHandler(SocketWrapper* sock);
	PacketHandler();
	// dtor
	~PacketHandler();

	// Update function, to be called every update...
	void Update();
	// Send an acknowledge to the sender of processed packet
	void SendAck();

	// Get data about how many packets have been sent and received
	uint GetNumPacketsSent();
	uint GetNumAcksReceived();

	// Set the socket to be used
	void SetSocket(SocketWrapper* sock);

	void PushPacket(ABPacket* pack);

	time_t GetLastPacketTime();

	// Allows you to add the function to packethandler externally
	// Will remove from map then add if the packetType is already in use
	void AddFunctionToMap(std::function<void()> func, PacketType packetType);
	void AssignAsClient();
	void AssignAsServer();

	void PushPending(IPAddress addr, ABPacket* pack, size_t size);

	void SetPacketNumber(unsigned short packetNumber);
	unsigned short GetPacketNumber();

	std::vector<Pending*>* GetPendingAcks();
	std::function<void(std::string, std::string)> GetClientMessageCallback()
	{
		return mClientMessageCallback;
	}
	void SetClientMessageCallback(std::function<void(std::string, std::string)> c)
	{
		mClientMessageCallback = c;
	}
	std::function<void(std::vector<std::string>)> GetClientUpdateRoomListCallback()
	{
		return mClientUpdateRoomListCallback;
	}
	void SetClientUpdateRoomListCallback(std::function<void(std::vector<std::string>)> c)
	{
		mClientUpdateRoomListCallback = c;
	}
	ABPacket* mCurrentPacket;
private:
	#pragma region PacketFunctions
	void Acknowledge();
	void Heartbeat();
	
	void CMessage();
	void SMessage();
	
	void DetectServer();
	void ServerInfo();

	void ChangeRoom();
	void ChangeRoomRequest();

	void ChangeUsername();
	void ChangeUsernameRequest();
	
	void UserList();
	void RoomList();

	void CreateRoom();
	void CreateRoomRequest();

	void ConnectToserver();
	void ConnectToserverRequest();
	#pragma endregion

	SocketWrapper* mSocket;

	IPAddress mLatestAddress;

	// Current packet number (loop around)
	unsigned short mPacketNumber = 0;
	// Queue of packets that are awaiting an acknowledge
	std::vector<Pending*> mPendingAcks;
	// Number of times to attempt resending packets
	int mMaxResendAttempts = 10;

	// Used for calculating packet loss
	uint mNumPacketsSent = 0;
	uint mNumAcksReceived = 0;

	// A map of all of the packet receive callback functions.
	std::map<PacketType, std::function<void()>> mPacketReceiveCallbacks;

	std::function<void(std::string, std::string)> mClientMessageCallback;
	std::function<void(std::vector<std::string > users)> mClientUpdateRoomListCallback;

	// Last packet recieved
	time_t mLastPacketTime;


};

#endif PACKET_HANDLER_H