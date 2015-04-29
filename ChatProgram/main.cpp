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

#include "Renderer.h"
#include "include\libtcod.hpp"
#include "IniManager.h"

int main()
{
	IniManager::getInstance().Init("Config/config.ini");

	Renderer rman;

	rman.SetupLayout(Renderer::CLIENT_CONNECTED);

	std::vector<std::string> fakeUsers;

	fakeUsers.push_back("Tommy");
	fakeUsers.push_back("Richard");
	fakeUsers.push_back("Harry");

	rman.SetContents("Users", fakeUsers);

	std::vector<std::string> fakeRooms;

	fakeRooms.push_back("Living Room");
	fakeRooms.push_back("Bathroom");
	fakeRooms.push_back("Kitchen");

	rman.SetContents("Rooms", fakeRooms);

	while (!TCODConsole::isWindowClosed())
	{
		//Cheeky shite
		if (rman.PressedEnter())
		{
			rman.AddEntry("Chat Log", rman.RetrieveDynamicField());
		}

		rman.Update();
	}

	return 0;
}
/*
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
*/
//#endif

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
