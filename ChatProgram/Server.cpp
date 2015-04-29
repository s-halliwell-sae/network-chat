#include "IPAddress.h"
#include "Logger.h"
#include "Server.h"
#include "SocketWrapper.h"
#include <Windows.h>

using std::cout;
using std::endl;

Server::Server(const string& name, unsigned short port)
{
	m_name = name;
	m_port = port;

	LOG("Constructing server: " + name + ".");

	m_socket = new SocketWrapper(IPAddress("127.0.0.1"), port);
}

int Server::run()
{
	m_running = true;

	while (m_running)
	{
		if (m_socket->CheckForWaitingData())
		{
			ABPacket* packet = m_socket->getLatestPacket();

			IPAddress senderIP(m_socket->getSenderIP());
			unsigned short senderPort = m_socket->getSenderPort();

			switch (packet->mPacketType)
			{
				case PT_MESSAGE:
				{
					PacketMessage ptm = *((PacketMessage*) packet);

					std::cout << "msg: " << ptm.message << std::endl;

					// temp non working solution
					//SocketWrapper* tmp = new SocketWrapper(senderIP, senderPort);
					//tmp->Send(senderIP, (ABPacket*) &PacketAcknowledge(), sizeof(PacketAcknowledge));
					//tmp->Close();
					//delete tmp;

					break;
				}
				case PT_DETECT_SERVER:
				{
					PacketDetectServer ptds = *((PacketDetectServer*) packet);

					std::cout << "test: " << ptds.mPacketType << std::endl;

					break;
				}
				case PT_CHANGE_ROOM_REQUEST:
				{

					break;
				}
				case PT_CHANGE_USER_NAME_REQUEST:
				{
					PacketChangeUserNameRequest ptcunr = *((PacketChangeUserNameRequest*) packet);

					User* user = getUser(ptcunr.newUserName);

					if (user)
					{
						// deny
					}
					else
					{
						user->SetName(ptcunr.newUserName);

						// send back confirmation

						std::string userString;

						for (size_t i = 0; i < m_users.size(); ++i)
							userString += m_users[i]->GetName();

						PacketUserList packet_users;
						memcpy(packet_users.roomList, userString.c_str(), 8192);

						// send new list to all users
					}

					break;
				}
				case PT_CREATE_ROOM_REQUEST:
				{
					PacketCreateRoomRequest ptcrr = *((PacketCreateRoomRequest*) packet);
					
					if (getRoom(ptcrr.newRoomName))
					{
						// if room exists, deny request as we can't have duplicate names
					}
					else
					{
						Room* newRoom = new Room(ptcrr.newRoomName);

						m_rooms.push_back(newRoom);

						// confirm room change

						std::string roomString;

						for (size_t i = 0; i < m_rooms.size(); ++i)
							roomString += m_rooms[i]->GetName();

						PacketRoomList packet_rooms;
						memcpy(packet_rooms.roomList, roomString.c_str(), 8192);

						// send room list packet to everyone
					}

					break;
				}
				case PT_CONNECT_TO_SERVER_REQUEST:
				{
					ConnectToServerRequest ptcsr = *((ConnectToServerRequest*) packet);
					
					if (getUser(ptcsr.Username))
					{
						// if user exists, deny request as we can't have duplicate names
						// may extend this to reallow disconnected users
					}
					else
					{
						User* newUser = new User(ptcsr.Username);

						m_users.push_back(newUser);
					}

					std::cout << "adf: " << ptcsr.mPacketType << std::endl;

					break;
				}
				default:
					break;
			}
		}
		else
		{
			//std::cout << "ta" << std::endl;
		}



		//std::cout << "p" << std::endl;

		

		Sleep(200);
	}

	return EXIT_SUCCESS;
}

Room* Server::getRoom(const string& room) const
{
	for (size_t i = 0; i < m_rooms.size(); ++i)
		if (strcmp(m_rooms[i]->GetName().c_str(), room.c_str()))
			return m_rooms[i];

	return nullptr;
}

User* Server::getUser(const string& user) const
{
	for (size_t i = 0; i < m_users.size(); ++i)
		if (strcmp(m_users[i]->GetName().c_str(), user.c_str()))
			return m_users[i];

	return nullptr;
}

Server::~Server()
{
	LOG("Destructing server: " + m_name + ".");

	for (size_t i = 0; i < m_users.size(); ++i)
	{
		delete m_users[i]->m_socket;
		delete m_users[i];
	}

	for (size_t i = 0; i < m_rooms.size(); ++i)
		delete m_rooms[i];

	delete m_socket;
	delete m_packetHandler;
}