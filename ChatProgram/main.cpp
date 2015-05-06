#ifdef NC_SERVER

#include "Server.h"

const u_short g_Port = 40000;

int main()
{
	Server server("TestServer", g_Port);

	return server.run();
}

#elif defined NC_CLIENT

#include "CBE.h"

int main()
{
	CBE program = CBE();
	program.Run();
	return 0;
}
#endif