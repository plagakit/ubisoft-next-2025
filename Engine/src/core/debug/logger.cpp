#include "pch.h"
#include "logger.h"

#include <iostream>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <cstdarg>
#include <windows.h>

void Logger::Init()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
}

void Logger::Log(Level level, const char* fmt, va_list args)
{
	static const char* levelStrs[] = { " [DEBUG] ", " [INFO] ", " [WARN] ", " [ERROR] " };
	static const WORD levelColors[] = {
		FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
		FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
		FOREGROUND_RED | FOREGROUND_INTENSITY
	};
	static WORD currentColor = levelColors[Level::LOG_INFO];

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (currentColor != levelColors[level])
	{
		SetConsoleTextAttribute(hConsole, levelColors[level]);
		currentColor = levelColors[level];
	}

	// Print time & level
	auto now = std::chrono::system_clock::now();
	std::time_t time_now = std::chrono::system_clock::to_time_t(now);
	std::tm* local_time = std::localtime(&time_now);

	std::cout << std::put_time(local_time, "%H:%M:%S") << levelStrs[level];

	vprintf(fmt, args);

	std::cout << std::endl;
}

void Logger::Debug(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	Log(LOG_DEBUG, fmt, args);
	va_end(args);
}

void Logger::Info(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	Log(LOG_INFO, fmt, args);
	va_end(args);
}

void Logger::Warn(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	Log(LOG_WARN, fmt, args);
	va_end(args);
}

void Logger::Error(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	Log(LOG_ERROR, fmt, args);
	va_end(args);
}

