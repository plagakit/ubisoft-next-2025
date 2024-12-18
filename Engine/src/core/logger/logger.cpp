#include "logger.h"

#include <iostream>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <cstdarg>
#include <windows.h>

void Logger::Log(Level level, std::string fmt, va_list args)
{
	const WORD defaultConsoleTextColor = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
	WORD consoleTextColor = defaultConsoleTextColor;
	std::string levelStr;

	switch (level)
	{
	case Level::LOG_DEBUG:
		levelStr = " [DEBUG] ";
		break;
	case Level::LOG_INFO:
		levelStr = " [INFO] ";
		break;
	case Level::LOG_WARN:
		levelStr = " [WARN] ";
		consoleTextColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY; // red + green = yellow!
		break;
	case Level::LOG_ERROR:
		levelStr = " [ERROR] ";
		consoleTextColor = FOREGROUND_RED | FOREGROUND_INTENSITY;
		break;
	}

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, consoleTextColor);

	// Print time & level
	auto now = std::chrono::system_clock::now();
	std::time_t time_now = std::chrono::system_clock::to_time_t(now);
	std::tm* local_time = std::localtime(&time_now);
	std::cout << std::put_time(local_time, "%H:%M:%S") << levelStr;

	vprintf(fmt.c_str(), args);

	std::cout << std::endl;
	SetConsoleTextAttribute(hConsole, defaultConsoleTextColor);
}

void Logger::Debug(std::string fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	Log(LOG_DEBUG, fmt, args);
	va_end(args);
}

void Logger::Info(std::string fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	Log(LOG_INFO, fmt, args);
	va_end(args);
}

void Logger::Warn(std::string fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	Log(LOG_WARN, fmt, args);
	va_end(args);
}

void Logger::Error(std::string fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	Log(LOG_ERROR, fmt, args);
	va_end(args);
}

