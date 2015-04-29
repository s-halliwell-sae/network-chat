#ifndef INIMANAGER_H
#define INIMANAGER_H

#include <map>

class IniManager
{
public:

	~IniManager();

	static IniManager& getInstance()
	{
		static IniManager instance;
		return instance;
	}

	bool Init(const char* fileLoc);

	//Getters
	bool GetFloat(const std::string& key, float& outVar);
	bool GetString(const std::string& key, std::string& outVar);
	bool GetInt(const std::string& key, int& outVar);
	bool GetChar(const std::string& key, const char*& outVar);
	bool GetBool(const std::string& key, bool& outVar);

private:
	IniManager() {};
	IniManager(IniManager const&) = delete;
	void operator=(IniManager const&) = delete;
};

#endif //INIMANAGER_H