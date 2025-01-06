#include "pch.h"
#include "uid.h"

UIDGenerator::UIDGenerator() :
	m_current(1)
{}

UID UIDGenerator::Next()
{
	return m_current++;
}
