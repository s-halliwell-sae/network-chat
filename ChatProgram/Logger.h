#ifndef LOGGER_H
#define LOGGER_H

#ifdef _DEBUG
#define MINIMUM_LOG_LEVEL 1000
#elif NDEBUG
#define MINIMUM_LOG_LEVEL 100
#endif

#define DEFAULT_LOG_LEVEL 1000
#define DEFAULT_WARN_LEVEL 100
#define DEFAULT_ERROR_LEVEL 10

//Doxygen documentaion stuff

#include <string>

class Logger
{
public:
	static void Log(const char* fileName, int fileLine, const char* textIn, const int& warningLevel, const char* logTypeIn);
	static void Log(const char* fileName, int fileLine, const std::string& textIn, const int& warningLevel, const char* logTypeIn);
};

#if MINIMUM_LOG_LEVEL >= DEFAULT_LOG_LEVEL
	#define LOG(message) Logger::Log(__FILE__, __LINE__, message, DEFAULT_LOG_LEVEL, "LOG")
#else
	#define LOG(message)
#endif

#if MINIMUM_LOG_LEVEL >= DEFAULT_WARN_LEVEL
	#define LOGWARN(message) Logger::Log(__FILE__, __LINE__, message, DEFAULT_WARN_LEVEL, "WARNING")
#else
	#define LOGWARN(message)
#endif

#if MINIMUM_LOG_LEVEL >= DEFAULT_ERROR_LEVEL
	#define LOGERROR(message) Logger::Log(__FILE__, __LINE__, message, DEFAULT_ERROR_LEVEL, "ERROR")
#else
	#define LOGERROR(message)
#endif

#if MINIMUM_LOG_LEVEL > 0
	#define LOGCUSTOM(message, warningLevel, logType) Logger::Log(__FILE__, __LINE__, message, warningLevel, logType)
#else
	#define LOGCUSTOM(message, warningLevel)
#endif

#endif //LOGGER_H

