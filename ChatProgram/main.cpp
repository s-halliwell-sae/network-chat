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
