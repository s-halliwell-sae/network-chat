#ifndef PENDING_H
#define PENDING_H

#include <time.h>
#include "ABPacket.h"
#include "Packets.h"

using uint = unsigned int;

class Pending
{
public:
	// ctor
	Pending();
	// dtor
	~Pending();

	void TrySend();

	#pragma region Setters
	void SetTimeStamp(time_t time);
	void SetPacketNumber(uint num);
	//	void SetAttempts(uint num);
	void SetPacket(ABPacket packet);
	#pragma endregion Setters

	#pragma region Getters
	time_t GetTimeStamp();
	uint GetPacketNumber();
	ABPacket GetPacket();
	uint GetAttempts();
	#pragma endregion Getters
	
private:
	// Will likely need a timer
	// Would that be a static global?

	time_t mTimeStamp;
	uint mPacketNumber;
	uint mAttempts;
	ABPacket mPacket;
};

#endif PENDING_H