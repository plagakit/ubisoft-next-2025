#pragma once

#include <cstdint>

using UID = uint32_t;
constexpr UID INVALID_UID = 0;

class UIDGenerator
{
public:
	UIDGenerator();
	UID Next();

private:
	UID m_current;

}; 