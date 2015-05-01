#include "CBE.h"

#ifdef NC_SERVER

#include "Server.h"

const unsigned short g_Port = 40000;

int main()
{
	Server server("Test Chat Server", g_Port);

	int run = server.run();
	WSACleanup();

	return run;
}

#elif defined NC_CLIENT

#include "CBE.h"

/*
int main()
{
	CBE program = CBE();
	program.Run();
	return 0;
}
*/

int main()
{
	srand((unsigned int) time(NULL));
	IPAddress IP = IPAddress("127.0.0.1");
	IPAddress sendIP = IPAddress("127.0.0.1");
	SocketWrapper sock(sendIP, 40000);

	PacketHandler handler(&sock);
	handler.AssignAsClient();

	LOG("broad");
	sock.Broadcast();

	LOG("conn");
	ConnectToServerRequest packet_conn;
	std::string user = std::to_string(sock.getSocketPort());
	strncpy_s(packet_conn.Username, 32, user.c_str(), 32);
	sock.Send(sendIP, (ABPacket*) &packet_conn, sizeof(ConnectToServerRequest));

	LOG("create");
	PacketCreateRoomRequest packet_create;
	strncpy_s(packet_create.newRoomName, 32, "ClientRoom", 32);
	sock.Send(sendIP, (ABPacket*) &packet_create, sizeof(PacketCreateRoomRequest));

	LOG("join");
	PacketChangeRoomRequest packet_join;
	strncpy_s(packet_join.newRoomName, 32, "ClientRoom", 32);
	sock.Send(sendIP, (ABPacket*) &packet_join, sizeof(PacketChangeRoomRequest));

	LOG("msg");
	PacketMessage msg;
	msg.SetMessage("This is a message from ASDF.");
	msg.SetUserName("ASDF");
	ABPacket* message = &msg;

	LOG("change");
	PacketChangeUserNameRequest req;
	strncpy_s(req.newUserName, 32, "NewName", 32);
	sock.Send(sendIP, (ABPacket*) &req, sizeof(PacketChangeUserNameRequest));
	LOG(std::to_string(sock.getSocketPort()));
	///<<<<<<< HEAD
	sock.Send(sendIP, message, sizeof(PacketMessage));


	//	CBE mCBE = CBE();

	//=======
	//	sock.Send(sendIP, message, sizeof(PacketMessage));
	
	//>>>>>>> origin/master
	while (1)
	{
		sock.Update();
		//		mCBE.Update();
	}
	return 0;
}

#endif

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
