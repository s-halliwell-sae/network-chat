#include "CommandManager.h"
#include <iostream>

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
		std::cout << "No function named " << functionName << std::endl;
		return false;// not found
	}
	return true;
}