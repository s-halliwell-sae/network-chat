#ifdef NC_CLIENT

#include "CBE.h"

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

#elif defined NC_SERVER

#include "Server.h"

const unsigned short g_Port = 40000;

int main()
{
	Server server("TestServer", g_Port);

	return server.run();
}

#else

#include <iostream>
#include "include\libtcod.hpp"

// /*
int main()
{
	bool endGame = false;
	TCODConsole::initRoot(80, 50, "my game", false);
	TCODSystem::setFps(25); // limit framerate to 25 frames per second
	TCODConsole::root->setDefaultForeground(TCODColor(255, 0, 0));
	TCODConsole::root->setDefaultBackground(TCODColor(128, 128, 128));
	for (int i = 0; i < 10; ++i)
	{
		TCODConsole::root->setDefaultForeground(TCODColor(i * 25, 0, 0));
		TCODConsole::root->print(10 + i, 10 + i, "hi there");
	}
	while (!endGame && !TCODConsole::isWindowClosed()) {
		// ... draw on TCODConsole::root
		TCODConsole::flush();
		TCOD_key_t key = TCODConsole::checkForKeypress();
	}
	return 0;
}

// */

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

#endif