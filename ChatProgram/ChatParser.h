#ifndef CHATPARSER_H
#define CHATPARSER_H

#include <map>
#include <functional>

typedef std::function<void(std::string&)> FunctionPointer;
typedef std::map<std::string, FunctionPointer> function_map;

class ChatParser
{
public:
	ChatParser(){};
	~ChatParser(){};
	void Parse(std::string chatBox);
	function_map mFunctionMap;
	void call_function(const std::string& pFunction);
private:
	
};

#endif //CHATPARSER_H