#pragma once

#include "entity/entity_manager/entity_manager.h"

class System
{
public:
	System() = delete;
	System(EntityManager& registry);

	virtual void RegisterAllRequiredComponents(size_t reserve) = 0;

protected:
	EntityManager& m_registry;

};