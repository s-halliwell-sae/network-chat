#include "CBE.h"

//#ifdef NC_SERVER

/*
#include "Server.h"

const u_short g_Port = 40000;

int main()
{
	Server server("TestServer", g_Port);

	return server.run();
}

//#elif defined NC_CLIENT
*/
#include "CBE.h"


int main()
{
	CBE program = CBE();
	program.Run();
	return 0;
}
/*
int main()
{
	srand(time(NULL));
	IPAddress IP = IPAddress("127.0.0.1");
	IPAddress sendIP = IPAddress("127.0.0.1");
	SocketWrapper sock(sendIP, 40000);
	//sock.Bind();

	PacketHandler handler(&sock);
	handler.AssignAsClient();

	LOG("broad");
	sock.Broadcast();

	LOG("conn");
	ConnectToServerRequest packet_conn;
	strncpy_s(packet_conn.Username, 32, std::to_string(rand()).c_str(), 32);
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
