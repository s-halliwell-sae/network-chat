#include "IniManager.h"
#include "Logger.h"
#include <fstream>
//#include <iostream>
//#include <string>

IniManager::IniManager()
{

}

IniManager::~IniManager()
{

}


bool IniManager::Init(const char* fileLoc)
{
	//Check to see if the file exists
	std::ifstream myFile(fileLoc);
	if (!myFile.is_open())
	{
		LOG(std::string("Failed to open file at ") + fileLoc);
		return false;
	}
	//Clear the existing Map
	configData.clear();

	//Load file into map
	std::string curLine;
	int iter = 0;
	while (!myFile.eof()) 
	{
		iter++;
		std::getline(myFile, curLine);
		//Check to see if there is a comment
		std::map<std::string, std::string>::iterator newEntry = configData.begin();
		if (curLine[0] != ';' && curLine[0] != '#')
		{
			std::size_t found = curLine.find('=');
			//Did we find an equals?
			if (found != std::string::npos)
			{
				configData.insert(newEntry, std::pair<std::string, std::string>(curLine.substr(0, found), curLine.substr(found + 1)));
			}
			else
			{
				LOGWARN(std::string("Failed to read '") + curLine + "' at line " + std::to_string(iter) + " of file.");
			}
		}	
	}

	return true;
}

//Getters
//Make keys const references
bool IniManager::GetFloat(const std::string& key, float& outVar)
{
	if (configData.count(key))
	{
		outVar = std::stof(configData[key], nullptr);
	}
	else
	{
		LOGWARN("No valid entry was found with key '" + key + "'.");
		return false;
	}
	return true;
}

bool IniManager::GetString(const std::string& key, std::string& outVar)
{
	if (configData.count(key))
	{
		outVar = configData[key];
	}
	else
	{
		LOGWARN("No valid entry was found with key '" + key + "'.");
		return false;
	}
	return true;
}

bool IniManager::GetInt(const std::string& key, int& outVar)
{
	if (configData.count(key))
	{
		outVar = std::stoi(configData[key]);
	}
	else
	{
		LOGWARN("No valid entry was found with key '" + key + "'.");
		return false;
	}
	return true;
}

bool IniManager::GetChar(const std::string& key, const char*& outVar)
{
	if (configData.count(key))
	{
		outVar = configData[key].c_str();
	}
	else
	{
		LOGWARN("No valid entry was found with key '" + key + "'.");
		return false;
	}
	return true;
}

bool IniManager::GetBool(const std::string& key, bool& outVar)
{
	if (configData.count(key))
	{
		if (configData[key] == "true" || configData[key] == "on" || configData[key] == "yes")
		{
			outVar = true;
		}
		else
		{
			outVar = false;
		}
	}
	else
	{
		LOGWARN("No valid entry was found with key '" + key + "'.");
		return false;
	}
	return true;
}