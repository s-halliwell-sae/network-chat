#include <iostream>
#include "SocketWrapper.h"
int main()
{
	SocketWrapper socketWrapper("127.0.0.1", 1234);

	while (1)
	{
		socketWrapper.Update();
	}

	std::cout << "Hello World!\n";
	return 0;
}
