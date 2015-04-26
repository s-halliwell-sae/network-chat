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

	// Set the socket to be used
	void SetSocket(SocketWrapper* sock);

	void PushPacket(ABPacket* pack);


	void AssignAsClient();
	void AssignAsServer();
	
private:
	#pragma region Packet Functions
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

	ABPacket* mCurrentPacket;

	#pragma region MOVE TO USER AND UNCOMMENT
	// Current packet number (loop around)
	short mPacketNumber = 0;
	// Queue of packets that are awaiting an acknowledge
	std::vector<Pending> mPendingAcks;

	// Used for calculating packet loss
	uint mNumPacketsSent = 0;
	uint mNumAcksReceived = 0;
	#pragma endregion

	// A map of all of the packet receive callback functions.
	std::map<PacketType, std::function<void()>> mPacketReceiveCallbacks;
};

#endif PACKET_HANDLER_H