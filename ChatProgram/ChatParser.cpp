#include "ChatParser.h"

ChatParser::ChatParser(){}
ChatParser::~ChatParser(){}

void ChatParser::call_function(const std::string& pFunction)
{
	function_map::const_iterator iter = mFunctionMap.find(pFunction);
	if (iter == mFunctionMap.end())
	{
		// not found
	}

	(*iter->second)();
}