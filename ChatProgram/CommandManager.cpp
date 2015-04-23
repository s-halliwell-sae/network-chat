#include "CommandManager.h"

void CommandManager::CallFunction(const std::string& pFunction, const std::string& value)
{
	function_map::const_iterator iter = mFunctionMap.find(pFunction);
	if (iter == mFunctionMap.end())
	{
		// not found
	}


	//	(*iter->second)();
}

void CommandManager::AddFunction(const std::string& functionName, FunctionPointer pFunction)
{
	mFunctionMap.insert(std::pair<std::string, FunctionPointer>(functionName, pFunction));
}