#pragma once
#include "pch.h"

class Logger
{
public:
	enum Level {
		LOG_DEBUG,
		LOG_INFO,
		LOG_WARN,
		LOG_ERROR
	};

	static void Init();

	static void Debug(const char* fmt, ...);
	static void Info(const char* fmt, ...);
	static void Warn(const char* fmt, ...);
	static void Error(const char* fmt, ...);

private:
	static void Log(Level level, const char* fmt, va_list args);
};


