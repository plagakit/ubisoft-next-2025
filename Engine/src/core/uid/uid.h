#pragma once

#include <cstdint>

// TODO: make this a 64 bit int with half being magic/salt number to support versioning
// I saw this concept on an ECS blog and in an article about resource managers so it seems useful.

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