#include <iostream>
#include "SocketWrapper.h"
#include "Packets.h"
#include "IPAddress.h"
int main()
{
	IPAddress IP = IPAddress("127.0.0.1");
	IPAddress sendIP = IPAddress("127.0.0.1");
	SocketWrapper sock;

	PacketMessage msg;
	msg.SetMessage("MESSAGE");
	msg.SetUserName("ASDF");
	ABPacket* message = &msg;
	
	PacketHandler handler(&sock);
	sock.SetHandler(&handler);

	sock.Send(sendIP, message, sizeof(PacketMessage));
	while (1)
	{
		sock.Update();
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
