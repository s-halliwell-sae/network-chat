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

private:
	#pragma region PacketFunctions
	void Acknowledge();
	void Heartbeat();
	void Message();
	void DetectServer();

	void ChangeRoom();
	void ChangeRoomRequest();

	void ChangeUsername();
	void ChangeUsernameRequest();
	
	void GetUserList();
	void GetRoomList();

	void CreateRoom();
	void CreateRoomRequest();

	void ConnectToserver();
	void ConnectToserverRequest();

	#pragma endregion PacketFunctions

	SocketWrapper* mSocket;

	ABPacket* mCurrentPacket;

	// Current packet number (loop around)
	short mPacketNumber = 0;
	// Queue of packets that are awaiting an acknowledge
	std::vector<Pending> mPendingAcks;

	// Used for calculating packet loss
	uint mNumPacketsSent = 0;
	uint mNumAcksReceived = 0;

	// A map of all of the packet receive callback functions.
	std::map< std::string, std::function<void(uint)>> mPacketReceiveCallbacks;

	// Last packet recieved
	time_t mLastPacketTime;
};

#endif PACKET_HANDLER_H