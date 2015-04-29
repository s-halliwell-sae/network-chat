#include "ChatParser.h"
#include <sstream>
#include <string>
#include <iostream>

ChatParser::ChatParser(std::string chatCommandName, std::string appendCommandName)
{
	mCommandManager = CommandManager();
	mChatCommand = chatCommandName;
	mAppendCommand = appendCommandName;
}
ChatParser::ChatParser(){}
ChatParser::~ChatParser(){}

CommandManager* ChatParser::GetCommandManager()
{
	return &mCommandManager;
}

void ChatParser::Parse(std::string chatBox)
{
	std::vector<std::string> values;
	char delimiter = ' ';
	std::string parsed(chatBox);
	std::string command;
	size_t pos = 0;
	std::string token;
	if (parsed.size() > 0 && parsed.substr(0, 1) == "/")
	{
		if ((pos = parsed.find(delimiter)) != std::string::npos)
		{
			command = parsed.substr(0, pos);
			if (GetCommandManager()->FindFunction(command))
			{
				parsed.erase(0, pos + sizeof(delimiter));
				while ((pos = parsed.find(delimiter)) != std::string::npos)
				{
					token = parsed.substr(0, pos);
					values.push_back(token);
					parsed.erase(0, pos + sizeof(delimiter));
				}
				pos = parsed.length();
				if (pos != 0)
				{
					token = parsed.substr(0, pos);
					values.push_back(token);
					parsed.erase();
				}
				std::cout << std::endl << "Executing Found Command";
				GetCommandManager()->CallFunction(values);
				return;
			}
		}
	}
	if (GetCommandManager()->FindFunction(mAppendCommand))
	{
		std::cout << std::endl << "Trying to Append Colour";
		values.push_back(parsed);
		GetCommandManager()->CallFunction(values);
	}
	if (GetCommandManager()->FindFunction(mChatCommand))
	{
		std::cout << std::endl << "Trying to Send Message";
		GetCommandManager()->CallFunction(values);
	}
}