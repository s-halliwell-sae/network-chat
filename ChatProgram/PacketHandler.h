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
private:
	#pragma region PacketFunctions
	std::function<void(uint)> fSendPacket;

	#pragma endregion PacketFunctions

	SocketWrapper* mSocket;

	// Current packet number (loop around)
	short mPacketNumber = 0;
	// Queue of packets that are awaiting an acknowledge
	std::vector<Pending> mPendingAcks;

	// Used for calculating packet loss
	uint mNumPacketsSent = 0;
	uint mNumAcksReceived = 0;

	// A map of all of the packet receive callback functions.
	std::map<std::vector<std::function<void(uint)>>, std::string> mPacketReceiveCallbacks;
};

#endif PACKET_HANDLER_H