#include "CommandManager.h"
#include "Logger.h"

CommandManager::CommandManager()
{
}

void CommandManager::CallFunction(std::vector<std::string>& value)
{
	(mFMapIter->second)(value);
}

void CommandManager::AddFunction(const std::string& functionName, FunctionPointer pFunction)
{
	mFunctionMap.insert(std::pair<std::string, FunctionPointer>(functionName, pFunction));
}

bool CommandManager::FindFunction(const std::string& functionName)
{
	mFMapIter = mFunctionMap.find(functionName);
	if (mFMapIter == mFunctionMap.end())
	{
		LOG("Could not find function " + functionName);
		return false;// not found
	}
	return true;
}