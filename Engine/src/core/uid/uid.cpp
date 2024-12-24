#include "pch.h"
#include "uid.h"

UIDGenerator::UIDGenerator() :
	m_current(0)
{}

UID UIDGenerator::Next()
{
	return m_current++;
}