#include "Pending.h"
Pending::Pending()
{
}
Pending::~Pending()
{
}

void Pending::TrySend()
{
	mAttempts++;

}

// Setters and Getters
void Pending::SetTimeStamp(time_t time)
{
	mTimeStamp = time;
}
time_t Pending::GetTimeStamp()
{
	return mTimeStamp;
}

void Pending::SetPacketNumber(uint num)
{
	mPacketNumber = num;
}
uint Pending::GetPacketNumber()
{
	return mPacketNumber;
}

uint Pending::GetAttempts()
{
	return mAttempts;
}

void Pending::SetPacket(ABPacket* packet)
{
	mPacket = packet;
}
ABPacket* Pending::GetPacket()
{
	return mPacket;
}