#include "Logger.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <string>

using std::string;

namespace {
	std::ofstream file;
	std::stringstream ss;
	char* buffer = new char[80];
}

string FormatFileName(const char* fileNameIn);

void CurrentTime(const string& fmt);

void Logger::Log(const char* fileName, int fileLine, const char* textIn, const int& warningLevel, const char* logTypeIn)
{
	CurrentTime("%I:%M:%S %p");
	ss << "[" << buffer << "] " << logTypeIn << ": " << textIn << " (FILE: " << FormatFileName(fileName) << ", LINE: " << std::to_string(fileLine) << ")";

	CurrentTime("%d-%m-%Y");
	file.open((string("Log/") + buffer + ".txt"), std::fstream::app);
	file << ss.str() << "\n";
	file.close();

	std::cout << ss.str() << std::endl;

	ss.str("");
}

void Logger::Log(const char* fileName, int fileLine, const std::string& textIn, const int& warningLevel, const char* logTypeIn)
{
	Log(fileName, fileLine, textIn.c_str(), warningLevel, logTypeIn);
}

string FormatFileName(const char* fileNameIn)
{
	string shortName = fileNameIn;
	unsigned found = shortName.find_last_of("/\\");
	return shortName.substr(found + 1);
}

void CurrentTime(const string& fmt)
{
	buffer[0] = '\0';
	time_t currentTime = time(0);
	struct tm timeinfo;
	time(&currentTime);
	localtime_s(&timeinfo, &currentTime);
	strftime(buffer, 80, fmt.c_str(), &timeinfo);
}