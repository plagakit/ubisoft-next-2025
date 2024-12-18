#pragma once

#include "common.h"

class Logger
{
public:
	enum Level {
		LOG_DEBUG,
		LOG_INFO,
		LOG_WARN,
		LOG_ERROR
	};

	static void Debug(std::string fmt, ...);
	static void Info(std::string fmt, ...);
	static void Warn(std::string fmt, ...);
	static void Error(std::string fmt, ...);

private:
	static void Log(Level level, std::string fmt, va_list args);
};


