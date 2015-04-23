#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include <map>
#include <functional>
#include <string>
#include <vector>

typedef std::function<void(std::vector<std::string>&)> FunctionPointer;
//typedef std::function<void(const CBE&, std::string&)> FunctionPointer;
typedef std::map<std::string, FunctionPointer> function_map;

class CommandManager
{
public:
	CommandManager(){};
	~CommandManager(){};
	void CallFunction(const std::string& pFunction, const std::string& value);
	void AddFunction(const std::string& functionName, FunctionPointer pFunction);
private:
	function_map mFunctionMap;
};
#endif //COMMANDMANAGER_H