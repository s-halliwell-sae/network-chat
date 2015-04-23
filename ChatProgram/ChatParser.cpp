#include "ChatParser.h"

ChatParser::ChatParser()
{
	mCommandManager = CommandManager();
}
ChatParser::~ChatParser(){}

CommandManager* ChatParser::GetCommandManager()
{
	return &mCommandManager;
}



