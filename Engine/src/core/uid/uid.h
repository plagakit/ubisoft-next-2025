#pragma once

#include <cstdint>

using UID = uint32_t;

class UIDGenerator
{
public:
	static constexpr UID INVALID_UID = -1;

	UIDGenerator();
	UID Next();

private:
	UID m_current;

}; 