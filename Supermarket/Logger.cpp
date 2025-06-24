#include "Logger.h"
#include "Constants.h"
#include "Worker.h"


void Logger::log(const char* message, const Worker* user)
{
	// open file to append
	std::ofstream logFile(FileName::LOG, std::ios::app);
	if (!logFile.is_open())
	{
		return;
	}

	char* currentTime = StrOpr::getTime();
	logFile << "[" << currentTime << "] ";

	if (user)// if logged user 
	{
		logFile << "[" << user->getId() << ": " << user->getFirstName() << "] " << message << std::endl;
	}
	else // if system message
	{
		logFile << "[System] " << message << std::endl;
	}

	delete[] currentTime;
}