#pragma once

#include "entity/entity_manager/entity_manager.h"

class System
{
public:
	System() = delete;
	System(EntityManager& registry);

protected:
	EntityManager& m_registry;

};