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
