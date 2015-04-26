#include <iostream>
#include "SocketWrapper.h"
#include "Packets.h"
#include "IPAddress.h"

//#include "CBE.h"
int main()
{
	IPAddress IP = IPAddress("127.0.0.1");
	IPAddress sendIP = IPAddress("127.0.0.1");
	SocketWrapper sock;

	PacketMessage msg;
	msg.SetMessage("This is a message from ASDF.");
	msg.SetUserName("ASDF");
	ABPacket* message = &msg;
	
	PacketHandler handler(&sock);
	handler.AssignAsClient();

	sock.Send(sendIP, message, sizeof(PacketMessage));

	sock.Broadcast();
	while (1)
	{
		sock.Update();
	}
	return 0;

	ABPacket* pack;
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

