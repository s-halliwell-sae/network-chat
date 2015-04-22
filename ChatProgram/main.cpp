#include <iostream>
#include "SocketWrapper.h"
#include "Packets.h"
#include "IPAddress.h"
int main()
{
	IPAddress IP = IPAddress("192.168.1.109");
	IPAddress sendIP = IPAddress("192.168.1.109");
	SocketWrapper socketWrapper;

	PacketMessage msg;
	msg.SetMessage("MESSAGE");
	msg.SetUserName("ASDF");
	ABPacket* message = &msg;
	

	socketWrapper.Send(sendIP, msg.userName);
	while (1)
	{
		socketWrapper.Update();
	}
	return 0;
}

/*
#include "Logger.h"
#include "Encrypter.h"

int main()
{
	Encrypter myCrypter;

	LOG("Test");
	std::string first = myCrypter.SaltedHash("Test");
	std::string second = myCrypter.SaltedHash("Test");

	if (first == second)
	{
		LOG("Same!");
	}

	while (1)
	{
		
	}
	return 0;
}
*/
