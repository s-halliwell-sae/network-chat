#include "Pending.h"
#include <time.h>
Pending::Pending()
{
}
Pending::Pending(ABPacket* packet)
{
	mPacket = *packet;
	SetTimeStamp(time(0));
	mAttempts = 0;
}
Pending::~Pending()
{
}

void Pending::TrySend()
{
	mAttempts++;
	SetTimeStamp(time(0));
	// If any more information needs to be udated do it here.
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
	mPacket = *packet;
}
ABPacket* Pending::GetPacket()
{
	return &mPacket;
}

void Pending::SetAddress(IPAddress addr)
{
	mAddress = addr;
}
IPAddress Pending::GetAddress()
{
	return mAddress;
}
void Pending::SetSize(size_t size)
{
	mSize = size;
}
size_t Pending::GetSize()
{
	return mSize;
}
