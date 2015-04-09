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
<<<<<<< HEAD
	PacketHander(SocketWrapper* sock);
	PacketHander();
=======
	PacketHandler(SocketWrapper sock);
>>>>>>> 69e27d014b8c42f51f6cbd06948a4a56dd659121
	// dtor
	~PacketHandler();

	void Update();
	void SendAck();

	void SetAddress(){};
	void GetAddress(){};

	uint GetNumPacketsSent();
	uint GetNumAcksReceived();

	void SetSocket(SocketWrapper* sock);


private:
	#pragma region PacketFunctions
	std::function<void(uint)> fSendPacket;
	#pragma endregion PacketFunctions

	SocketWrapper* mSocket;

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