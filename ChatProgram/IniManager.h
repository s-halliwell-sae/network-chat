#ifndef INIMANAGER_H
#define INIMANAGER_H

#include <map>

class IniManager
{
public:

	IniManager();
	~IniManager();

	bool Init(const char* fileLoc);

	//Getters
	bool GetFloat(const std::string& key, float& outVar);
	bool GetString(const std::string& key, std::string& outVar);
	bool GetInt(const std::string& key, int& outVar);
	bool GetChar(const std::string& key, const char*& outVar);
	bool GetBool(const std::string& key, bool& outVar);

private:
	std::map <std::string, std::string> configData;
};

#endif //INIMANAGER_H