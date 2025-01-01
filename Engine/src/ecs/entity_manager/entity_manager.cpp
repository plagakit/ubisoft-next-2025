#include "pch.h"
#include "entity_manager.h"

EntityManager::EntityManager()
{
}

Entity EntityManager::CreateEntity()
{
	Entity e = 0;
	m_signatures.Insert(0, 0);

	return e;
}

void EntityManager::QueueEntityDelete(Entity entity)
{
	if (m_deletionQueue.find(entity) == m_deletionQueue.end())
		m_deletionQueue.insert(entity);
}

void EntityManager::FlushEntityDeleteQueue()
{
	for (Entity e : m_deletionQueue)
		DeleteEntity(e);
	m_deletionQueue.clear();
}

void EntityManager::DeleteEntity(Entity id) const
{
}
