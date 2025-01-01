#pragma once

#include "core/debug/logger.h"

#ifdef _DEBUG
#define DEBUG_BREAK() __debugbreak()
#else
#define DEBUG_BREAK()
#endif

#ifdef ENABLE_ASSERTIONS

#define ASSERT_ERROR(check, message, ...) \
	if (!(check)) { Logger::Error(message, __VA_ARGS__); DEBUG_BREAK(); }

#define ASSERT_WARN(check, message, ...) \
	if (!(check)) { Logger::Warn(message, __VA_ARGS__); }

#else
#define ASSERT_ERROR(...) if (false) {}
#define ASSERT_WARN(...)  if (false) {}
#endif