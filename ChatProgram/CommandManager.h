#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include <map>
#include <functional>
#include <string>
#include <vector>

typedef std::function<void(std::vector<std::string>&)> FunctionPointer;
typedef std::map<std::string, FunctionPointer> function_map;

class CommandManager
{
public:
	CommandManager();
	~CommandManager(){};
	void CallFunction(std::vector<std::string>& value);
	void AddFunction(const std::string& functionName, FunctionPointer pFunction);
	bool FindFunction(const std::string& functionName);
private:
	function_map mFunctionMap;
	//function_map::const_iterator mFMapIter;
	function_map::iterator mFMapIter;
	std::string mChatCommand;
	std::string mAppendCommand;
};
#endif //COMMANDMANAGER_H