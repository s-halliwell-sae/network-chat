#ifndef PENDING_H
#define PENDING_H

#include <time.h>
#include "ABPacket.h"
#include "Packets.h"
#include "IPAddress.h"

using uint = unsigned int;

class Pending
{
public:
	// ctor
	Pending();
	Pending(ABPacket* packet);
	// dtor
	~Pending();

	void TrySend();

	#pragma region Setters
	void SetTimeStamp(time_t time);
	void SetPacketNumber(uint num);
	void SetPacket(ABPacket* packet);
	void SetAddress(IPAddress addr);
	void SetSize(size_t size);
#pragma endregion Setters

	#pragma region Getters
	time_t GetTimeStamp();
	uint GetPacketNumber();
	ABPacket* GetPacket();
	uint GetAttempts();
	IPAddress GetAddress();
	size_t GetSize();
	#pragma endregion Getters
	
private:
	// Address to send to
	IPAddress mAddress;
	// Timestamp of packet first send
	time_t mTimeStamp;
	// Store packetnumber here for easier access
	uint mPacketNumber;
	// Number of attempts to resend so far
	uint mAttempts;
	// The whole packet
	ABPacket mPacket;
	// Size of the packet type
	size_t mSize;
};

#endif PENDING_H