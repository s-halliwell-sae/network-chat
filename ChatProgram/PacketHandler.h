#ifndef PACKET_HANDLER_H
#define PACKET_HANDLER_H

#include <functional>
#include <vector>
#include <map>
#include <string>

#include "Pending.h"

class SocketWrapper;
using uint = unsigned int;

class PacketHander
{
public:
	// ctor
	PacketHander(SocketWrapper sock);
	// dtor
	~PacketHander();

	void Update();
	void SendAck();

//	void SendPacket(unsigned int sizeOfPacket);

	void SetAddress(){};
	void GetAddress(){};

	// SetPort is being defined elsewhere, we might not need port however.
//	void SetPort();
//	void GetPort();

	uint GetNumPacketsSent(){};
	uint GetNumAcksReceived(){};

private:
	#pragma region PacketFunctions
	std::function<void(uint)> fSendPacket;
	#pragma endregion PacketFunctions

	SocketWrapper& mSocket;

//	ULONG mSendAddress;
//	u_short mPort;	// Probably don't need to worry about the port here
					// Socket handles it.

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