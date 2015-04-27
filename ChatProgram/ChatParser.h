#ifndef CHATPARSER_H
#define CHATPARSER_H

#include <string>

#include "CommandManager.h"

class ChatParser
{
public:
	ChatParser(std::string chatCommandName);
	ChatParser();
	~ChatParser();
	void Parse(std::string chatBox);
	CommandManager* GetCommandManager();
private:
	CommandManager mCommandManager;
};

#endif //CHATPARSER_H