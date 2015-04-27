#include "CBE.h"
#include <iostream>
#include <vector>
#include <string>

int main()
{
	CBE cbe = CBE();
	cbe.mChatBox = "/createroom roomname password";
	cbe.SubmitTextBox();

	char* msg = new char();

	while (1)
	{
		cbe.Update();
		std::cin >> msg;
		cbe.mChatBox = msg;
		cbe.SubmitTextBox();
	}
	return 0;
}
/*#include <iostream>
#include "SocketWrapper.h"
#include "Packets.h"
#include "IPAddress.h"

<<<<<<< HEAD
=======
//#include "CBE.h"


//#ifdef SERVER
//
//#include "Server.h"
//
//const u_short g_Port = 40000;
//
//int main()
//{
//	Server server("TestServer", g_Port);
//
//	return server.run();
//}
//
//#else

>>>>>>> origin/master
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
	///<<<<<<< HEAD
	sock.Send(sendIP, message, sizeof(PacketMessage));

	//	CBE mCBE = CBE();

	//=======
	//	sock.Send(sendIP, message, sizeof(PacketMessage));
	sock.Broadcast();
	//>>>>>>> origin/master
	while (1)
	{
		sock.Update();
		//		mCBE.Update();
	}
	return 0;
}
<<<<<<< HEAD
*/
=======

//#endif

>>>>>>> origin/master
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
