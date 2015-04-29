#ifndef CHATPARSER_H
#define CHATPARSER_H

#include <string>

#include "CommandManager.h"

class ChatParser
{
public:
	ChatParser(std::string chatCommandName, std::string appendCommandName);
	ChatParser();
	~ChatParser();
	void Parse(std::string chatBox);
	CommandManager* GetCommandManager();
private:
	CommandManager mCommandManager;
	std::string mChatCommand;
	std::string mAppendCommand;
};

#endif //CHATPARSER_H