#include <iostream>
#include "SocketWrapper.h"
#include "IPAddress.h"
int main()
{
	SocketWrapper socketWrapper(IPAddress("127.0.0.1"), 1234);

	while (1)
	{
		socketWrapper.Update();
	}
	return 0;
}

/*
#include "Logger.h"

int main()
{
	LOG("Test log");
	LOGWARN("Test warning");
	LOGCUSTOM("Test custom", 1, "MyCustom");
	while (1)
	{
		
	}
	return 0;
}
*/